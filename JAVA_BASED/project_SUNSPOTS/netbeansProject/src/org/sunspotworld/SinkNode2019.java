/*******************************************************************************
 * To change this license header, choose License Headers in Project Properties.*
 * To change this template file, choose Tools | Templates                      *
 * and open the template in the editor.                                        *
 *******************************************************************************/
package org.sunspotworld;
//necessary utilities
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;
import java.io.*;
import javax.microedition.io.*;
import com.sun.spot.io.j2me.radiostream.*;
import com.sun.spot.peripheral.NoRouteException;

/**************************************************
 * @author     Vasileios Mavrommatis 1115201200106*
 * @revision   30/8/2019                           *
 **************************************************/
//MIDlet SinkNode2019 implementation class
public class SinkNode2019 extends MIDlet
{
    public final boolean startDataGathering = true;
    public boolean mergeFlag = true;
    //Main MIDlet function
    protected void startApp() throws MIDletStateChangeException 
    {
        try {
            System.out.println("SinkNode2019 MIDlet STARTING ...");
            RadiostreamConnection conn =
                (RadiostreamConnection)Connector.open("radiostream://0A00.020F.0000.1003:100");
            DataInputStream dis = conn.openDataInputStream();
            double[] inputMergedStreams = new double[8];
            double[] inputBothStreams = new double[16];
            try {
               while(startDataGathering == true)
               {
                   mergeFlag = dis.readBoolean();
                   if(mergeFlag == true)
                   {
                       System.out.println("RECEIVING MERGED STREAM FROM AGGREGATOR ...");
                       for(int i = 0; i < 8; ++i)
                           inputMergedStreams[i] = dis.readDouble();
                       System.out.println("PRINTING RECEIVED DATA STREAM...");
                       for(int i = 0; i < 8; ++i)
                           System.out.println(inputMergedStreams[i]);
                   }
                   else
                   {
                       System.out.println("RECEIVING BOTH STREAMS FROM AGGREGATOR ...");
                       for(int i = 0; i < 16; ++i)
                           inputBothStreams[i] = dis.readDouble();
                       System.out.println("PRINTING RECEIVED DATA STREAM1 ...");
                       for(int i = 0; i < 8; ++i)
                           System.out.println(inputBothStreams[i]);
                       System.out.println("PRINTING RECEIVED DATA STREAM2 ...");
                       for(int i = 8; i < 16; ++i)
                           System.out.println(inputBothStreams[i]);
                   }
               } 
            } catch (NoRouteException e) {
                System.out.println ("NO ROUTE TO SENSORS ...");
            } finally {
                dis.close();
                conn.close();
            }
            System.out.println("SinkNode2019 MIDlet EXITING ...");
        } catch (IOException ex) {
        }
    }
    
    protected void pauseApp() 
    {
        // This will never be called by the Squawk VM
    }
    
    protected void destroyApp(boolean arg0) throws MIDletStateChangeException 
    {
        // Only called if startApp throws any exception other than MIDletStateChangeException
    }
}
