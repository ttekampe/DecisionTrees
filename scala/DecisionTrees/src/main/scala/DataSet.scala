import scala.io.Source
import scala.collection.mutable.ListBuffer

class DataSet(data: Map[String, List[Double]]) {

}

object DataSet {
  def from_csv_file(filename: String): Map[String, List[Double]] = {
    val src = Source.fromFile(filename)
    val lines = src.getLines()
    val header = lines.take(1).next().split(",").drop(1)
    println("Found columns")
    println("\t" + header.mkString("\n\t"))

    val data = header.map(_ => scala.collection.mutable.ListBuffer.empty[Double])

    for(line <- lines){
      val row = line.split(",").drop(1).map(_.toDouble)
      for ((column, value) <- data zip row) column.append(value)
    }

    src.close
    (header zip data.map(_.toList)).toMap
    // DataSet((header zip data.map(_.toList)).toMap)
  }
}