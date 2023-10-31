cmake_minimum_required(VERSION 3.20)
project(wavemap_io)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

# Compiler definitions and options
add_wavemap_compile_definitions_and_options()

# Libraries
add_library(${PROJECT_NAME}
    src/file_conversions.cc
    src/stream_conversions.cc)
ament_target_dependencies(${PROJECT_NAME} rclcpp)

# Tests
if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  
  add_executable(test_${PROJECT_NAME}
      test/src/test_file_conversions.cc)
  
  target_link_libraries(test_${PROJECT_NAME} ${PROJECT_NAME})
  ament_target_dependencies(test_${PROJECT_NAME} rclcpp)
  ament_add_gtest(test_${PROJECT_NAME} EXECUTABLE test_${PROJECT_NAME})
endif()

# Install targets
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Export dependencies and targets
ament_export_dependencies(rclcpp)

ament_export_targets(${PROJECT_NAME} HAS_LIBRARY_TARGET)

ament_package()
