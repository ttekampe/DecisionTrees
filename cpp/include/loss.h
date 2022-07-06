#ifndef CPP_INCLUDE_LOSS_H_
#define CPP_INCLUDE_LOSS_H_

#include <string>
#include <vector>

class ILoss {
public:
  explicit ILoss(const std::string& name) : m_name_(name) {}; 
  virtual double operator()(const std::vector<double> &truth,
                            const std::vector<double> &prediction) const = 0;
  virtual ~ILoss() = default;
  std::string getName() const { return m_name_; }

private:
  std::string m_name_;
};

class LogLoss : public ILoss {
public:
  explicit LogLoss(double eps = 1e-15) : ILoss("LogLoss"), m_eps_(eps) {};

  double operator()(const double &truth,
                    const double &prediction) const;
  double operator()(const std::vector<double> &truth,
                    const std::vector<double> &prediction) const override;
  ~LogLoss() override = default;

private:
  double m_eps_;
};

class Gini : public ILoss {
public:
  Gini() : ILoss("Gini") {};

  double operator()(const std::vector<double> &truth,
                    const std::vector<double> &prediction) const override;

  ~Gini() override = default;
};

#endif // CPP_INCLUDE_LOSS_H_
