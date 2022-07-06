#include <loss.h>

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

double LogLoss::operator()(const double &truth,
                           const double &prediction) const {
  const double &prediction_ = std::clamp(prediction, m_eps_, 1. - m_eps_);

  if (truth > 0.5)
    return -1. * log(prediction_);
  return -1. * log(1 - prediction_);
}

double LogLoss::operator()(const std::vector<double> &truth,
                           const std::vector<double> &prediction) const {
  if (prediction.size() != truth.size()) {
    throw std::range_error(
        "Predictions and truth vectors have different lengths. (" +
        std::to_string(prediction.size()) +
        " != " + std::to_string(truth.size()) + ")");
  }

  double loss_sum{0};
  for (auto idx = 0; idx < prediction.size(); ++idx) {
    loss_sum += (*this)(truth[idx], prediction[idx]);
  }

  return loss_sum / static_cast<unsigned int>(prediction.size());
}

double Gini::operator()(const std::vector<double> &truth,
                        const std::vector<double> &prediction) const {
  if (prediction.size() != truth.size()) {
    throw std::range_error(
        "Predictions and truth vectors have different lengths. (" +
        std::to_string(prediction.size()) +
        " != " + std::to_string(truth.size()) + ")");
  }

  double left_sum{0.};
  double right_sum{0.};
  int left_cnt{0};
  int right_cnt{0};
  for (auto idx{0}; idx < truth.size(); ++idx) {
    if (prediction[idx] > 0.5) {
      left_sum += truth[idx];
      left_cnt++;
    } else {
      right_sum += truth[idx];
      right_cnt++;
    }
  }

  auto left_p1 = left_cnt > 0 ? left_sum / left_cnt : 0;
  auto left_gini = 1. - left_p1 * left_p1 - (1 - left_p1) * (1 - left_p1);

  auto right_p1 = right_cnt > 0 ? right_sum / right_cnt : 0;
  auto right_gini = 1. - right_p1 * right_p1 - (1 - right_p1) * (1 - right_p1);

  return (left_cnt * left_gini + right_cnt * right_gini) /
         (left_cnt + right_cnt);
}
