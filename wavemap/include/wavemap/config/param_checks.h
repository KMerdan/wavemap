#ifndef WAVEMAP_CONFIG_PARAM_CHECKS_H_
#define WAVEMAP_CONFIG_PARAM_CHECKS_H_

#include <functional>
#include <string>

#include "wavemap/utils/nameof.h"

namespace wavemap {
#define IS_PARAM_EQ(value, threshold, verbose) \
  is_param<std::equal_to<>>(value, threshold, verbose, NAMEOF(value), "==")

#define IS_PARAM_NE(value, threshold, verbose) \
  is_param<std::not_equal_to<>>(value, threshold, verbose, NAMEOF(value), "!=")

#define IS_PARAM_LT(value, threshold, verbose) \
  is_param<std::less<>>(value, threshold, verbose, NAMEOF(value), "<")

#define IS_PARAM_LE(value, threshold, verbose) \
  is_param<std::less_equal<>>(value, threshold, verbose, NAMEOF(value), "<=")

#define IS_PARAM_GT(value, threshold, verbose) \
  is_param<std::greater<>>(value, threshold, verbose, NAMEOF(value), ">")

#define IS_PARAM_GE(value, threshold, verbose) \
  is_param<std::greater_equal<>>(value, threshold, verbose, NAMEOF(value), ">=")

template <typename ComparisonOp, typename T>
bool is_param(T value, T threshold) {
  return ComparisonOp{}(value, threshold);
}

template <typename ComparisonOp, typename T>
bool is_param(T value, T threshold, bool verbose, const std::string& value_name,
              const std::string& comparison_op_name) {
  if (is_param<ComparisonOp, T>(value, threshold)) {
    return true;
  } else {
    LOG_IF(WARNING, verbose) << "Param \"" << value_name << "\" is not "
                             << comparison_op_name << " " << threshold;
    return false;
  }
}
}  // namespace wavemap

#endif  // WAVEMAP_CONFIG_PARAM_CHECKS_H_
