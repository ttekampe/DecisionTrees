#include <dataset.h>
#define BOOST_TEST_MODULE DatasetTest
#include <boost/test/unit_test.hpp>
#include <cstdlib>
#include <vector>

BOOST_AUTO_TEST_SUITE(DatasetTest)

template <typename T>
std::vector<double> ConvertVectorToDouble(const std::vector<T>& some_vector) {
  return std::vector<double>(some_vector.begin(), some_vector.end());
}

BOOST_AUTO_TEST_CASE(data_set_test) {
  std::vector<float> float_values{42.0f, 3.14f};
  std::vector<double> double_values{42.0, 3.14};
  std::vector<int> int_values{42, 3};
  std::vector<short> short_values{0, 1};
  std::vector<long> long_values{1337, 8080};
  std::vector<bool> bool_values{true, false};

  DataSet test_data;
  test_data.AddColumn("float_feature", float_values);
  test_data.AddColumn("double_feature", double_values);
  test_data.AddColumn("int_feature", int_values);
  test_data.AddColumn("short_feature", short_values);
  test_data.AddColumn("long_feature", long_values);
  test_data.AddColumn("bool_feature", bool_values);

  std::vector<std::string> expected_column_names{
      "float_feature", "double_feature", "int_feature",
      "short_feature", "long_feature",   "bool_feature"};

  auto actual_column_names = test_data.GetColumnNames();

  BOOST_CHECK_EQUAL_COLLECTIONS(
      expected_column_names.begin(), expected_column_names.end(),
      actual_column_names.begin(), actual_column_names.end());

  auto floatFeature = test_data["float_feature"];
  auto float_values_d = ConvertVectorToDouble(float_values);
  BOOST_CHECK_EQUAL_COLLECTIONS(float_values_d.begin(), float_values_d.end(),
                                floatFeature.begin(), floatFeature.end());

  std::string double_feature_name{"double_feature"};
  auto double_feature = test_data[double_feature_name];
  BOOST_CHECK_EQUAL_COLLECTIONS(double_values.begin(), double_values.end(),
                                double_feature.begin(), double_feature.end());

  auto int_feature = test_data["int_feature"];
  auto int_values_d = ConvertVectorToDouble(int_values);
  BOOST_CHECK_EQUAL_COLLECTIONS(int_values_d.begin(), int_values_d.end(),
                                int_feature.begin(), int_feature.end());

  std::string short_feature_name{"short_feature"};
  auto short_feature = test_data[short_feature_name];
  auto short_values_d = ConvertVectorToDouble(short_values);
  BOOST_CHECK_EQUAL_COLLECTIONS(short_values_d.begin(), short_values_d.end(),
                                short_feature.begin(), short_feature.end());

  auto long_feature = test_data["long_feature"];
  auto long_values_d = ConvertVectorToDouble(long_values);
  BOOST_CHECK_EQUAL_COLLECTIONS(long_values_d.begin(), long_values_d.end(),
                                long_feature.begin(), long_feature.end());

  std::string bool_feature_name{"bool_feature"};
  auto bool_feature = test_data[bool_feature_name];
  auto bool_values_d = ConvertVectorToDouble(bool_values);
  BOOST_CHECK_EQUAL_COLLECTIONS(bool_values_d.begin(), bool_values_d.end(),
                                bool_feature.begin(), bool_feature.end());
}

BOOST_AUTO_TEST_CASE(data_set_from_csv_test) {
  auto test_data_path = std::string(std::getenv("TEST_DATA_DIR"));
  auto csv_file = test_data_path + "/reduced_iris.csv";

  BOOST_TEST_MESSAGE("Reading " + csv_file);

  DataSet test_data(csv_file);

  int expected_length{100};

  BOOST_CHECK_EQUAL(test_data.getSize(), expected_length);

  std::vector<std::string> expected_colum_names = {
      "sepal length (cm)", "sepal width (cm)", "petal length (cm)",
      "petal width (cm)", "target"};

  auto actual_column_names = test_data.GetColumnNames();

  BOOST_CHECK_EQUAL_COLLECTIONS(
      actual_column_names.begin(), actual_column_names.end(),
      expected_colum_names.begin(), expected_colum_names.end());
}

BOOST_AUTO_TEST_SUITE_END()
