using Microsoft.Extensions.Logging;

namespace DecisionTrees.DataSet;


public class DataSet
{
    private readonly List<Column> _data;
    private readonly List<string> _columnNames;
    private readonly Index _index;
    public int Size { get; private set;}
    private readonly ILogger<DataSet> _logger;

    public DataSet(ILogger<DataSet> logger)
    {
        _logger = logger;
        _data = new List<Column>();
        Size = 0;
        _index = new List<int>();
        _columnNames = new List<string>();

    }

    public DataSet(string csvFile, ILogger<DataSet> logger)
    {
        _logger = logger;
        _index = new List<int>();

        if (!File.Exists(csvFile))
        {
            throw new ArgumentException($"Unable to open file: {csvFile}");
        }

        _logger.LogInformation("Reading data from file {csvFile}", csvFile);

        var lines = File.ReadLines(csvFile);
        _columnNames = lines.First().Split(",").Select(x => x.Trim()).ToList();
        var hasIndex = _columnNames.First() == "";
        if (hasIndex)
        {
            _columnNames.RemoveAt(0);
        }
        _logger.LogInformation("Found columns:");

        _data = new List<Column>(_columnNames.Count);
        foreach (var columnName in _columnNames)
        {
            _logger.LogInformation("\t{columnName}", columnName);
            _data.Add(new List<double>());
        }
        

        var offset = 0;
        if (hasIndex)
        {
            offset = 1;
        }


        foreach (var line in lines.Skip(1))
        {
            var values = line.Split(",");
            if (hasIndex)
            {
                _index.Add(Convert.ToInt32(values[0]));
            }
            for (int idx = 0; idx < _columnNames.Count; ++idx)
            {
                var value = Convert.ToDouble(values[idx + offset]);
                _data[idx].Add(value);
            }

        }
        Size = _data[0].Count;

        if (!hasIndex)
        {
            _index = Enumerable.Range(0, Size).ToList();
        }

        _logger.LogInformation("Done. Read {m_size_} samples", Size);
    }

    public void ToCsv(string csvFile)
    {
        using StreamWriter outFile = new(csvFile);

        outFile.WriteLine(String.Join(",", _columnNames));
        for(var idx = 0; idx < Size; idx++)
        {
            outFile.WriteLine(String.Join(",", _data.Select(column => column[idx])));
        }
  
    }

    public Row this[int idx]
    {
        get 
        {
            Row row = new();
            for(var colIdx = 0; idx < _columnNames.Count; idx++)
            {
                row[_columnNames[colIdx]] = _data[colIdx][idx];
            }
            return row;
        }
    }

    public Column this[string colName]
    {
        get
        {
            if (!_columnNames.Contains(colName))
            {
                throw new ArgumentException($"Column with name {colName} does not exist");
            }
            return _data[_columnNames.IndexOf(colName)].AsReadOnly();
        }
    }

    public IList<string> GetColumnNames()
    {
        return _columnNames.AsReadOnly();
    }

    public Index GetIndex()
    {
        return _index.AsReadOnly();
    }

    public void AddColumn<T>(string columnName, List<T> column)
    {
        if (_columnNames.Contains(columnName))
        {
            throw new ArgumentException($"Column with name {columnName} already exists");

        }
        if (column.Count != Size && Size != 0)
        {
            throw new ArgumentException("Column length and DataSet length do not match");
        }
        if (Size == 0)
        {
            Size = column.Count;
        }
        _columnNames.Add(columnName);
        Column newColumn = column.ConvertAll(x => Convert.ToDouble(x));
        _data.Add(newColumn);
    }
}