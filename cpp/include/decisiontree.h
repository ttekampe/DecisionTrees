#ifndef CPP_INCLUDE_DECISIONTREE_H_
#define CPP_INCLUDE_DECISIONTREE_H_

#include <dataset.h>

#include <string>
#include <vector>
#include <memory>

enum class NodeType { root = 0, branch = 1, leaf = 2 };

class Node {
 private:
  const std::string m_feature_name_;
  const double m_threshold_;
  std::unique_ptr<Node> m_left_;
  std::unique_ptr<Node> m_right_;
  NodeType m_node_type_;

 public:
  Node(const std::string& feature_name_, const double& threshold_);
  int predict(const double& value) const;
  std::vector<int>& predict(const std::vector<double>& feature_vector) const;
  std::vector<int>& predict(const DataSet& data_set) const;

  friend class DecisionTree;
};

class DecisionTree {
 public:
  void fit(const DataSet& Data);
  void predict(const DataSet& Data) const;
 private:
  std::unique_ptr<Node> m_root_;
};

#endif  // CPP_INCLUDE_DECISIONTREE_H_
