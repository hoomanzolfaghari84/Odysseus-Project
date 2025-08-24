#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <cstdint>

#include "Odysseus2D/Core/Layer.hpp"
#include "Odysseus2D/Scene/Scene.hpp"

namespace Odysseus2D {

    using SceneID = uint32_t;

    class WorldLayer : public Layer
    {
    public:
        WorldLayer()
            : Layer("World"), m_NextSceneID(1)
        {
        }

        ~WorldLayer() override = default;

        void OnAttach() override {}
        void OnDetach() override { 
            if (Scene* active = GetActiveScene())
                active->OnStop();

            m_Scenes.clear();
            m_ActiveSceneID = 0;
        }

        void OnUpdate(Timestep ts) override
        {
            if (Scene* active = GetActiveScene())
                active->OnUpdate(ts);
        }

        // === Scene Management ===

        SceneID CreateScene(const std::string& name = "Untitled")
        {
            auto scene = std::make_unique<Scene>(
#ifdef _DEBUG
                name
#endif
            );
            return RegisterScene(std::move(scene));
        }

        SceneID RegisterScene(std::unique_ptr<Scene> scene)
        {
            SceneID id = m_NextSceneID++;
            Scene* raw = scene.get();

            m_Scenes.emplace(id, std::move(scene));

            if (m_ActiveSceneID == 0)
                SetActiveScene(id);

            return id;
        }

        void RemoveScene(SceneID id)
        {
            auto it = m_Scenes.find(id);
            if (it == m_Scenes.end())
                return;

            if (id == m_ActiveSceneID)
            {
                it->second->OnStop();
                m_ActiveSceneID = 0;
            }

            m_Scenes.erase(it);
        }

        void SetActiveScene(SceneID id)
        {
            auto it = m_Scenes.find(id);
            if (it != m_Scenes.end())
            {
                if (m_ActiveSceneID != 0)
                    GetActiveScene()->OnStop();

                m_ActiveSceneID = id;
                it->second->OnStart();
            }
        }

        Scene* GetActiveScene() const
        {
            auto it = m_Scenes.find(m_ActiveSceneID);
            return (it != m_Scenes.end()) ? it->second.get() : nullptr;
        }

        Scene* GetScene(SceneID id) const
        {
            auto it = m_Scenes.find(id);
            return (it != m_Scenes.end()) ? it->second.get() : nullptr;
        }

        const std::unordered_map<SceneID, std::unique_ptr<Scene>>& GetScenes() const
        {
            return m_Scenes;
        }

        SceneID GetActiveSceneID() const { return m_ActiveSceneID; }

    private:
        std::unordered_map<SceneID, std::unique_ptr<Scene>> m_Scenes;

        SceneID m_ActiveSceneID = 0;
        SceneID m_NextSceneID;
    };

}
