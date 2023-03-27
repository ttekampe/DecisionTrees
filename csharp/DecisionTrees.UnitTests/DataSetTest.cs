namespace DecisionTrees.UnitTests.DataSet;
using DecisionTrees.DataSet;
using Microsoft.Extensions.Logging;

public class DataSetTests
{
    [Fact]
    public void TestAddColumn()
    {
        // Given
        var floatValues = new List<float> { 42.0f, 3.14f };
        var doubleValues = new List<double> { 42.0, 3.14 };
        var intValues = new List<int> { 42, 3 };
        var shortValues = new List<short> { 0, 1 };
        var longValues = new List<long> { 1337, 8080 };
        var boolValues = new List<bool> { true, false };

        using var logFactory = LoggerFactory.Create(builder => builder.AddConsole());
        var logger = logFactory.CreateLogger<DataSet>();
        var testSDataSet = new DataSet(logger);

        // When
        testSDataSet.AddColumn("float_feature", floatValues);
        testSDataSet.AddColumn("double_feature", doubleValues);
        testSDataSet.AddColumn("int_feature", intValues);
        testSDataSet.AddColumn("short_feature", shortValues);
        testSDataSet.AddColumn("long_feature", longValues);
        testSDataSet.AddColumn("bool_feature", boolValues);

        // Then
        var expectedColumnNames = new List<string>{
            "float_feature", "double_feature", "int_feature",
            "short_feature", "long_feature",   "bool_feature"};

        var actualColumnNames = testSDataSet.GetColumnNames();

        Assert.Equal(expectedColumnNames, actualColumnNames);
    }

    [Fact]
    public void TestGetColumn()
    {
        // Given
        var floatValues = new List<float> { 42.0f, 3.14f };
        using var logFactory = LoggerFactory.Create(builder => builder.AddConsole());
        var logger = logFactory.CreateLogger<DataSet>();
        var testSDataSet = new DataSet(logger);

        // When
        testSDataSet.AddColumn("float_feature", floatValues);

        // Then
        Assert.Equal(floatValues.ConvertAll(Convert.ToDouble), testSDataSet["float_feature"]);
    }

    [Fact]
    public void TestDataSetFromCsv()
    {
        // Given
        var test_data_path = Environment.GetEnvironmentVariable("TEST_DATA_DIR");
        var csv_file = test_data_path + "/breast_cancer.csv";

        using var logFactory = LoggerFactory.Create(builder => builder.AddConsole());
        var logger = logFactory.CreateLogger<DataSet>();

        var expectedLength = 569;
        var expectedColumNames = new List<string>{
            "mean radius",
            "mean texture",
            "mean perimeter",
            "mean area",
            "mean smoothness",
            "mean compactness",
            "mean concavity",
            "mean concave points",
            "mean symmetry",
            "mean fractal dimension",
            "radius error",
            "texture error",
            "perimeter error",
            "area error",
            "smoothness error",
            "compactness error",
            "concavity error",
            "concave points error",
            "symmetry error",
            "fractal dimension error",
            "worst radius",
            "worst texture",
            "worst perimeter",
            "worst area",
            "worst smoothness",
            "worst compactness",
            "worst concavity",
            "worst concave points",
            "worst symmetry",
            "worst fractal dimension",
            "target"};

        // When
        var testDataSet = new DataSet(csv_file, logger);
        var actualColumnNames = testDataSet.GetColumnNames();
        var actualLength = testDataSet.Size;
    
        // Then
        Assert.Equal(expectedLength, actualLength);
        Assert.Equal(expectedColumNames, actualColumnNames);
    }
}