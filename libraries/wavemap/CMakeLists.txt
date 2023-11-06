cmake_minimum_required(VERSION 3.20)
project(wavemap)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(minkindr REQUIRED)
find_package(glog 0.4.0 REQUIRED)
find_package(Boost REQUIRED)

include(cmake/wavemap-extras.cmake)
add_wavemap_compile_definitions_and_options()

# Include directories
include_directories(include)

# Libraries
add_library(${PROJECT_NAME}
    src/config/value_with_unit.cc
    src/data_structure/volumetric/hashed_blocks.cc
    src/data_structure/volumetric/hashed_chunked_wavelet_octree.cc
    src/data_structure/volumetric/hashed_chunked_wavelet_octree_block.cc
    src/data_structure/volumetric/hashed_wavelet_octree.cc
    src/data_structure/volumetric/hashed_wavelet_octree_block.cc
    src/data_structure/volumetric/volumetric_octree.cc
    src/data_structure/volumetric/wavelet_octree.cc
    src/data_structure/volumetric/volumetric_data_structure_base.cc
    src/data_structure/volumetric/volumetric_data_structure_factory.cc
    src/integrator/measurement_model/continuous_beam.cc
    src/integrator/measurement_model/constant_ray.cc
    src/integrator/measurement_model/continuous_ray.cc
    src/integrator/measurement_model/measurement_model_factory.cc
    src/integrator/projection_model/circular_projector.cc
    src/integrator/projection_model/ouster_projector.cc
    src/integrator/projection_model/pinhole_camera_projector.cc
    src/integrator/projection_model/spherical_projector.cc
    src/integrator/projection_model/projector_factory.cc
    src/integrator/projective/coarse_to_fine/coarse_to_fine_integrator.cc
    src/integrator/projective/coarse_to_fine/hashed_chunked_wavelet_integrator.cc
    src/integrator/projective/coarse_to_fine/hashed_wavelet_integrator.cc
    src/integrator/projective/coarse_to_fine/wavelet_integrator.cc
    src/integrator/projective/fixed_resolution/fixed_resolution_integrator.cc
    src/integrator/projective/projective_integrator.cc
    src/integrator/ray_tracing/ray_tracing_integrator.cc
    src/integrator/integrator_base.cc
    src/integrator/integrator_factory.cc
    src/utils/stopwatch.cc
    src/utils/thread_pool.cc)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${EIGEN3_INCLUDE_DIR}
    ${minkindr_INCLUDE_DIRS}
    ${glog_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${minkindr_LIBRARIES}
    ${glog_LIBRARIES}
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
ament_target_dependencies(${PROJECT_NAME} rclcpp)

# Install targets
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(EXPORT ${PROJECT_NAME}-targets
  FILE ${PROJECT_NAME}Targets.cmake
  DESTINATION lib/cmake/${PROJECT_NAME}
)

# Install header files
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.h"
)

# Export dependencies and targets
ament_export_dependencies(rclcpp)
ament_export_targets(${PROJECT_NAME}-targets HAS_LIBRARY_TARGET)

ament_export_libraries(${PROJECT_NAME})

ament_package()
