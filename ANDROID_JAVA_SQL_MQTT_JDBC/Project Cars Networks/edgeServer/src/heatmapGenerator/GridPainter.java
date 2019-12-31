package heatmapGenerator;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class GridPainter
{
  private int height;
  private int width;
  private BufferedImage img;
  private double[][] valArray;
  private int cellRows;
  private int cellCols;
  private boolean type;
  //constructor
  public GridPainter(final String filename, final double[][] values,
                     final int cellRows, final int cellCols, final boolean type)
  {
    valArray = values;
    this.cellRows = cellRows;
    this.cellCols = cellCols;
    this.type = type;
    //System.out.println("Just created a GridPainter2 object");
    try
    {
      img = ImageIO.read(new File(filename));
    }
    catch (IOException e)
    {
      e.printStackTrace();
    }
    if(img != null)
    {
      height = img.getHeight();
      width = img.getWidth();
    }
    else
    {
      System.out.println("Invalid image");
      height = 0;
      width = 0;
    }
  }
  //this method returns the input image's width
  public int getW()
  {
    return width;
  }
  //this method returns the input image's height
  public int getH()
  {
    return height;
  }
  //this method creates a Heatmap and returns it as a BufferedImage object
  public BufferedImage createHeatmap()
  {
    double minVal;
    double maxVal;
    if(type)//RSSI
    {
      minVal = 20.0;
      maxVal = 100.0;
    }
    else//Throughput
    {
      minVal = 10.0;
      maxVal = 50.0;
    }
    System.out.println("Creating heatmap grid");
    for(int i = 0; i < cellRows; ++i)
      for(int j = 0; j < cellCols; ++j)
        if(valArray[i][j] != -1.0)//skip empty cells
          valArray[i][j] = ((valArray[i][j] - minVal) / (maxVal - minVal)) * 100;//calculate the percentage

    double cellWidth = (double) width / (double) cellCols;
    double cellHeight = (double) height / (double) cellRows;
    Graphics2D g2d = img.createGraphics();
    MultiGradient mg = new MultiGradient(100);
    //69 color 4 section multi gradient for optimal visualisation of the heatmap
    Color[] myPallete = mg.createMultiGradient(new Color[]{Color.GREEN, Color.YELLOW, Color.ORANGE, Color.RED},
            276);
    double minPercentage = 101.0;
    double maxPercentage = -1.0;
    for(int i = 0; i < cellRows; ++i)//find the max and min percentages to adjust the painting
    {
      for(int j = 0; j < cellCols; ++j)
      {
        if(valArray[i][j] >= maxPercentage)
          maxPercentage = valArray[i][j];
        if(valArray[i][j] <= minPercentage && valArray[i][j] != -1.0)
          minPercentage = valArray[i][j];
      }
    }
    //System.out.println("minpercentage = :" + minPercentage + "\tmaxpercentage = :" + maxPercentage);
    if(img != null)
    {
      int i = 0;
      int j;
      for(double y = 0.0; y < (double) img.getHeight(); y += cellHeight)
      {
        j = 0;
        for(double x = 0.0; x < (double) img.getWidth(); x += cellWidth)
        {
          Rectangle2D.Double rect = new Rectangle2D.Double(x, y, cellWidth, cellHeight);
          Color myColor = new Color(0,0,0,10);
          g2d.setColor(myColor);
          g2d.fill(rect);
          g2d.draw(rect);
          if(valArray[i][j] != -1.0)//non empty cell
          {
            Color curColor = myPallete[(int)(275 * (valArray[i][j] - minPercentage) / (maxPercentage - minPercentage))];
            g2d.setColor(curColor);
            g2d.fill(rect);
          }
          ++j;
        }
        ++i;
      }
    }
    g2d.dispose();
    System.out.println("Heatmap grid completed");
    return img;
  }
}
