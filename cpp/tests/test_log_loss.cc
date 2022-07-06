#include <loss.h>
#define BOOST_TEST_MODULE LossTest
#include <vector>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(LossTest)

BOOST_AUTO_TEST_CASE(log_loss_test) {
  std::vector<double> y_true{0, 0, 1, 1};
  std::vector<double> y_pred{0.1, 0.2, 0.7, 0.99};

  double expected_log_loss{0.1738073366910675};
  std::vector<double> expected_log_loss_per_sample{
      0.10536051565782628, 0.2231435513142097, 0.35667494393873245,
      0.01005033585350145};

  auto loss_function = LogLoss();
  auto calculated_log_loss = loss_function(y_true, y_pred);

  BOOST_CHECK_CLOSE(expected_log_loss, calculated_log_loss, 1e-10);

  for (unsigned int idx{0}; idx < y_true.size(); ++idx) {
    auto loss = loss_function(y_true[idx], y_pred[idx]);
    BOOST_CHECK_CLOSE(expected_log_loss_per_sample[idx], loss, 1e-10);
  }

  BOOST_CHECK_EQUAL("LogLoss", loss_function.getName());
}

BOOST_AUTO_TEST_SUITE_END()
