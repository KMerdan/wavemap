#ifndef WAVEMAP_INTEGRATOR_PROJECTIVE_FIXED_RESOLUTION_FIXED_RESOLUTION_INTEGRATOR_H_
#define WAVEMAP_INTEGRATOR_PROJECTIVE_FIXED_RESOLUTION_FIXED_RESOLUTION_INTEGRATOR_H_

#include <memory>
#include <utility>

#include "wavemap/data_structure/aabb.h"
#include "wavemap/data_structure/image.h"
#include "wavemap/integrator/projective/projective_integrator.h"

namespace wavemap {
class FixedResolutionIntegrator : public ProjectiveIntegrator {
 public:
  FixedResolutionIntegrator(const ProjectiveIntegratorConfig& config,
                            ProjectorBase::ConstPtr projection_model,
                            PosedImage<>::Ptr posed_range_image,
                            Image<Vector2D>::Ptr beam_offset_image,
                            MeasurementModelBase::ConstPtr measurement_model,
                            VolumetricDataStructureBase::Ptr occupancy_map)
      : ProjectiveIntegrator(
            config, std::move(projection_model), std::move(posed_range_image),
            std::move(beam_offset_image), std::move(measurement_model)),
        occupancy_map_(std::move(CHECK_NOTNULL(occupancy_map))) {}

 private:
  const VolumetricDataStructureBase::Ptr occupancy_map_;
  AABB<Point3D> aabb_;

  void importPointcloud(const PosedPointcloud<>& pointcloud) override;
  void importRangeImage(const PosedImage<>& range_image_input) override;

  void updateMap() override;
};
}  // namespace wavemap

#endif  // WAVEMAP_INTEGRATOR_PROJECTIVE_FIXED_RESOLUTION_FIXED_RESOLUTION_INTEGRATOR_H_
