package xml2csvConverter;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.LinkedList;

public class XMLParser
{
  //constructor
  public XMLParser()
  {
    //System.out.println("Just created a XMLParser object!");
  }
  //this function parses the xml file into a linked list with oldschool string manipulation
  //and returns it to the caller function(currently main)
  public LinkedList<String> parse(String filename)//static removed after public
  {
    LinkedList<String> list = new LinkedList<String>();
    String header = "timestep, id, lat, long, angle, speed, RSSI, throughput";
    list.add(header);//add the fields @ list head
    System.out.println("Parsing XML file : " + filename);
    try
    {
      BufferedReader reader = new BufferedReader(new FileReader(filename));
      //skip the first 2 lines
      reader.readLine();
      reader.readLine();
      String line = reader.readLine();
      String timestep = "";
      String xLon;
      String yLat;
      String id;
      String angle;
      String speed;
      String xmlString;
      RSSI_Throughput generator = new RSSI_Throughput();
      String finalString;

      while(line != null)
      {
        if(line.contains("<timestep time="))//get the timestep
        {
          int sPos = line.indexOf('\"');
          int ePos = line.lastIndexOf('\"');
          timestep = line.substring(sPos+1, ePos);
        }
        if(line.contains("<vehicle id="))//get the vehicle data
        {
          //extract the id
          int pos = line.indexOf('\"');
          String temp = line.substring(pos+1);
          pos = temp.indexOf('\"');
          id = temp.substring(0, pos);
          temp = temp.substring(pos+1);
          //extract the longitude
          pos = temp.indexOf('\"');
          temp = temp.substring(pos+1);
          pos = temp.indexOf('\"');
          xLon = temp.substring(0, pos);
          temp = temp.substring(pos+1);
          //extract the latitude
          pos = temp.indexOf('\"');
          temp = temp.substring(pos+1);
          pos = temp.indexOf('\"');
          yLat = temp.substring(0, pos);
          temp = temp.substring(pos+1);
          //extract the angle
          pos = temp.indexOf('\"');
          temp = temp.substring(pos+1);
          pos = temp.indexOf('\"');
          angle = temp.substring(0, pos);
          temp = temp.substring(pos+1);
          //skip the type
          pos = temp.indexOf('\"');
          temp = temp.substring(pos+1);
          pos = temp.indexOf('\"');
          temp = temp.substring(pos+1);
          //extract the speed
          pos = temp.indexOf('\"');
          temp = temp.substring(pos+1);
          pos = temp.indexOf('\"');
          speed = temp.substring(0, pos);
          //create the string containing just the needed fields of the xml file
          xmlString = timestep + ", " + id + ", " + yLat + ", " + xLon + ", " + angle + ", " + speed + ", ";
          //add RSSI and throughput values
          finalString = generator.createValues(xmlString);
          list.add(finalString);
        }
        line = reader.readLine();
      }
      System.out.println("Parsing completed");
      reader.close();
    }
    catch(IOException e)
    {
      e.printStackTrace();
    }
    return list;
  }
}