#include "wavemap_ros/wavemap_server.h"

#include <std_srvs/Empty.h>
#include <wavemap/data_structure/volumetric/volumetric_data_structure_factory.h>
#include <wavemap/utils/nameof.h>
#include <wavemap_io/file_conversions.h>
#include <wavemap_msgs/FilePath.h>
#include <wavemap_msgs/Map.h>
#include <wavemap_ros_conversions/config_conversions.h>
#include <wavemap_ros_conversions/map_msg_conversions.h>

#include "wavemap_ros/input_handler/input_handler_factory.h"

namespace wavemap {
DECLARE_CONFIG_MEMBERS(WavemapServerConfig,
                      (world_frame)
                      (thresholding_period, SiUnit::kSeconds)
                      (pruning_period, SiUnit::kSeconds)
                      (publication_period, SiUnit::kSeconds)
                      (max_num_blocks_per_msg));

bool WavemapServerConfig::isValid(bool verbose) const {
  bool all_valid = true;

  all_valid &= IS_PARAM_NE(world_frame, std::string(""), verbose);
  all_valid &= IS_PARAM_GT(max_num_blocks_per_msg, 0, verbose);

  return all_valid;
}

WavemapServer::WavemapServer(ros::NodeHandle nh, ros::NodeHandle nh_private)
    : WavemapServer(nh, nh_private,
                    WavemapServerConfig::from(param::convert::toParamMap(
                        nh_private, "map/general"))) {}

WavemapServer::WavemapServer(ros::NodeHandle nh, ros::NodeHandle nh_private,
                             const WavemapServerConfig& config)
    : config_(config.checkValid()),
      transformer_(std::make_shared<TfTransformer>()) {
  // Setup data structure
  const param::Map data_structure_params =
      param::convert::toParamMap(nh_private, "map/data_structure");
  occupancy_map_ = VolumetricDataStructureFactory::create(
      data_structure_params, VolumetricDataStructureType::kHashedBlocks);
  CHECK_NOTNULL(occupancy_map_);

  // Setup input handlers
  const param::Array integrator_params_array =
      param::convert::toParamArray(nh_private, "inputs");
  for (const auto& integrator_params : integrator_params_array) {
    if (integrator_params.holds<param::Map>()) {
      const auto param_map = integrator_params.get<param::Map>();
      addInput(param_map, nh, nh_private);
    }
  }

  // Connect to ROS
  subscribeToTimers(nh);
  subscribeToTopics(nh);
  advertiseTopics(nh_private);
  advertiseServices(nh_private);
}

void WavemapServer::publishMap(bool republish_whole_map) {
  if (occupancy_map_ && !occupancy_map_->empty()) {
    if (auto* hashed_wavelet_octree =
            dynamic_cast<HashedWaveletOctree*>(occupancy_map_.get());
        hashed_wavelet_octree) {
      publishHashedMap(hashed_wavelet_octree, republish_whole_map);
    } else if (auto* hashed_chunked_wavelet_octree =
                   dynamic_cast<HashedChunkedWaveletOctree*>(
                       occupancy_map_.get());
               hashed_chunked_wavelet_octree) {
      publishHashedMap(hashed_chunked_wavelet_octree, republish_whole_map);
    } else {
      occupancy_map_->threshold();
      wavemap_msgs::Map map_msg;
      if (convert::mapToRosMsg(*occupancy_map_, map_msg.header.frame_id,
                               map_msg.header.stamp, map_msg)) {
        map_pub_.publish(map_msg);
      }
    }
  }
}

bool WavemapServer::saveMap(const std::string& file_path) const {
  if (occupancy_map_) {
    occupancy_map_->threshold();
    return io::mapToFile(*occupancy_map_, file_path);
  } else {
    LOG(ERROR) << "Could not save map because it has not yet been allocated.";
  }
  return false;
}

bool WavemapServer::loadMap(const std::string& file_path) {
  return io::fileToMap(file_path, occupancy_map_);
}

InputHandler* WavemapServer::addInput(const param::Map& integrator_params,
                                      const ros::NodeHandle& nh,
                                      ros::NodeHandle nh_private) {
  auto input_handler =
      InputHandlerFactory::create(integrator_params, config_.world_frame,
                                  occupancy_map_, transformer_, nh, nh_private);
  if (input_handler) {
    return input_handlers_.emplace_back(std::move(input_handler)).get();
  }
  return nullptr;
}

void WavemapServer::setMapUpdatedCallback(
    std::function<void(const VolumetricDataStructureBase&)> callback) {
  for (auto& input_handler : input_handlers_) {
    input_handler->setMapUpdateCallback([&map = occupancy_map_, callback]() {
      if (map) {
        callback(*map);
      }
    });
  }
}

void WavemapServer::subscribeToTimers(const ros::NodeHandle& nh) {
  if (0.f < config_.thresholding_period) {
    ROS_INFO_STREAM("Registering map thresholding timer with period "
                    << config_.thresholding_period << "s");
    map_thresholding_timer_ = nh.createTimer(
        ros::Duration(config_.thresholding_period),
        [this](const auto& /*event*/) { occupancy_map_->threshold(); });
  }

  if (0.f < config_.pruning_period) {
    ROS_INFO_STREAM("Registering map pruning timer with period "
                    << config_.pruning_period << "s");
    map_pruning_timer_ = nh.createTimer(
        ros::Duration(config_.pruning_period),
        [this](const auto& /*event*/) { occupancy_map_->prune(); });
  }

  if (0.f < config_.publication_period) {
    ROS_INFO_STREAM("Registering map publishing timer with period "
                    << config_.publication_period << "s");
    map_publication_timer_ =
        nh.createTimer(ros::Duration(config_.publication_period),
                       [this](const auto& /*event*/) { publishMap(); });
  }
}

void WavemapServer::subscribeToTopics(ros::NodeHandle& /*nh*/) {}

void WavemapServer::advertiseTopics(ros::NodeHandle& nh_private) {
  map_pub_ = nh_private.advertise<wavemap_msgs::Map>("map", 10, true);
}

void WavemapServer::advertiseServices(ros::NodeHandle& nh_private) {
  republish_whole_map_srv_ =
      nh_private.advertiseService<std_srvs::Empty::Request,
                                  std_srvs::Empty::Response>(
          "republish_whole_map", [this](auto& /*request*/, auto& /*response*/) {
            publishMap(true);
            return true;
          });

  save_map_srv_ = nh_private.advertiseService<wavemap_msgs::FilePath::Request,
                                              wavemap_msgs::FilePath::Response>(
      "save_map", [this](auto& request, auto& response) {
        response.success = saveMap(request.file_path);
        return true;
      });

  load_map_srv_ = nh_private.advertiseService<wavemap_msgs::FilePath::Request,
                                              wavemap_msgs::FilePath::Response>(
      "load_map", [this](auto& request, auto& response) {
        response.success = loadMap(request.file_path);
        return true;
      });
}
}  // namespace wavemap
