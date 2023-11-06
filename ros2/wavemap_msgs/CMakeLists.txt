cmake_minimum_required(VERSION 3.5)
project(wavemap_msgs)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find packages
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# Generate ROS interfaces
set(msg_files
    "HashedWaveletOctreeBlock.msg",
    "HashedWaveletOctree.msg",
    "Index3D.msg",
    "Map.msg",
    "OctreeNode.msg",
    "WaveletOctree.msg",
    "WaveletOctreeNode.msg"
)

rosidl_generate_interfaces(${PROJECT_NAME}
    ${msg_files}
)

ament_export_dependencies(rosidl_default_runtime)

ament_package()