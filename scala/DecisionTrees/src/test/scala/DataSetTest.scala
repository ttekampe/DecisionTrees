import java.io.File
import org.scalatest.flatspec.AnyFlatSpec

class DataSetTest extends AnyFlatSpec {
  "A DataSet " should "be constructable from a csv file" in {
    val dataDir = sys.env("TEST_DATA_DIR")
    val dataPath = new File(dataDir, "breast_cancer.csv").getPath
    val dataSet = DataSet.from_csv_file(dataPath)

    assert(dataSet.size == 31)
    assert(dataSet.head._2.size == 569)
  }
}
