package edgeServer;

public class distanceCalculator
{
  //constructor
  public distanceCalculator()
  {
    //System.out.println("Just created a distanceCalculator object");
  }

  //this method calculates the distance between 2 points given their latitude and longitude values
  public double calcDist(final double lat1, final double lon1, final double lat2, final double lon2)
  {
    if((lat1 == lat2) && (lon1 == lon2))
    {
      return 0.0;
    }
    else
    {
      final double theta = lon1 - lon2;
      double dist = (Math.sin(Math.toRadians(lat1)) * Math.sin(Math.toRadians(lat2))) + (Math.cos(Math.toRadians(lat1)) * Math.cos(Math.toRadians(lat2)) * Math.cos(Math.toRadians(theta)));
      dist = Math.acos(dist);
      dist = Math.toDegrees(dist);
      dist *=  (60 * 1.1515);
      dist *= 1.609344;//Km transform
      dist *= 1000.0;//M transform
      return dist;
    }
  }
}
