namespace DecisionTrees.UnitTests.Loss;
using DecisionTrees.Loss;

public class LogLossTests
{
    [Fact]
    public void LogLossListTest()
    {
        var lossFunction = new LogLoss();

        var yTrue = new List<double> { 0, 0, 1, 1 };
        var yPred = new List<double> { 0.1, 0.2, 0.7, 0.99 };
        var expectedLogLoss = 0.1738073366910675;

        var calculatedLogLoss = lossFunction.Evaluate(yTrue, yPred);

        Assert.Equal(expectedLogLoss, calculatedLogLoss);

    }

    [Theory]
    [InlineData(0, 0.1, 0.10536051565782628)]
    [InlineData(0, 0.2, 0.2231435513142097)]
    [InlineData(1, 0.7, 0.35667494393873245)]
    [InlineData(1, 0.99, 0.01005033585350145)]
    public void LogLossScalarTest(double yTrue, double yPred, double expectedLogLoss)
    {
        var loss_function = new LogLoss();

        var calculatedLoss = loss_function.Evaluate(yTrue, yPred);
        Assert.Equal(expectedLogLoss, calculatedLoss);
    }
}