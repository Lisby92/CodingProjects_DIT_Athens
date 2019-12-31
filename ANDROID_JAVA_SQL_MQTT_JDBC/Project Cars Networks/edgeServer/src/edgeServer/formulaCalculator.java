package edgeServer;

public class formulaCalculator
{
  private String data;
  private double horizon;
  private double timestep;
  private double latEnd;
  private double lonEnd;
  private double latStart;
  private double lonStart;
  private double angle;
  private double speed;
  private double RSSI;
  private double throughput;
  private final double earthRadius = 6.371 * Math.pow(10,6);

  //constructor
  public formulaCalculator(String data, double horizon)
  {
    //System.out.println("Just created a formula calculator object");
    this.data = data;
    this.horizon = horizon;
  }

  //this method predicts the geographical position of the android terminal
  //after processing the received data from it
  public void calculatePosition()
  {
    extractData();
    degrees2Radians();
    applyFormula();
    radians2Degrees();
  }

  //this method extracts the needed data from the received message(CSV line)
  private void extractData()
  {
    String temp = data;
    //extract the timestep
    int pos = temp.indexOf(',');
    String sTimestep = temp.substring(0, pos);
    timestep = Double.parseDouble(sTimestep);
    //System.out.println("Timestep extracted : " + timestep);
    temp = temp.substring(pos+2);
    //skip the vehicle id and space char
    pos = temp.indexOf(',');
    temp = temp.substring(pos+2);
    //extract the latitude
    pos = temp.indexOf(',');
    String sLat = temp.substring(0, pos);
    latStart = Double.parseDouble(sLat);
    //System.out.println("Latitude extracted : " + latStart);
    temp = temp.substring(pos+2);
    //extract the longitude
    pos = temp.indexOf(',');
    String sLon = temp.substring(0, pos);
    lonStart = Double.parseDouble(sLon);
    //System.out.println("Longitude extracted : " + lonStart);
    temp = temp.substring(pos+2);
    //extract the angle
    pos = temp.indexOf(',');
    String sAngle = temp.substring(0, pos);
    angle = Double.parseDouble(sAngle);
    //System.out.println("Angle extracted : " + angle);
    temp = temp.substring(pos+2);
    //extract the speed
    pos = temp.indexOf(',');
    String sSpeed = temp.substring(0, pos);
    speed = Double.parseDouble(sSpeed);
    //System.out.println("Speed extracted : " + speed);
    temp = temp.substring(pos+2);
    //extract the RSSI
    pos = temp.indexOf(',');
    String sRSSI = temp.substring(0, pos);
    RSSI = Double.parseDouble(sRSSI);
    //System.out.println("RSSI extracted : " + RSSI);
    temp = temp.substring(pos+2);
    //extract the throughput
    throughput = Double.parseDouble(temp);
    //System.out.println("Throughput extracted : " + throughput);
  }

  //this method converts the start position and angle values from degrees to radians
  private void degrees2Radians()
  {
    latStart = Math.toRadians(latStart);
    lonStart = Math.toRadians(lonStart);
    angle = Math.toRadians(angle);
  }

  //this method converts the start and end position values from radians to degrees
  private void radians2Degrees()
  {
    latEnd = Math.toDegrees(latEnd);
    lonEnd = Math.toDegrees(lonEnd);
    latStart = Math.toDegrees(latStart);
    lonStart = Math.toDegrees(lonStart);
  }

  //this method applies the formula of location prediction
  private void applyFormula()
  {
    double delta = (horizon * speed) / earthRadius;
    latEnd = Math.asin(Math.sin(latStart) * Math.cos(delta) + Math.cos(latStart) * Math.sin(delta) * Math.cos(angle));
    lonEnd = lonStart + Math.atan2(Math.sin(angle) * Math.sin(delta) * Math.cos(latStart),
                                   Math.cos(delta) - Math.sin(latStart) * Math.sin(latEnd));
  }

  //get android terminal's predicted latitude
  public double getPredictedLat()
  {
    return latEnd;
  }

  //get android terminal's predicted longitude
  public double getPredictedLon()
  {
    return lonEnd;
  }

  //get android terminal's predicted timestep based on prediction's horizon
  public double getPredictedStep()
  {
    return (timestep + horizon);
  }

  //get android terminal's RSSI
  public double getThroughput()
  {
    return throughput;
  }

  //get android terminal's throughput
  public double getRSSI()
  {
    return RSSI;
  }

  public double getCurrentLat()
  {
    return latStart;
  }

  public double getCurrentLon()
  {
    return lonStart;
  }

  public double getCurrentStep()
  {
    return timestep;
  }
}
