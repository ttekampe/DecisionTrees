#ifndef CPP_INCLUDE_DATASET_H_
#define CPP_INCLUDE_DATASET_H_

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include <map>

using Index = std::vector<int>;
using Column = std::vector<double>;
using Row = std::map<std::string, double, std::less<> >;

class DataSet {
 public:
  DataSet() = default;
  explicit DataSet(const std::string& csv_file);

  std::vector<std::string> GetColumnNames() const;

  const Column& operator[](const char* columnName) const;

  const Column& operator[](const std::string& columnName) const;

  Row operator[](const size_t idx) const;

  inline const Index& getIndex() const {
    return m_index_;
  }

  template <typename T>
  void AddColumn(const std::string& column_name, const std::vector<T>& values) {
    if (auto it = std::find(m_column_names_.begin(), m_column_names_.end(),
                            column_name);
        it != m_column_names_.end()) {
      std::string exc_message;
      exc_message +=
          "Column with name " + std::string(column_name) + " already exists!";
      throw std::invalid_argument(exc_message);
    }

    if (m_size_ == 0) {
      m_size_ = values.size();
      m_index_.resize(values.size());
      std::iota(m_index_.begin(), m_index_.end(), 0);
    } else {
      if (m_size_ != values.size()) {
        throw std::range_error("values length does not match DataSet length");
      }
    }

    const Column column(values.begin(), values.end());
    m_data_.push_back(column);
    m_column_names_.push_back(column_name);
  }
  inline std::size_t getSize() const { return m_size_; }

  inline void head(const int n_rows=5) const {
    std::cout << "Index\t";
    for (const auto& col : m_column_names_){
      std::cout << col + "\t";
    }
    std::cout << std::endl;
    for (auto idx{0}; idx < n_rows; ++idx) {
      std::cout << m_index_[idx] << "\t";
      for (auto feature_idx{0}; feature_idx < m_column_names_.size(); ++feature_idx){
        std::cout << m_data_[feature_idx][idx] << "\t";
      }
      std::cout << std::endl;
    }

  }

  void ToCSV(const std::string& csv_file) const;

 private:
  std::vector<Column > m_data_;
  std::vector<std::string> m_column_names_;
  std::size_t m_size_ = 0;
  Index m_index_;
};
#endif  // CPP_INCLUDE_DATASET_H_
