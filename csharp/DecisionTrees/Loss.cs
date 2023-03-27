namespace DecisionTrees.Loss;

public interface ILoss
{
    string Name { get; }

    public double Evaluate(List<double> truth, List<double> prediction);

    public double Evaluate(double truth, double prediction);
}

public class LogLoss : ILoss
{
    private readonly double _eps;

    public string Name { get; }

    public LogLoss(double eps = 1e-15)
    {
        _eps = eps;
        Name = "LogLoss";

    }

    public double Evaluate(List<double> truth, List<double> prediction)
    {
        if (truth.Count != prediction.Count)
        {
            throw new ArgumentException($"Predictions and truth have different sizes ({truth.Count} != {prediction.Count})");
        }
        return truth.Zip(prediction, Evaluate).Average();

    }

    public double Evaluate(double truth, double prediction)
    {
        if (truth > 0.5)
        {
            return -1.0 * Math.Log(Math.Clamp(prediction, _eps, 1 - _eps));
        }

        return -1.0 * Math.Log(1 - Math.Clamp(prediction, _eps, 1 - _eps));
    }

}

