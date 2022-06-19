#include <dataset.h>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<std::string> DataSet::GetColumnNames() const {
  return m_column_names_;
}

const std::vector<double>& DataSet::operator[](const char* column_name) const {
  auto it =
      std::find(m_column_names_.begin(), m_column_names_.end(), column_name);
  if (it == m_column_names_.end()) {
    std::string exc_message;
    exc_message += "Feature " + std::string(column_name) + " does not exist";
    throw std::invalid_argument(exc_message);
  }
  auto index = it - m_column_names_.begin();
  return m_data_[index];
}

const std::vector<double>& DataSet::operator[](
    const std::string& column_name) const {
  return (*this)[column_name.c_str()];
}
