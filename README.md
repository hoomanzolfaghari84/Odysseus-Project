# Odysseus Project

Odysseus is a **game engine passion project**. I was inspired by [Hazel Engine](https://github.com/TheCherno/Hazel).
It is being developed alongside a **sample Brick Breaker game** that serves as a showcase of the engine’s features.

The project is still in active development — some parts are built on existing libraries, but the goal is to gradually replace them with custom implementations as the engine evolves.

---

## ✨ Features
- Entity-Component-System (**ECS**) architecture (currently using EnTT)
- (Trying to consider) Data-Oriented Programming
- 2D rendering with sprite sheets and textures (currently using SFML as backend)
- Physics system via **Box2D**
- Input handling (keyboard/mouse)
- Scene management & layering system
- Example game: **Brick Breaker**

---

## 📦 Dependencies
The project currently depends on the following libraries:

- [SFML](https://www.sfml-dev.org/) — windowing, events, and graphics
- [Box2D](https://github.com/erincatto/box2d) — 2D physics engine
- [nlohmann/json](https://github.com/nlohmann/json) — JSON configuration
- [glm](https://github.com/g-truc/glm) — mathematics library (vectors, matrices)

> 🔧 Future goal: replace some dependencies with in-house implementations as the engine matures.

---

## 🚀 Getting Started

### Clone the repository
```bash
git clone https://github.com/yourusername/odysseus.git
cd odysseus
```
### Build
Make sure you have all dependencies installed, then build with CMake:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
### Run
From the build directory:
```bash
./Odysseus
```
