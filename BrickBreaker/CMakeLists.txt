# CMakeList.txt : CMake project for BrickBreaker, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (BrickBreaker "src/BrickBreaker.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BrickBreaker PROPERTY CXX_STANDARD 20)
endif()

# Copy assets after build
copy_assets(BrickBreaker)

# TODO: Add tests and install targets if needed.
target_link_libraries(BrickBreaker PRIVATE Odysseus2D)
