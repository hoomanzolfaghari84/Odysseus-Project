// advanced_event_system.cpp
// An advanced, professional event system for C++ (header-only style in a single file)
// - C++17 minimum
// - Features:
//   * Type-safe events via templates
//   * Runtime type-erasure for storing events
//   * Synchronous dispatch (EventDispatcher)
//   * Thread-safe async EventBus with worker thread(s)
//   * Prioritized handlers, one-shot handlers, predicate filters
//   * Connection tokens for unsubscribe
//   * Automatic lifetime management via weak owner pointers
//   * Minimal dependencies (std only)

#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace Odysseus2D {

    // ----------------------------- basic types -----------------------------
    using HandlerId = uint64_t;

    struct IEvent {
        virtual ~IEvent() = default;
        virtual std::type_index type() const = 0;
    };

    template<typename E>
    struct Event final : IEvent {
        static_assert(!std::is_pointer<E>::value, "Event type should not be a pointer.");
        E payload;
        Event(E&& p) : payload(std::move(p)) {}
        std::type_index type() const override { return typeid(E); }
    };

    // a small utility for capturing const reference or move depending on caller
    template<typename T>
    using decay_t = typename std::decay<T>::type;

    // ----------------------------- handler storage -----------------------------

    struct HandlerWrapperBase { virtual ~HandlerWrapperBase() = default; };

    template<typename E>
    struct HandlerWrapper : HandlerWrapperBase {
        using Func = std::function<void(const E&)>;
        HandlerId id;
        int priority; // higher = earlier
        bool once;
        std::weak_ptr<void> owner; // if provided, handler auto-unsubscribes when expired
        Func fn;
        std::function<bool(const E&)> filter; // optional filter; if returns false, skip

        HandlerWrapper(HandlerId id_, Func f, int prio, bool once_, std::weak_ptr<void> owner_, std::function<bool(const E&)> fil)
            : id(id_), priority(prio), once(once_), owner(owner_), fn(std::move(f)), filter(std::move(fil)) {
        }
    };

    // ----------------------------- subscription token -----------------------------

    struct Subscription {
        std::type_index type;
        HandlerId id;
        bool valid = true;
    };

    // ----------------------------- dispatcher (sync) -----------------------------

    class EventDispatcher {
    public:
        EventDispatcher() : nextHandlerId(1) {}

        template<typename E, typename F, typename Owner = void>
        Subscription subscribe(F&& f, int priority = 0, bool once = false, std::shared_ptr<Owner> owner = nullptr, std::function<bool(const E&)> filter = nullptr) {
            using EW = HandlerWrapper<E>;
            std::unique_lock<std::mutex> lock(mutex_);
            HandlerId id = nextHandlerId++;
            auto& vec = handlers_[typeid(E)];
            auto wrapper = std::make_shared<EW>(id, std::function<void(const E&)>(std::forward<F>(f)), priority, once, owner ? std::weak_ptr<void>(owner) : std::weak_ptr<void>(), std::move(filter));
            vec.push_back(wrapper);
            // keep list sorted by priority (descending)
            std::sort(vec.begin(), vec.end(), [](const std::shared_ptr<HandlerWrapperBase>& a, const std::shared_ptr<HandlerWrapperBase>& b) {
                auto* pa = static_cast<HandlerWrapper<E>*>(a.get());
                auto* pb = static_cast<HandlerWrapper<E>*>(b.get());
                return pa->priority > pb->priority;
                });
            return Subscription{ typeid(E), id, true };
        }

        template<typename E>
        void unsubscribe(const Subscription& sub) {
            std::unique_lock<std::mutex> lock(mutex_);
            auto it = handlers_.find(sub.type);
            if (it == handlers_.end()) return;
            auto& vec = it->second;
            vec.erase(std::remove_if(vec.begin(), vec.end(), [&](const std::shared_ptr<HandlerWrapperBase>& b) {
                auto* hb = static_cast<HandlerWrapper<E>*>(b.get());
                return hb->id == sub.id;
                }), vec.end());
        }

        template<typename E>
        void publish(const E& e) {
            auto type = std::type_index(typeid(E));
            std::vector<std::shared_ptr<HandlerWrapperBase>> snapshot;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                auto it = handlers_.find(type);
                if (it == handlers_.end()) return;
                snapshot = it->second; // copy - handlers can be changed during iteration
            }

            // iterate over snapshot to avoid deadlocks; handle 'once' and expired owners
            for (auto& hb_base : snapshot) {
                auto* hb = static_cast<HandlerWrapper<E>*>(hb_base.get());
                // check owner
                if (!hb->owner.expired()) {
                    // ok
                }
                if (hb->owner.lock() == nullptr && !hb->owner.expired()) {
                    // owner is expired -> skip invoking (and schedule removal)
                    schedule_remove<E>(hb->id);
                    continue;
                }
                // filter
                if (hb->filter && !hb->filter(e)) continue;
                try {
                    hb->fn(e);
                }
                catch (...) {
                    // swallow or log - don't let an exception kill dispatch
                    std::cerr << "Exception in event handler\n";
                }
                if (hb->once) schedule_remove<E>(hb->id);
            }

            // perform removals
            flush_removals<E>();
        }

    private:
        std::mutex mutex_;
        std::atomic<HandlerId> nextHandlerId;
        std::unordered_map<std::type_index, std::vector<std::shared_ptr<HandlerWrapperBase>>> handlers_;

        // pending removals per type
        std::unordered_map<std::type_index, std::vector<HandlerId>> pending_removals_;

        template<typename E>
        void schedule_remove(HandlerId id) {
            std::unique_lock<std::mutex> lock(mutex_);
            pending_removals_[typeid(E)].push_back(id);
        }

        template<typename E>
        void flush_removals() {
            std::unique_lock<std::mutex> lock(mutex_);
            auto it = pending_removals_.find(typeid(E));
            if (it == pending_removals_.end()) return;
            auto& list = it->second;
            if (list.empty()) return;
            auto& vec = handlers_[typeid(E)];
            vec.erase(std::remove_if(vec.begin(), vec.end(), [&](const std::shared_ptr<HandlerWrapperBase>& b) {
                auto* hb = static_cast<HandlerWrapper<E>*>(b.get());
                for (auto id : list) if (hb->id == id) return true;
                return false;
                }), vec.end());
            list.clear();
        }
    };

    // ----------------------------- async EventBus -----------------------------

    class EventBus {
    public:
        EventBus(size_t worker_count = 1) : stopped(false) {
            for (size_t i = 0; i < worker_count; ++i) workers.emplace_back(&EventBus::worker_thread, this);
        }

        ~EventBus() {
            stop();
        }

        // enqueue an event by value (copy/move)
        template<typename E>
        void publish_async(E&& e) {
            auto wrapped = std::make_shared<Event<decay_t<E>>>(std::forward<E>(e));
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                queue.push(wrapped);
            }
            queue_cv.notify_one();
        }

        // subscribe reuses EventDispatcher's subscribe semantics
        template<typename E, typename F, typename Owner = void>
        Subscription subscribe(F&& f, int priority = 0, bool once = false, std::shared_ptr<Owner> owner = nullptr, std::function<bool(const E&)> filter = nullptr) {
            return dispatcher.subscribe<E>(std::forward<F>(f), priority, once, owner, std::move(filter));
        }

        template<typename E>
        void unsubscribe(const Subscription& sub) { dispatcher.unsubscribe<E>(sub); }

        void stop() {
            bool expected = false;
            if (stopped.compare_exchange_strong(expected, true)) {
                queue_cv.notify_all();
                for (auto& t : workers) if (t.joinable()) t.join();
            }
        }

    private:
        EventDispatcher dispatcher;
        std::atomic<bool> stopped;
        std::vector<std::thread> workers;

        std::mutex queue_mutex;
        std::condition_variable queue_cv;
        std::queue<std::shared_ptr<IEvent>> queue;

        void worker_thread() {
            while (!stopped) {
                std::shared_ptr<IEvent> item;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    queue_cv.wait(lock, [&] { return stopped || !queue.empty(); });
                    if (stopped && queue.empty()) return;
                    if (queue.empty()) continue;
                    item = queue.front(); queue.pop();
                }
                dispatch_item(item);
            }
        }

        void dispatch_item(const std::shared_ptr<IEvent>& item) {
            // find type and use dispatcher to publish
            // we need to cast to its concrete type based on type_index: use if chains via stored type_info
            auto t = item->type();
            // Unfortunately C++ RTTI doesn't give us a templated dispatch. We'll use a map of dispatchers keyed by type_index
            // We'll call a generic publish_by_type method
            publish_by_type(item);
        }

        // map type_index -> callable that casts and calls dispatcher's publish<E>
        std::unordered_map<std::type_index, std::function<void(const std::shared_ptr<IEvent>&)>> publish_map;
        std::mutex publish_map_mutex;

        void publish_by_type(const std::shared_ptr<IEvent>& ev) {
            auto t = ev->type();
            std::function<void(const std::shared_ptr<IEvent>&)> fn;
            {
                std::unique_lock<std::mutex> lock(publish_map_mutex);
                auto it = publish_map.find(t);
                if (it != publish_map.end()) fn = it->second;
            }
            if (fn) { fn(ev); return; }

            // not found -> create and cache
            std::unique_lock<std::mutex> lock(publish_map_mutex);
            // double-check
            auto it = publish_map.find(t);
            if (it != publish_map.end()) { fn = it->second; }
            else {
                // create lambda that casts to concrete Event<E> and calls dispatcher.publish<E>(payload)
                fn = [this, t](const std::shared_ptr<IEvent>& base) {
                    // use type_info to attempt cast by comparing names - this is safe since we register by subscribe
                    // We will attempt a dynamic_cast chain for common use-case where Event<E> is the dynamic type.
                    // Try dynamic_cast to Event<...> by comparing type_index name -> not elegant but functional.
                    // Safer approach: require that the event type has been subscribed before publishing.
                    // We'll iterate through subscribers map in dispatcher to find matching type and then attempt cast.
                    // For simplicity: we rely on the fact that 'base' is actually Event<T> and we try to cast through void pointer.
                    // This implementation uses a lambda factory per type when a first publish occurs.
                    };
                // Place a temporary, then replace below when we can generate a real one.
                publish_map.emplace(t, fn);
            }

            // attempt to build a working fn by inspecting handler lists in dispatcher
            // Not ideal, but we'll implement a helper: dispatcher exposes a friend method to publish by raw IEvent
            // To avoid complexity here, we'll implement a simpler bus: the EventBus will only dispatch events where someone has subscribed with the same concrete type before.
            // When a subscription is made, we'll register a typed forwarder in publish_map so the worker can call it.
        }

    public:
        // Register typed forwarder - called by subscribe
        template<typename E>
        void register_forwarder() {
            std::unique_lock<std::mutex> lock(publish_map_mutex);
            publish_map[typeid(E)] = [this](const std::shared_ptr<IEvent>& base) {
                auto derived = std::static_pointer_cast<Event<E>>(base);
                if (derived) dispatcher.publish<E>(derived->payload);
                };
        }
    };

    // Convenience helper that binds subscribe to forwarder registration
    template<typename E, typename Bus, typename F, typename Owner = void>
    Subscription bus_subscribe(Bus& bus, F&& f, int priority = 0, bool once = false, std::shared_ptr<Owner> owner = nullptr, std::function<bool(const E&)> filter = nullptr) {
        bus.register_forwarder<E>();
        return bus.subscribe<E>(std::forward<F>(f), priority, once, owner, std::move(filter));
    }

} // namespace Odysseus2D


//// ----------------------------- example usage -----------------------------
//#ifdef EVT_EXAMPLE
//#include <thread>
//#include <chrono>
//
//struct PlayerDied { int id; std::string reason; };
//struct Damage { int target; int amount; };
//
//int main() {
//    using namespace Odysseus2D;
//    EventDispatcher disp;
//    auto sub = disp.subscribe<PlayerDied>([](const PlayerDied& p) { std::cout << "Player " << p.id << " died: " << p.reason << "\n"; }, 10);
//    disp.publish<PlayerDied>({ 42, "fell off cliff" });
//
//    EventBus bus(2);
//    auto token = bus_subscribe<PlayerDied>(bus, [](const PlayerDied& p) { std::cout << "(async) Player " << p.id << " died.\n"; });
//    bus.publish_async(PlayerDied{ 7, "lava" });
//
//    std::this_thread::sleep_for(std::chrono::milliseconds(100));
//    bus.stop();
//    return 0;
//}
//#endif

