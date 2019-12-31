package edgeServer;

import heatmapGenerator.GridPainter;
import heatmapGenerator.LegendPainter;
import heatmapGenerator.PercentCalculator;
import org.eclipse.paho.client.mqttv3.MqttException;
import xml2csvConverter.CSVwriter;
import xml2csvConverter.XMLParser;

import javax.imageio.ImageIO;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.sql.*;
import java.util.LinkedList;
import java.util.Properties;

public class Main
{
  private static final int cellRows = 4;
  private static final int cellCols = 10;
  private static final double minLon = 23.7647600;
  private static final double maxLon = 23.7753900;
  private static final double minLat = 37.9668800;
  private static final double maxLat = 37.9686200;
  private static final double horizon = 1.0;
  private static double[][] rssiArray;
  private static double[][] throughputArray;
  private static double [][] cellMinLonArray;
  private static double [][] cellMaxLonArray;
  private static double [][] cellMinLatArray;
  private static double [][] cellMaxLatArray;
  private static MQTTclientHandler topicHandler26;
  private static MQTTclientHandler topicHandler27;
  private static boolean firstMsg26;
  private static boolean firstMsg27;

  //this method uses a thread to clear up the database every time a csv header is received
  //meaning that the terminal either connects for the first time and sends data (therefore no effect)
  //or sends the data (for any purpose) again and row-key collisions should be avoided
  private static void clearDatabase(final int handlerID, final Connection sqlHandler)
  {
    Runnable dbRunnable = () ->
    {
      PreparedStatement prepStmt;
      String query = null;
      if(handlerID == 26)
        query = "DELETE FROM cars2019DB.dataTable WHERE (device_id = \"26\") AND (timestep >= 0)";
      else if(handlerID == 27)
        query = "DELETE FROM cars2019DB.dataTable WHERE (device_id = \"27\") AND (timestep >= 0)";
      try
      {
        synchronized(sqlHandler)
        {
          prepStmt = sqlHandler.prepareStatement(query);
          prepStmt.execute();
          prepStmt.close();
        }
      }
      catch(SQLException e)
      {
        e.printStackTrace();
      }
      System.out.println("Database cleared for android terminal " + handlerID);
    };
    Thread dbCleanerThread = new Thread(dbRunnable);
    dbCleanerThread.start();
  }

  //this method uses a thread to handle received non-header messages and update
  //the database if needed by inserting rows
  private static void handleMessage(final String message, final int handlerID,
                                    double[] predictedValsOld, double[] predictedValsNew, final Connection sqlHandler)
  {
    Runnable msgRunnable = () ->
    {
      formulaCalculator formCal = new formulaCalculator(message, horizon);
      formCal.calculatePosition();
      predictedValsNew[0] = formCal.getPredictedStep();
      predictedValsNew[1] = formCal.getPredictedLat();
      predictedValsNew[2] = formCal.getPredictedLon();
      final double curStep = formCal.getCurrentStep();
      final double currentLat = formCal.getCurrentLat();
      final double currentLon = formCal.getCurrentLon();
      final double curRSSI = formCal.getRSSI();
      final double curThroughput = formCal.getThroughput();
      int locRow = 0;
      int locCol = 0;
      boolean flag = false;
      for(int i = 0; i < cellRows; ++i)
      {
        if(flag)
          break;
        for(int j = 0; j < cellCols; ++j)
        {
          if((predictedValsNew[1] >= cellMinLatArray[i][j] && predictedValsNew[1] <= cellMaxLatArray[i][j]) &&
             (predictedValsNew[2] >= cellMinLonArray[i][j] && predictedValsNew[2] <= cellMaxLonArray[i][j]))//located the android terminal
          {
            flag = true;
            locRow = i;
            locCol = j;
            break;
          }
        }
      }
      String msgResponse;
      if(!flag)
      {
        System.out.println("Could not locate the android terminal inside the grid!");
        msgResponse = "Could not locate your position, terminal " + handlerID;
      }
      else//vehicle located @ grid, get RSSI and Throughput grid cell values
      {
        predictedValsNew[3] = rssiArray[locRow][locCol];
        predictedValsNew[4] = throughputArray[locRow][locCol];
        System.out.println("Predicted Timestep = " + predictedValsNew[0] +
                ", Predicted Lat = " + predictedValsNew[1] + ", Predicted Lon = " + predictedValsNew[2]);
        System.out.println("Predicted RSSI = " + predictedValsNew[3] + ", Predicted Throughput = " + predictedValsNew[4]);
        //send the info to the android terminals
        msgResponse = Double.toString(predictedValsNew[0]);
        msgResponse += ", ";
        msgResponse += Double.toString(predictedValsNew[1]);
        msgResponse += ", ";
        msgResponse += Double.toString(predictedValsNew[2]);
        msgResponse += ", ";
        msgResponse += Double.toString(predictedValsNew[3]);
        msgResponse += ", ";
        msgResponse += Double.toString(predictedValsNew[4]);

        if((handlerID == 26 && firstMsg26) || (handlerID == 27 && firstMsg27))//update the database
        {
          Statement stmt;
          try
          {
            synchronized(sqlHandler)
            {
              stmt = sqlHandler.createStatement(ResultSet.TYPE_SCROLL_SENSITIVE, ResultSet.CONCUR_UPDATABLE);
              ResultSet uprs = stmt.executeQuery( "SELECT * FROM cars2019DB.dataTable");
              uprs.moveToInsertRow();
              uprs.updateDouble("timestep", predictedValsOld[0]);
              uprs.updateInt("device_id", handlerID);
              uprs.updateDouble("real_lat", currentLat);
              uprs.updateDouble("real_long", currentLon);
              uprs.updateDouble("predicted_lat", predictedValsOld[1]);
              uprs.updateDouble("predicted_long", predictedValsOld[2]);
              uprs.updateDouble("real_RSSI", curRSSI);
              uprs.updateDouble("real_throughput", curThroughput);
              uprs.updateDouble("predicted_RSSI", predictedValsOld[3]);
              uprs.updateDouble("predicted_throughput", predictedValsOld[4]);
              uprs.insertRow();
              uprs.beforeFirst();
              stmt.close();
            }
          }
          catch(SQLException e)
          {
            e.printStackTrace();
          }
          System.out.println("Inserted row into database for android terminal " + handlerID);
        }
        System.arraycopy(predictedValsNew, 0, predictedValsOld, 0, 5);//old = new value copy
        if(!firstMsg26)
          if(handlerID == 26)
            firstMsg26 = true;
        else if(!firstMsg27)
          if(handlerID == 27)
            firstMsg27 = true;
      }
      if(handlerID == 26)
      {
        try
        {
          topicHandler26.MQTTpublish(msgResponse);
        }
        catch(MqttException e)
        {
          e.printStackTrace();
        }
      }
      else if(handlerID == 27)
      {
        try
        {
          topicHandler27.MQTTpublish(msgResponse);
        }
        catch(MqttException e)
        {
          e.printStackTrace();
        }
      }
    };
    Thread msgHandleThread = new Thread(msgRunnable);
    msgHandleThread.start();
  }

  //this method uses a thread to get the data inserted into the sql database
  //and use them to calculate the average error in metres based on predictions
  //and actual geographical positions of the android terminal during all timesteps
  //finally it creates a visualization of the errors as a png file
  private static void calculateError(final int handlerID, final Connection sqlHandler)
  {
    Runnable distRunnable = () ->
    {
      System.out.println("Calculating error statistics for android terminal : " + handlerID);
      distanceCalculator distCal = new distanceCalculator();
      Statement stmt;
      ResultSet downRs = null;
      try//get rows from sql data based on handlerID
      {
        synchronized(sqlHandler)
        {
          stmt = sqlHandler.createStatement(ResultSet.TYPE_SCROLL_SENSITIVE, ResultSet.CONCUR_UPDATABLE);
          if(handlerID == 26)
            downRs = stmt.executeQuery( "SELECT * FROM cars2019DB.dataTable WHERE (device_id = \"26\") AND (timestep >= 0)");
          else if(handlerID == 27)
            downRs = stmt.executeQuery( "SELECT * FROM cars2019DB.dataTable WHERE (device_id = \"27\") AND (timestep >= 0)");
        }
        if(downRs != null)//there were data to be returned
        {
          int totalRows = 0;
          double meanError = 0.0;
          int totalZeroErrs = 0;
          LinkedList<Double> errorList = new LinkedList<Double>();
          LinkedList<Double> timestepList = new LinkedList<Double>();
          while(downRs.next())//iterate through the result set and extract the needed values
          {
            final double realLat = downRs.getDouble("real_lat");
            final double realLon = downRs.getDouble("real_long");
            final double predLat = downRs.getDouble("predicted_lat");
            final double predLon = downRs.getDouble("predicted_long");
            final double predTimestep = downRs.getDouble("timestep");
            //System.out.print("Timestep : " + predTimestep);
            final double distance = distCal.calcDist(realLat, realLon, predLat, predLon);
            //System.out.println(", Error in metres : " + distance);
            meanError += distance;
            ++totalRows;
            if(distance == 0.0)
              ++totalZeroErrs;
            errorList.add(distance);//hold the distance error in metres
            timestepList.add(predTimestep);//and hold the timestep to create the graph
          }
          meanError /= totalRows;//calculate mean error
          System.out.println("\nAverage Error in metres : " + meanError);
          System.out.println("Predicted terminal's location with zero error : " + totalZeroErrs + "/" + totalRows);
          ErrorPainter errPaint = new ErrorPainter(timestepList, errorList, handlerID);
          errPaint.createGraph("error_data/errorChart_" + handlerID + ".png");
        }
        stmt.close();
      }
      catch(SQLException | IOException e)
      {
        e.printStackTrace();
      }
    };
    Thread errCalcThread = new Thread(distRunnable);
    errCalcThread.start();
  }

  public static void main(String[] args) throws IOException, MqttException, InterruptedException, SQLException {
    //parse the XML files
    XMLParser myParser = new XMLParser();
    LinkedList<String> listAll = myParser.parse("Simulation Data/Input Data/all_vehicles.xml");
    LinkedList<String> list26 = myParser.parse("Simulation Data/Input Data/vehicle_26.xml");
    LinkedList<String> list27 = myParser.parse("Simulation Data/Input Data/vehicle_27.xml");

    //create and write the CSV files
    CSVwriter writer = new CSVwriter();
    writer.writeCSV(listAll, "CSV_DATA/all_vehicles.csv");
    writer.writeCSV(list26, "CSV_DATA/vehicle_26.csv");
    writer.writeCSV(list27, "CSV_DATA/vehicle_27.csv");

    //calculate the mean of rssi and throughput map cells and store in 2 2d double arrays to create the heatmap
    PercentCalculator perCal = new PercentCalculator(minLon, maxLon, minLat, maxLat, cellRows, cellCols);
    rssiArray = perCal.calculate(true, "CSV_DATA/all_vehicles.csv");
    throughputArray = perCal.calculate(false, "CSV_DATA/all_vehicles.csv");

    //get the cell latitude and longitude bounds to locate the android terminals
    cellMinLonArray = perCal.getCellMinLonArray();
    cellMaxLonArray = perCal.getCellMaxLonArray();
    cellMinLatArray = perCal.getCellMinLatArray();
    cellMaxLatArray = perCal.getCellMaxLatArray();

    //create the legend
    LegendPainter lp = new LegendPainter(275, 140);
    BufferedImage legend = lp.createLegend();

    //create the RSSI and Throughput heatmaps
    double[][] rssiPercentArray = new double[cellRows][cellCols];
    double[][] throughputPercentArray = new double[cellRows][cellCols];
    for(int i = 0; i < cellRows; ++ i)
    {
      for(int j = 0; j < cellCols; ++j)
      {
        rssiPercentArray[i][j] = rssiArray[i][j];
        throughputPercentArray[i][j] = throughputArray[i][j];
      }
    }
    GridPainter gpRSSI = new GridPainter("Simulation Data/Map.png", rssiPercentArray, cellRows, cellCols, true);
    GridPainter gpThroughput = new GridPainter("Simulation Data/Map.png", throughputPercentArray, cellRows, cellCols, false);
    BufferedImage heatmapRSSI = gpRSSI.createHeatmap();
    BufferedImage heatmapThroughput = gpThroughput.createHeatmap();

    //concatenate the heatmaps with the legend
    BufferedImage rImg = new BufferedImage(gpRSSI.getW() + legend.getWidth(),
                                            gpRSSI.getH(), BufferedImage.TYPE_INT_RGB);
    BufferedImage tImg = new BufferedImage(gpThroughput.getW() + legend.getWidth(),
                                            gpThroughput.getH(), BufferedImage.TYPE_INT_RGB);
    Graphics2D rG2D = rImg.createGraphics();
    rG2D.drawImage(heatmapRSSI, 0, 0, null);
    rG2D.drawImage(legend, heatmapRSSI.getWidth() + 1, 0, null);
    try
    {
      if(ImageIO.write(rImg, "png", new File("heatmaps_data/RSSI_heatmap.png")))
        System.out.println("RSSI heatmap : heatmaps_data/RSSI_heatmap.png exported");
    }
    catch(IOException e)
    {
      e.printStackTrace();
    }
    rG2D.dispose();
    Graphics2D tG2D = tImg.createGraphics();
    tG2D.drawImage(heatmapThroughput, 0, 0, null);
    tG2D.drawImage(legend, heatmapThroughput.getWidth() + 1, 0, null);
    try
    {
      if(ImageIO.write(tImg, "png", new File("heatmaps_data/Throughput_heatmap.png")))
        System.out.println("Throughput heatmap : heatmaps_data/Throughput_heatmap.png exported");
    }
    catch(IOException e)
    {
      e.printStackTrace();
    }
    tG2D.dispose();

    //create 2 handlers for both topics with bidirectional communication
    topicHandler26 = new MQTTclientHandler("topic26", 2, "tcp://127.0.0.1:1883", "edgeServer26");
    topicHandler27 = new MQTTclientHandler("topic27", 2, "tcp://127.0.0.1:1883", "edgeServer27");

    //subscribe to the topics
    topicHandler26.MQTTsubscribe();
    topicHandler27.MQTTsubscribe();

    //establish connection with mySQL database
    final String connection = "jdbc:mysql://127.0.0.1:3306/cars2019DB";
    System.out.println("Establishing connection to database : " + connection);
    Properties p = new Properties();
    p.put("user", "root");
    p.put("password", "sisixaxa");
    final Connection sqlHandler = DriverManager.getConnection(connection, p);
    System.out.println("Connection established");

    //create 4 arrays to hold predicted values from each terminal old and new
    double[] predictedVals26Old = new double[5];
    double[] predictedVals27Old = new double[5];
    double[] predictedVals26New = new double[5];
    double[] predictedVals27New = new double[5];

    while(true)//read published data from both android terminals
    {
      if(!topicHandler26.getRdy())
        Thread.sleep(0);
      else//get the message from topic26
      {
        String rcvdMsg = topicHandler26.getMsg();
        System.out.println("Received message from topic26 = " + rcvdMsg);
        if(rcvdMsg.contains("timestep, id, lat, long, angle, speed, RSSI, throughput"))//header of csv
        {
          clearDatabase(26, sqlHandler);
          firstMsg26 = false;
        }
        else if(rcvdMsg.contains("END OF DATA"))//Data transmission ended
        {
          System.out.println("Data transmission from android terminal 26 completed");
          firstMsg26 = false;
          calculateError(26, sqlHandler);
        }
        else//handle it
          handleMessage(rcvdMsg, 26, predictedVals26Old, predictedVals26New, sqlHandler);
      }

      if(!topicHandler27.getRdy())
        Thread.sleep(0);
      else//get the message from topic27
      {
        String rcvdMsg = topicHandler27.getMsg();
        System.out.println("Received message from topic27 = " + rcvdMsg);
        if(rcvdMsg.contains("timestep, id, lat, long, angle, speed, RSSI, throughput"))//header of csv
        {
          clearDatabase(27, sqlHandler);
          firstMsg27 = false;
        }
        else if(rcvdMsg.contains("END OF DATA"))//Data transmission ended
        {
          System.out.println("Data transmission from android terminal 27 completed");
          firstMsg27 = false;
          calculateError(27, sqlHandler);
        }
        else//handle it
          handleMessage(rcvdMsg, 27, predictedVals27Old, predictedVals27New, sqlHandler);
      }
    }
  }
}