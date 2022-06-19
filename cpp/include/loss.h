#ifndef CPP_INCLUDE_LOSS_H_
#define CPP_INCLUDE_LOSS_H_

#include <string>
#include <vector>


class ILoss {
 public:
  virtual double operator()(const double& truth, const double& prediction,
                            const double& eps) const = 0;
  virtual double operator()(const std::vector<double>& truth,
                            const std::vector<double>& prediction,
                            const double& eps) const = 0;
  virtual ~ILoss() = default;
  std::string getName() const { return name; }

 protected:
  std::string name;
};

class LogLoss : public ILoss {
 public:
  LogLoss();

  double operator()(const double& truth, const double& prediction,
                    const double& eps = 1e-15) const override;
  double operator()(const std::vector<double>& truth,
                    const std::vector<double>& prediction,
                    const double& eps = 1e-15) const override;
  ~LogLoss() final = default;
};

#endif  // CPP_INCLUDE_LOSS_H_
