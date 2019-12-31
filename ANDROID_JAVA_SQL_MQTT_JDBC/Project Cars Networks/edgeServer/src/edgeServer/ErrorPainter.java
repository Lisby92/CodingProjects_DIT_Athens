package edgeServer;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartUtilities;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import java.io.File;
import java.io.IOException;
import java.util.LinkedList;
import java.util.ListIterator;

public class ErrorPainter
{
  private LinkedList<Double> xVals;
  private LinkedList<Double> yVals;
  private int terminalID;

  //constructor
  public ErrorPainter(final LinkedList<Double> xVals, final LinkedList<Double> yVals, final int terminalID)
  {
    this.xVals = xVals;
    this.yVals = yVals;
    this.terminalID = terminalID;
    //System.out.println("Just created an ErrorPainter object");
  }

  //this method uses the given x and y values and creates a graphic line chart
  //to visualise the error of the geographical location predictions made by the edge server
  public void createGraph(final String location) throws IOException
  {
    System.out.println("Creating error line chart");
    XYSeries series = new XYSeries("Android Terminal " + terminalID);
    //create 2 iterators to parse the list data
    ListIterator<Double> listIterX = xVals.listIterator(0);
    ListIterator<Double> listIterY = yVals.listIterator(0);
    while(listIterX.hasNext() && listIterY.hasNext())//iterate the data and add them to the chart
      series.add(listIterX.next(), listIterY.next());
    XYSeriesCollection data = new XYSeriesCollection(series);
    //create the xy line chart
    final JFreeChart xyLineChartObject = ChartFactory.createXYLineChart("Error Predictions Line Chart",
                                                                   "Timestep (s)",
                                                                   "Distance Error (m)",
                                                                             data, PlotOrientation.VERTICAL,
                                                                      true,true,false);
    final int width = 1920;
    final int height = 1080;//full HD
    //save the chart @ specified location
    final File lineChart = new File(location);
    ChartUtilities.saveChartAsPNG(lineChart ,xyLineChartObject, width ,height);
    System.out.println("Error line chart : " + location + " exported");
  }
}
