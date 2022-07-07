#include <dataset.h>

#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <iostream>

namespace fs = std::filesystem;

std::vector<std::string> DataSet::GetColumnNames() const {
  return m_column_names_;
}

const Column& DataSet::operator[](const char* column_name) const {
  return operator[](std::string(column_name));
}

const Column& DataSet::operator[](const std::string& column_name) const {
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

std::vector<std::string> split(const std::string& s, char delim = ',') {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, delim)) {
    elems.push_back(std::move(item));
  }
  return elems;
}

DataSet::DataSet(const std::string& csv_file) {
  auto csv_file_path = fs::path(csv_file).lexically_normal();

  std::cout << "Reading file: " << csv_file_path << std::endl;

  std::ifstream file(csv_file_path);
  if (!file) {
    throw std::invalid_argument("Unable to open file: " + csv_file);
  }
  std::string line;

  // process header
  std::getline(file, line);
  std::vector<std::string> column_names = split(line);
  bool has_index;
  if (column_names[0] == "") {
    has_index = true;
    m_column_names_ =
        std::vector<std::string>(column_names.begin() + 1, column_names.end());
  } else {
    has_index = false;
    m_column_names_ =
        std::vector<std::string>(column_names.begin(), column_names.end());
  }

  std::cout << "Detected columns:" << std::endl;
  for (const auto& col : m_column_names_) {
    std::cout << "\t" << col << std::endl;
  }

  m_data_ = std::vector<Column>(m_column_names_.size(), Column());
  unsigned int offset{0};
  if (has_index){
    m_index_ = Index();
    offset = 1;
  }

  std::vector<std::string> row;
  double val;
  while (std::getline(file, line)) {
    row = split(line);
    if (has_index){
      m_index_.push_back(std::stoi(row[0]));
    }
    for (size_t idx{0}; idx < m_column_names_.size(); ++idx) {
      val = std::stod(row[idx + offset]);
      m_data_[idx].push_back(val);
    }
  }
  m_size_ = m_data_[0].size();
  if (!has_index){
    m_index_ = Index();
    m_index_.resize(m_size_);
    std::iota(m_index_.begin(), m_index_.end(), 0);
  }
  std::cout << "Done. Read " << m_size_ << " samples" << std::endl;
}

void DataSet::ToCSV(const std::string &csv_file) const {
  std::cout << "Writing data set of " << m_size_ << " to " << csv_file << std::endl;
  std::ofstream file(csv_file);
  if (!file) {
    throw std::invalid_argument("Unable to open file: " + csv_file);
  }
  for (const auto& col : m_column_names_){
    file << "," + col;
  }
  file << std::endl;
  for (auto idx{0}; idx < m_size_; ++idx){
    file << m_index_[idx];
    for (auto idx_feature{0}; idx_feature < m_column_names_.size(); ++idx_feature){
      file << "," << m_data_[idx_feature][idx];
    }
    file << std::endl; 
  }

}

Row DataSet::operator[](const size_t idx) const {
  Row row;
  for (const auto& feature_name : m_column_names_){
    row[feature_name] = operator[](feature_name)[idx];
  }
  return row;
}

