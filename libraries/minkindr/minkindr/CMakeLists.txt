cmake_minimum_required(VERSION 3.20)
project(minkindr)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(Threads REQUIRED)

# Add all files to show up in QtCreator.
FILE(GLOB_RECURSE LibFiles "include/*")
add_custom_target(headers SOURCES ${LibFiles})

# Install targets
install(DIRECTORY include/
  DESTINATION include
)

# Export dependencies and targets
ament_export_include_directories(include)


ament_package()