#include <decisiontree.h>

#include <algorithm>
#include <functional>
#include <string>
#include <vector>

Node::Node(const std::string& feature_name_, const double& threshold_)
    : m_feature_name_(feature_name_), m_threshold_(threshold_) {}

int Node::predict(const double& value) const {
  if (value > m_threshold_) return 1;
  return 0;
}

std::vector<int>& Node::predict(
    const std::vector<double>& feature_vector) const {
  auto rv = std::vector<int>(feature_vector.size());
  std::transform(feature_vector.begin(), feature_vector.end(), rv,
                 [this](const double& val) { return this->predict(val); });
}

std::vector<int>& Node::predict(const DataSet& data_set) const {
    return (*this).predict(data_set[m_feature_name_]);
}
