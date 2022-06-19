#ifndef CPP_INCLUDE_DATASET_H_
#define CPP_INCLUDE_DATASET_H_

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

class DataSet {
 public:
  std::vector<std::string> GetColumnNames() const;

  const std::vector<double>& operator[](const char* columnName) const;

  const std::vector<double>& operator[](const std::string& columnName) const;

  template <typename T>
  void AddColumn(const std::string& column_name, const std::vector<T>& values) {
    if (auto it = std::find(m_column_names_.begin(),
                            m_column_names_.end(), column_name);
        it != m_column_names_.end()) {
      std::string exc_message;
      exc_message +=
          "Column with name " + std::string(column_name) + " already exists!";
      throw std::invalid_argument(exc_message);
    }

    const std::vector<double> column(values.begin(), values.end());
    m_data_.push_back(column);
    m_column_names_.push_back(column_name);
  }

 private:
  std::vector<std::vector<double> > m_data_;
  std::vector<std::string> m_column_names_;
};
#endif  // CPP_INCLUDE_DATASET_H_
