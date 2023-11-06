cmake_minimum_required(VERSION 3.20)
project(wavemap_io)

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(wavemap REQUIRED) # Ensure this package is findable

# Include directories
include_directories(include ${wavemap_INCLUDE_DIRS})

# Libraries
add_library(${PROJECT_NAME}
    src/file_conversions.cc
    src/stream_conversions.cc)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${EIGEN3_INCLUDE_DIR}
    ${wavemap_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} ${wavemap_LIBRARIES}) # Link against wavemap libraries
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
ament_target_dependencies(${PROJECT_NAME} rclcpp wavemap) # Add wavemap to dependencies

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