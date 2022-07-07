#ifndef CPP_INCLUDE_DECISIONTREE_H_
#define CPP_INCLUDE_DECISIONTREE_H_

#include <dataset.h>
#include <loss.h>

#include <functional>
#include <iosfwd>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

class DecisionTree;

enum class NodeType { root = 0, branch = 1, leaf = 2 };
std::ostream &operator<<(std::ostream &, const NodeType &);

struct HyperParameters {
  std::shared_ptr<ILoss> criterion{std::make_shared<LogLoss>()};
  unsigned int max_depth{2};
  unsigned int min_samples_split{2};
  unsigned int min_samples_leaf{1};
  double min_weight_fraction_leaf{0.0};
  unsigned int max_features{0};
  int random_state{42};
  unsigned int max_leaf_nodes{0};
  double min_impurity_decrease{0.0};
  double class_weight{-1.0};
  double ccp_alpha{0.0};

  HyperParameters() = default;
  HyperParameters(const HyperParameters &other) = default;
};

class Node {
private:
  std::string m_feature_name_;
  std::string m_loss_name_;
  double m_threshold_;
  double m_loss_value_;
  double m_prediction_;
  int m_support_0;
  int m_support_1;
  int m_depth_;
  std::unique_ptr<Node> m_left_;
  std::unique_ptr<Node> m_right_;
  NodeType m_node_type_;
  void SetThreshold(const Column &feature, const std::string &feature_name,
                    const Column &truth, const double &threshold);

public:
  Node() : m_depth_(0), m_left_(nullptr), m_right_(nullptr) {
    m_node_type_ = NodeType::root;
  }

  explicit Node(const double &prediction, const int &support_0,
                const int &support_1, const int& depth)
      : m_prediction_(prediction), m_support_0(support_0),
        m_support_1(support_1), m_depth_(depth), m_left_(nullptr), m_right_(nullptr) {
    m_node_type_ = NodeType::leaf;
  }

  void split(const DataSet &data_set, const Column &truth,
             const HyperParameters &hyp_par, bool recursive,
             const int current_depth = 0, Index subset_indices = Index());
  double predict_proba(const double &value) const;
  double predict_proba(const Row &row) const;
  Column predict_proba(const Column &feature_vector) const;
  Column predict_proba(const DataSet &data_set) const;
  inline void print() const {
    if (m_node_type_ == NodeType::leaf) {
      std::cout << m_node_type_ << " node at depth " << m_depth_
                << " with support (" << m_support_0 << " ," << m_support_1
                << ") predicts " << m_prediction_ << std::endl;
    } else {
      std::cout << m_node_type_ << " node at depth " << m_depth_
                << " with support (" << m_support_0 << " ," << m_support_1
                << ") based on " << m_feature_name_ << " cuts at "
                << m_threshold_ << " and achieves " << m_loss_name_ << " of "
                << m_loss_value_ << std::endl;
    }
  }
  inline void print_tree() const {
    print();
    if (m_left_ != nullptr) {
      m_left_->print_tree();
    }
    if (m_right_ != nullptr) {
      m_right_->print_tree();
    }
  }
  friend DecisionTree;
};

class DecisionTree {
public:
  DecisionTree() = default;
  explicit DecisionTree(const HyperParameters &hyper_parameters)
      : m_hyper_parameters_(hyper_parameters) {}
  void fit(const DataSet &data_set, const Column &truth);
  Column predict_proba(const DataSet &data_set) const;

  inline void print_tree() const { m_root_.print_tree(); }

private:
  Node m_root_;
  HyperParameters m_hyper_parameters_;
};

#endif // CPP_INCLUDE_DECISIONTREE_H_
