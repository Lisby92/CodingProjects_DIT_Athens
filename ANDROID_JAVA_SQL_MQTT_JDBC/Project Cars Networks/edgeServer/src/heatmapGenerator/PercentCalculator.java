package heatmapGenerator;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class PercentCalculator
{
  private double minLon;
  private double maxLon;
  private double minLat;
  private double maxLat;
  private int cellRows;
  private int cellCols;
  private double [][] cellMinLonArray;
  private double [][] cellMaxLonArray;
  private double [][] cellMinLatArray;
  private double [][] cellMaxLatArray;

  //constructor
  public PercentCalculator(final double minLon, final double maxLon, final double minLat, final double maxLat,
                           final int cellRows, final int cellCols)
  {
    this.minLon = minLon;
    this.maxLon = maxLon;
    this.minLat = minLat;
    this.maxLat = maxLat;
    this.cellRows = cellRows;
    this.cellCols = cellCols;
    this.cellMinLonArray = new double[cellRows][cellCols];
    this.cellMaxLonArray = new double[cellRows][cellCols];
    this.cellMinLatArray = new double[cellRows][cellCols];
    this.cellMaxLatArray = new double[cellRows][cellCols];
    //System.out.println("percentCalculator object created successfully!");
  }
  //this method calculates the mean values of RSSI or Throughput based on the corresponding heatmap cells
  //and returns them as a 2d double array
  public double[][] calculate(final boolean type, final String csvPath) throws IOException
  {
    System.out.println("Calculating mean RSSI and Throughput");
    double[][] dArray = new double[cellRows][cellCols];
    int[][] tArray = new int[cellRows][cellCols];
    //init
    for(int i = 0; i < cellRows; ++i)
    {
      for(int j = 0; j < cellCols; ++j)
      {
        dArray[i][j] = 0.0;
        tArray[i][j] = 0;
      }
    }
    BufferedReader reader = new BufferedReader(new FileReader(csvPath));
    String line;
    reader.readLine();//skip the header
    while((line = reader.readLine()) != null)//we need 2 or 3 fields and the occurences
    {
      int pos = line.indexOf(',');
      String temp = line.substring(pos+1);//skip timestep
      pos = temp.indexOf(',');
      temp = temp.substring(pos+2);//skip id and space
      pos = temp.indexOf(',');
      double curLat = Double.parseDouble(temp.substring(0, pos));
      if(curLat < minLat || curLat > maxLat)//check bounds
        continue;
      temp = temp.substring(pos+2);
      pos = temp.indexOf(',');
      double curLon = Double.parseDouble(temp.substring(0, pos));
      if(curLon < minLon || curLon > maxLon)
        continue;
      temp = temp.substring(pos+2);
      pos = temp.indexOf(',');
      temp = temp.substring(pos+2);//skip angle
      pos = temp.indexOf(',');
      temp = temp.substring(pos+2);//skip speed
      double curRSSI = 0.0;
      double curThroughput = 0.0;
      if(type)//get RSSI
      {
        pos = temp.indexOf(',');
        curRSSI = Double.parseDouble(temp.substring(0, pos));
      }
      else//skip RSSI and get throughput
      {
        pos = temp.indexOf(',');
        temp = temp.substring(pos+2);//skip RSSI
        curThroughput = Double.parseDouble(temp);
      }
      //now find out based on curLon and curLat which cell does the needed value belong to
      int c;//inversion fix
      for(int k = 0; k < cellRows; ++k)
      {
        c = 3 - k;
        double latSpread = (maxLat - minLat) / cellRows;
        double curMinLat = minLat + (k * latSpread);
        double curMaxLat = curMinLat + latSpread;
        for(int l = 0; l < cellCols; ++l)
        {
          double lonSpread = (maxLon - minLon) / cellCols;
          double curMinLon = minLon + (l * lonSpread);
          double curMaxLon = curMinLon + lonSpread;
          //set the cell bounds
          cellMinLonArray[c][l] = curMinLon;
          cellMaxLonArray[c][l] = curMaxLon;
          cellMinLatArray[c][l] = curMinLat;
          cellMaxLatArray[c][l] = curMaxLat;
          if((curLon < curMinLon || curLon > curMaxLon) || (curLat < curMinLat || curLat > curMaxLat))//not in this cell
            continue;
          ++tArray[c][l];
          if(type)//RSSI
            dArray[c][l] += curRSSI;
          else//throughput
            dArray[c][l] += curThroughput;
          break;
        }
      }
    }
    //turn them to mean
    for(int i = 0; i < cellRows; ++i)
    {
      for(int j = 0; j < cellCols; ++j)
      {
        if(tArray[i][j] > 0)//skip empty cells
          dArray[i][j] /= tArray[i][j];//calculate the mean
        else
          dArray[i][j] = -1.0;//indicate empty cells for caller function
      }
    }
    System.out.println("Calculations completed");
    return dArray;
  }

  public double[][] getCellMinLatArray()
  {
    return cellMinLatArray;
  }

  public double[][] getCellMaxLatArray()
  {
    return cellMaxLatArray;
  }

  public double[][] getCellMinLonArray()
  {
    return cellMinLonArray;
  }

  public double[][] getCellMaxLonArray()
  {
    return cellMaxLonArray;
  }
}
