# CMakeList.txt : CMake project for BrickBreaker, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Sandbox "src/Sandbox.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Sandbox PROPERTY CXX_STANDARD 20)
endif()

# Copy assets after build
#copy_assets(Sandbox ${CMAKE_CURRENT_SOURCE_DIR}/assets)


# TODO: Add tests and install targets if needed.
target_link_libraries(Sandbox PRIVATE Odysseus2D)
