package xml2csvConverter;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.util.LinkedList;

public class CSVwriter
{
  //constructor
  public CSVwriter()
  {
    //System.out.println("CSVwriter object created successfully!");
  }
  //this method writes out the CSV file
  public void writeCSV(LinkedList<String> list, String destination) throws FileNotFoundException
  {
    System.out.println("Creating CSV file : " + destination);
    PrintWriter pw = new PrintWriter(new FileOutputStream(destination));
    try
    {
      for (String s : list)
        pw.println(s);
    }
    finally
    {
      pw.close();
    }
    System.out.println("File created");
  }
}
