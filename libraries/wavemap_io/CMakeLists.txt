cmake_minimum_required(VERSION 3.20)
project(wavemap_io)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

find_package (Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
message("-- Found Eigen3 in: ${EIGEN3_INCLUDE_DIR}")
link_directories(${Eigen_INCLUDE_DIRS})

find_package(wavemap REQUIRED) # Ensure this package is findable

# Include directories
include_directories(include ${wavemap_INCLUDE_DIRS})

# Libraries
add_library(${PROJECT_NAME}
    src/file_conversions.cc
    src/stream_conversions.cc)
ament_target_dependencies(${PROJECT_NAME} rclcpp wavemap) # Add wavemap to dependencies

# Link libraries
target_link_libraries(${PROJECT_NAME} ${wavemap_LIBRARIES}) # Link against wavemap libraries

# Install targets and create an export set
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}_targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Install header files
install(DIRECTORY include/
  DESTINATION include
)

# Export the targets
ament_export_targets(${PROJECT_NAME}_targets)

# Export dependencies
ament_export_dependencies(rclcpp wavemap)

ament_package()
