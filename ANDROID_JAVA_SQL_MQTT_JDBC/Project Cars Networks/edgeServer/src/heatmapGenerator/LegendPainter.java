package heatmapGenerator;

import java.awt.*;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;

public class LegendPainter
{
  private int height;
  private int width;
  //constructor
  public LegendPainter(final int h, final int w)
  {
    //System.out.println("Just created a LegendPainter object");
    height = h;
    width = w;
  }
  //this method creates a Legend and returns it as a BufferedImage object
  public BufferedImage createLegend()
  {
    System.out.println("Creating max-min heatmap legend");
    BufferedImage legend = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
    Graphics2D g2Legend = legend.createGraphics();
    MultiGradient mg = new MultiGradient(100);
    Color[] myPallete = mg.createMultiGradient(new Color[]{Color.GREEN, Color.YELLOW, Color.ORANGE, Color.RED},
            275);
    for(int i = 0; i < 275; ++i)
    {
      Rectangle2D.Double rect = new Rectangle2D.Double(0, i, 140, 1);
      g2Legend.draw(rect);
      g2Legend.setColor(myPallete[i]);
      g2Legend.fill(rect);
    }
    g2Legend.setColor(Color.BLACK);
    g2Legend.setFont(new Font("TimesRoman", Font.PLAIN, 20));
    g2Legend.drawString("MAX", 45, 20);
    g2Legend.drawString("MIN", 45, 270);
    g2Legend.dispose();
    System.out.println("Legend created");
    return legend;
  }
}
