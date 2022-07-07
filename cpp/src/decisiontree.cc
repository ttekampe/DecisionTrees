#include "dataset.h"
#include <decisiontree.h>
#include <loss.h>

#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <numeric>
#include <ostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

std::ostream &operator<<(std::ostream &os, const NodeType &obj) {
  switch (obj) {
  case NodeType::root:
    os << "root";
    break;
  case NodeType::branch:
    os << "branch";
    break;
  case NodeType::leaf:
    os << "leaf";
    break;
  default:
    break;
  }
  return os;
}

double Node::predict_proba(const Row &row) const {
  if (m_node_type_ == NodeType::leaf)
    return m_prediction_;

  if (row.at(m_feature_name_) <= m_threshold_) {
    return m_left_->predict_proba(row);
  }
  return m_right_->predict_proba(row);
}

double Node::predict_proba(const double &value) const {
  if (m_node_type_ == NodeType::leaf)
    return m_prediction_;

  if (value <= m_threshold_) {
    return m_left_->predict_proba(value);
  }
  return m_right_->predict_proba(value);
}

Column Node::predict_proba(const Column &feature_vector) const {
  if (feature_vector.empty()) {
    throw std::logic_error("Predict received empty feature vector");
  }
  Column rv;
  rv.reserve(feature_vector.size());
  std::transform(
      feature_vector.begin(), feature_vector.end(), std::back_inserter(rv),
      [this](const double &val) { return this->predict_proba(val); });

  return rv;
}

Column Node::predict_proba(const DataSet &data_set) const {
  Column prediction;
  prediction.reserve(data_set.getSize());
  for (auto idx{0}; idx < data_set.getSize(); ++idx){
    prediction.push_back(predict_proba(data_set[idx]));
  }
  return prediction;

}

void Node::split(const DataSet &data_set, const Column &truth,
                 const HyperParameters &hyp_par, bool recursive,
                 const int current_depth, Index subset_indices) {
  if (truth.size() != data_set.getSize()) {
    throw std::range_error(
        "Truth and feature vectors have different lengths. (" +
        std::to_string(truth.size()) +
        " != " + std::to_string(data_set.getSize()) + ")");
  }
  m_depth_ = current_depth;
  if (m_depth_ == 0) {
    subset_indices = data_set.getIndex();
    m_support_1 = std::accumulate(truth.begin(), truth.end(), 0);
    m_support_0 = truth.size() - m_support_1;
  }

  if (auto total_support = subset_indices.size();
      total_support < hyp_par.min_samples_split) {
    std::cout << "Not splitting at depth " << m_depth_
              << " because sample size " << total_support
              << " is smaller than min_sample split "
              << hyp_par.min_samples_split << std::endl;
    return;
  }

  if (current_depth >= hyp_par.max_depth) {
    std::cout << "Not splitting at depth " << m_depth_
              << " because reached max depth " << hyp_par.max_depth
              << std::endl;
    return;
  }

  m_loss_name_ = hyp_par.criterion->getName();

  Column truth_subset;
  truth_subset.reserve(subset_indices.size());
  Column feature_subset;
  feature_subset.reserve(subset_indices.size());

  std::transform(subset_indices.begin(), subset_indices.end(),
                 std::back_inserter(truth_subset),
                 [truth](const size_t &index) { return truth[index]; });

  Index sorted_index(truth_subset.size());
  std::iota(sorted_index.begin(), sorted_index.end(), 0);

  Node test_node;
  test_node.m_node_type_ = NodeType::root;

  for (const auto &feature_name : data_set.GetColumnNames()) {
    const auto &feature = data_set[feature_name];

    std::transform(subset_indices.begin(), subset_indices.end(),
                   std::back_inserter(feature_subset),
                   [feature](const size_t &index) { return feature[index]; });

    std::sort(sorted_index.begin(), sorted_index.end(),
              [&feature_subset](const size_t &a, const size_t &b) {
                return feature_subset[a] < feature_subset[b];
              });

    for (size_t idx{0}; idx < sorted_index.size() - 1; ++idx) {
      auto threshold = feature[idx] + (feature[idx + 1] - feature[idx]) / 2.;

      test_node.SetThreshold(feature_subset, feature_name, truth_subset,
                             threshold);
      auto prediction = test_node.predict_proba(feature_subset);
      auto current_loss =
          hyp_par.criterion->operator()(truth_subset, prediction);

      if (current_loss < m_loss_value_ || m_feature_name_ == "") {
        std::cout << "Achieved better " << m_loss_name_ << " of " << current_loss << " at "
                  << feature_name << " <= " << threshold << std::endl;
        m_loss_value_ = current_loss;
        m_threshold_ = threshold;
        m_feature_name_ = feature_name;
      }
    }

    feature_subset.clear();
  }
  SetThreshold(data_set[m_feature_name_], m_feature_name_, truth_subset,
               m_threshold_);
  if (recursive) {
    auto feature = data_set[m_feature_name_];
    Index subset_indices_left;
    Index subset_indices_right;
    for (const auto &idx : subset_indices) {
      if (feature[idx] <= m_threshold_)
        subset_indices_left.push_back(idx);
      else
        subset_indices_right.push_back(idx);
    }
    m_left_->split(data_set, truth, hyp_par, recursive, current_depth + 1,
                   subset_indices_left);
    m_right_->split(data_set, truth, hyp_par, recursive, current_depth + 1,
                    subset_indices_right);
  }
}

void Node::SetThreshold(const Column &feature, const std::string &feature_name,
                        const Column &truth, const double &threshold) {
  m_threshold_ = threshold;
  m_feature_name_ = feature_name;
  if (m_node_type_ == NodeType::leaf) {
    m_node_type_ = NodeType::branch;
  }

  int support_0_left{0};
  int support_1_left{0};
  int support_0_right{0};
  int support_1_right{0};
  for (auto idx{0}; idx < feature.size(); ++idx) {
    if (feature[idx] <= threshold) {
      if (truth[idx] > 0.5) {
        support_1_left++;
      } else {
        support_0_left++;
      }
    } else {
      if (truth[idx] > 0.5) {
        support_1_right++;
      } else {
        support_0_right++;
      }
    }
  }

  auto pred_left =
      support_1_left / static_cast<double>(support_0_left + support_1_left);
  m_left_ = std::make_unique<Node>(pred_left, support_0_left, support_1_left,
                                   m_depth_ + 1);
  auto pred_right =
      support_1_right / static_cast<double>(support_0_right + support_1_right);
  m_right_ = std::make_unique<Node>(pred_right, support_0_right,
                                    support_1_right, m_depth_ + 1);
}

void DecisionTree::fit(const DataSet &data_set, const Column &truth) {
  m_root_ = Node();
  m_root_.m_node_type_ = NodeType::root;
  m_root_.split(data_set, truth, m_hyper_parameters_, true);
}

Column DecisionTree::predict_proba(const DataSet &data_set) const {
  return m_root_.predict_proba(data_set);
}