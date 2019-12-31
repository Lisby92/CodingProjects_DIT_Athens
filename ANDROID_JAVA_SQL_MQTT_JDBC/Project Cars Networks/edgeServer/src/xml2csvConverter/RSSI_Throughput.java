package xml2csvConverter;

import java.util.Random;

class RSSI_Throughput
{
  //constructor
  RSSI_Throughput()
  {
    //System.out.println("An RSSI_Throughput object has been created!");
  }
  //this method generates random values using normal distribution with mean 60 and std 20
  //in the range [20-100]
  String createValues(final String s)
  {
    String creation = s;
    //generate random RSSI
    Random r = new Random();
    double result = r.nextGaussian() * 20 + 60;// mean 60, dev 20
    while(result < 20.0 || result > 100.0)
      result = r.nextGaussian() * 20 + 60;
    //add it to the string
    creation += Double.toString(result);
    creation += ", ";
    //calculate throughput and add it
    double throughput = (result / 100) * 50;
    creation += Double.toString(throughput);
    return creation;
  }
}