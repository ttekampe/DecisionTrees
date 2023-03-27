namespace DecisionTrees;

using DecisionTrees.Loss;
using DecisionTrees.DataSet;

public struct HyperParameters
{
    public ILoss Criterion { get; set; }
    public uint MaxDepth { get; set; }
    public uint MinSamplesSplit { get; set; }
    public uint MinSamplesLeaf { get; set; }
    public double MinWeightFractionLeaf { get; set; }
    public uint MaxFeatures { get; set; }
    public int RandomState { get; set; }
    public uint MaxLeafNodes { get; set; }
    public double MinImpurityDecrease { get; set; }
    public double ClassWeight { get; set; }
    public double CcpAlpha { get; set; }

    public HyperParameters()
    {
        Criterion = new LogLoss();
        MaxDepth = 2;
        MinSamplesSplit = 2;
        MinSamplesLeaf = 1;
        MinWeightFractionLeaf = 0;
        MaxFeatures = 0;
        RandomState = 42;
        MaxLeafNodes = 0;
        MinImpurityDecrease = 0.0;
        ClassWeight = -1.0;
        CcpAlpha = 0.0;
    }
}

enum NodeType { Root = 0, Branch = 1, Leaf = 2 };

class Node
{
    string? _featureName;
    string? _lossName;
    double _threshold;
    double _loss_value;
    double _prediction;
    int _support0;
    int _support1;
    int _depth;
    Node? _left;
    Node? _right;
    NodeType _nodeType;
    void SetThreshold(in Column feature, string featureName, in Column truth, double threshold)
    {
        _threshold = threshold;
        _featureName = featureName;
        if (_nodeType == NodeType.Leaf)
        {
            _nodeType = NodeType.Branch;
        }

        // Could do this with LINQ, but an explicit loop gives better performance
        int support0Left = 0;
        int support1Left = 0;
        int support0Right = 0;
        int support1Right = 0;
        for (int idx = 0; idx < feature.Count; ++idx)
        {
            if (feature[idx] <= threshold)
            {
                if (truth[idx] > 0.5)
                {
                    support1Left++;
                }
                else
                {
                    support0Left++;
                }
            }
            else
            {
                if (truth[idx] > 0.5)
                {
                    support1Right++;
                }
                else
                {
                    support0Right++;
                }
            }

        }

        double predLeft = support1Left / Convert.ToDouble(support0Left + support1Left);
        _left = new Node(predLeft, support0Left, support1Left, _depth + 1);
        double predRight = support1Right / Convert.ToDouble(support0Right + support1Right);
        _right = new Node(predRight, support0Right, support1Right, _depth + 1);
    }

    public Node(double prediction, int support0, int support1,  int depth)
    {
        _prediction = prediction;
        _support0 = support0;
        _support1 = support1;
        _depth = depth;
        _nodeType = NodeType.Leaf;
    }

    public Node()
    {
        _depth = 0;
        _nodeType = NodeType.Root;
    }

    public void Split(in DataSet dataSet, in Column truth, in HyperParameters hypPar, bool recursive, int currentDepth = 0, in Index? subsetIndices_)
    {
        if (dataSet.Size != truth.Count)
        {
            throw new ArgumentException("Truth and feature vectors have different lengths");
        }
        _depth = currentDepth;
        var subsetIndices = subsetIndices_ ?? dataSet.Index;
        _support1 = truth.Select(Convert.ToInt32).Sum();
        _support0 = truth.Count - _support1;
   
    }
}

public class DecisionTree
{

}
