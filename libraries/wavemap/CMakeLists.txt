cmake_minimum_required(VERSION 3.20)
project(wavemap)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

# Compiler definitions and options
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

ament_target_dependencies(${PROJECT_NAME} rclcpp)

# Install targets
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Install header files
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.h"
)

# Tests
if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  
  add_executable(test_${PROJECT_NAME}
      test/src/data_structure/test_aabb.cc
      test/src/data_structure/test_haar_cell.cc
      test/src/data_structure/test_hashed_blocks.cc
      test/src/data_structure/test_image.cc
      test/src/data_structure/test_ndtree.cc
      test/src/data_structure/test_pointcloud.cc
      test/src/data_structure/test_volumetric_data_structure.cc
      test/src/data_structure/test_volumetric_octree.cc
      test/src/data_structure/test_sparse_vector.cc
      test/src/indexing/test_index_conversions.cc
      test/src/indexing/test_ndtree_index.cc
      test/src/integrator/projection_model/test_circular_projector.cc
      test/src/integrator/projection_model/test_image_projectors.cc
      test/src/integrator/projection_model/test_spherical_projector.cc
      test/src/integrator/test_hierarchical_range_image.cc
      test/src/integrator/test_measurement_models.cc
      test/src/integrator/test_pointcloud_integrators.cc
      test/src/integrator/test_range_image_intersector.cc
      test/src/iterator/test_grid_iterator.cc
      test/src/iterator/test_ray_iterator.cc
      test/src/iterator/test_subtree_iterator.cc
      test/src/utils/test_approximate_trigonometry.cc
      test/src/utils/test_bit_manipulation.cc
      test/src/utils/test_data_utils.cc
      test/src/utils/test_fill_utils.cc
      test/src/utils/test_int_math.cc
      test/src/utils/test_log_odds_converter.cc
      test/src/utils/test_map_interpolator.cpp
      test/src/utils/test_query_accelerator.cc
      test/src/utils/test_thread_pool.cc
      test/src/utils/test_tree_math.cc)
  target_include_directories(test_${PROJECT_NAME} PRIVATE test/include)
  target_link_libraries(test_${PROJECT_NAME} ${PROJECT_NAME})
  ament_target_dependencies(test_${PROJECT_NAME} rclcpp)
  ament_add_gtest(test_${PROJECT_NAME} EXECUTABLE test_${PROJECT_NAME})
endif()

# Benchmarks
if (ENABLE_BENCHMARKING)
  add_executable(benchmark_haar_transforms
      benchmark/benchmark_haar_transforms.cc)
  target_link_libraries(benchmark_haar_transforms ${PROJECT_NAME}
      benchmark::benchmark)

  add_executable(benchmark_sparse_vector benchmark/benchmark_sparse_vector.cc)
  target_link_libraries(benchmark_sparse_vector ${PROJECT_NAME}
      benchmark::benchmark)
endif ()

# Export dependencies and targets
ament_export_dependencies(rclcpp)
ament_export_targets(${PROJECT_NAME} HAS_LIBRARY_TARGET)

ament_package()
