package heatmapGenerator;

import java.awt.*;

public class MultiGradient
{
  private int transparency;
  //constructor
  public MultiGradient(final int transparency)
  {
    this.transparency = transparency;
    //System.out.println("Just created a MultGradient object");
  }
  //this method creates a gradient from the desired colors and returns an array of Color objects
  private Color[] createGradient(final Color startColor, final Color endColor, final int numSteps)
  { //get startColor RGB properties
    int r1 = startColor.getRed();
    int g1 = startColor.getGreen();
    int b1 = startColor.getBlue();
    //get endColor RGB properties
    int r2 = endColor.getRed();
    int g2 = endColor.getGreen();
    int b2 = endColor.getBlue();

    Color[] gradient = new Color[numSteps];
    double iNorm;
    for (int i = 0; i < numSteps; ++i)
    {
      iNorm = i / (double)numSteps; //a normalized [0:1] variable
      int newR = (int) (r1 + iNorm * (r2 - r1));
      int newG = (int) (g1 + iNorm * (g2 - g1));
      int newB = (int) (b1 + iNorm * (b2 - b1));
      gradient[i] = new Color(newR, newG, newB, transparency);//create and store the color
      //can also use the same principle for transparency gradient by getting the transparency value of each color
    }
    return gradient;
  }
  //this method creates a linear multi gradient of at least 2 colors
  //with equal spacing between colors and made up of n sections
  public Color[] createMultiGradient(final Color[] colors, final int numSteps)
  {
    int numSections = colors.length - 1;
    int gradientIndex = 0;
    Color[] gradient = new Color[numSteps];
    Color[] temp;

    if (numSections <= 0)
      throw new IllegalArgumentException("You must pass in at least 2 colors in the array!");
    for (int section = 0; section < numSections; ++section)
    {
      //divide the gradient into (n - 1) sections, and do a regular gradient for each
      temp = createGradient(colors[section], colors[section+1], numSteps / numSections);
      for(int i = 0; i < temp.length; ++i)
      {
        //copy the sub-gradient into the overall gradient
        gradient[gradientIndex++] = temp[i];
      }
    }
    if(gradientIndex < numSteps)
    {
      //The rounding failed, and there is at least
      // one unfilled slot in the gradient[] array where we copy the final color
      while (gradientIndex < numSteps)
      {
        gradient[gradientIndex] = colors[colors.length - 1];
        ++gradientIndex;
      }
    }
    return gradient;
  }
}
