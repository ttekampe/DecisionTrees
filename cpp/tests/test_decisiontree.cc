#include <dataset.h>
#include <decisiontree.h>
#include <loss.h>
#define BOOST_TEST_MODULE DecisiontreeTest
#include <boost/test/unit_test.hpp>
#include <cstdlib>
#include <vector>

BOOST_AUTO_TEST_SUITE(DecisionTreeTest)

BOOST_AUTO_TEST_CASE(decision_tree_node_test) {
  auto test_data_path = std::string(std::getenv("TEST_DATA_DIR"));
  auto csv_file = test_data_path + "/breast_cancer.csv";

  BOOST_TEST_MESSAGE("Reading " + csv_file);

  DataSet test_data(csv_file);

  const auto &columns = test_data.GetColumnNames();
  BOOST_TEST_MESSAGE("Found columns:");
  for (const auto &col : columns) {
    BOOST_TEST_MESSAGE("\t\"" + col + "\"");
  }

  BOOST_TEST_MESSAGE("Setting truth sample");
  const auto &truth = test_data["target"];
  BOOST_TEST_MESSAGE("Size truth sample " + std::to_string(truth.size()));

  BOOST_TEST_MESSAGE("Setting feature sample");
  DataSet features;
  for (const auto &col : test_data.GetColumnNames()) {
    if (col == "target")
      continue;
    BOOST_TEST_MESSAGE("Adding " + col);
    features.AddColumn(col, test_data[col]);
  }
  BOOST_TEST_MESSAGE("Size features sample " +
                     std::to_string(features.getSize()));

  HyperParameters hyp_param;
  hyp_param.max_depth = 1;
  hyp_param.criterion = std::make_shared<LogLoss>();

  Node n;
  n.split(features, truth, hyp_param, false);

  n.print_tree();

  auto prediction = n.predict_proba(features);
}

BOOST_AUTO_TEST_CASE(decision_tree_test) {
  auto test_data_path = std::string(std::getenv("TEST_DATA_DIR"));
  auto csv_file = test_data_path + "/breast_cancer.csv";

  BOOST_TEST_MESSAGE("Reading " + csv_file);

  DataSet test_data(csv_file);

  const auto &columns = test_data.GetColumnNames();
  BOOST_TEST_MESSAGE("Found columns:");
  for (const auto &col : columns) {
    BOOST_TEST_MESSAGE("\t\"" + col + "\"");
  }

  BOOST_TEST_MESSAGE("Setting truth sample");
  const auto &truth = test_data["target"];
  BOOST_TEST_MESSAGE("Size truth sample " + std::to_string(truth.size()));

  BOOST_TEST_MESSAGE("Setting feature sample");
  DataSet features;
  for (const auto &col : test_data.GetColumnNames()) {
    if (col == "target")
      continue;
    BOOST_TEST_MESSAGE("Adding " + col);
    features.AddColumn(col, test_data[col]);
  }
  BOOST_TEST_MESSAGE("Size features sample " +
                     std::to_string(features.getSize()));

  HyperParameters hyp_param;
  hyp_param.max_depth = 4;
  hyp_param.criterion = std::make_shared<Gini>();

  DecisionTree dt(hyp_param);
  dt.fit(features, truth);

  dt.print_tree();

  auto prediction = dt.predict_proba(features);
  test_data.AddColumn("prediction", prediction);
  test_data.ToCSV(test_data_path + "/predicted_data.csv");

  test_data.head();
}

BOOST_AUTO_TEST_SUITE_END()
