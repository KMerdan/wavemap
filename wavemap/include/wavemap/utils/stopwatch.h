#ifndef WAVEMAP_UTILS_STOPWATCH_H_
#define WAVEMAP_UTILS_STOPWATCH_H_

#include "wavemap/utils/time.h"

namespace wavemap {
class Stopwatch {
 public:
  void start();
  void stop();

  double getLastEpisodeDuration() const {
    return to_seconds<double>(last_episode_duration_);
  }
  double getTotalDuration() const {
    return to_seconds<double>(total_duration_);
  }

 private:
  bool running = false;

  Timestamp episode_start_time_{};
  Duration last_episode_duration_{};
  Duration total_duration_{};
};
}  // namespace wavemap

#endif  // WAVEMAP_UTILS_STOPWATCH_H_
