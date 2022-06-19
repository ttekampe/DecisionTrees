#include <loss.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>

LogLoss::LogLoss() { name = "LogLoss"; }

double LogLoss::operator()(const double& truth, const double& prediction,
                           const double& eps) const {
  const double& prediction_ = std::clamp(prediction, eps, 1. - eps);

  if (truth == 1.0) return -1. * log(prediction_);
  return -1. * log(1 - prediction_);
}

double LogLoss::operator()(const std::vector<double>& truth,
                           const std::vector<double>& prediction,
                           const double& eps) const {
  if (prediction.size() != truth.size()) {
    throw std::range_error(
        "Predictions and truth vectors have different lengths");
  }

  double loss_sum{0};
  for (auto idx = 0; idx < prediction.size(); ++idx) {
    loss_sum += ( *this)(truth[idx], prediction[idx]);
  }

  return loss_sum / static_cast<unsigned int>(prediction.size());
}
