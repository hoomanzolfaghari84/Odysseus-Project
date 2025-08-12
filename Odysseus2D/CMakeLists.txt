# CMakeList.txt : CMake project for Odysseus2D, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (Odysseus2D STATIC "src/Odysseus2D.hpp"
    "src/Odysseus2D/Core/Application.hpp"
    "src/Odysseus2D/Core/Application.cpp"
    "src/Odysseus2D/Core/Layer.hpp"
    "src/Odysseus2D/Core/Layer.cpp"
    "src/Odysseus2D/Core/LayerStack.hpp"
    "src/Odysseus2D/Core/LayerStack.cpp"
    "src/Odysseus2D/Scene/Scene.cpp"
     "src/Odysseus2D/Renderer/Renderer2D.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Odysseus2D PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

target_link_libraries(Odysseus2D PUBLIC SFML::Graphics EnTT::EnTT box2d nlohmann_json glm::glm)

target_include_directories(Odysseus2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src 
    #PUBLIC include/my_public_headers
    #INTERFACE external/dependency/include
    )