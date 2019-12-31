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
import java.util.*;
import javax.microedition.io.*;
import com.sun.spot.io.j2me.radiostream.*;
import com.sun.spot.peripheral.NoRouteException;
/**************************************************
 * @author     Vasileios Mavrommatis 1115201200106*
 * @revision   30/8/2019                           *
 **************************************************/
//MIDlet SensorNode2019 implementation class
public class SensorNode2019 extends MIDlet
{
    public final double PI = Math.PI;
    public final double E = Math.E;
    public double spare;
    public boolean isSpareReady = false;
    public final boolean startDataCreation = true;
    Random randomD = new Random(System.currentTimeMillis());//init with time seed
    //this function implements the Marsaglia Pollar Gausian Noise Generator
    //as an alterantive (cartessian cords)
    public double gngMarsagliaPollar(double mean, double stdDev)
    {
        if (isSpareReady)
        {
            isSpareReady = false;
            return spare * stdDev + mean;
        } 
        else
        {
            double u, v, s;
            do 
            {
                u = randomD.nextDouble() * 2.0 - 1.0;
                v = randomD.nextDouble() * 2.0 - 1.0;
                s = u * u + v * v;
            }while (s >= 1 || s == 0);
            double mul = Math.sqrt(-2.0 * lnE(s) / s);
            spare = v * mul;
            isSpareReady = true;
            return mean + stdDev * u * mul;
        }
    }
    //this function implements the Box Muller Gaussian Noise Generator
    //used currently (polar cords)
    public double gngBoxMuller(double mu, double sigma)
    {
        double twoPI = PI * 2;
        double epsilon = 2.22507e-308;//smallest double positive value
        double z0, z1;
        double u1, u2;
        int RAND_MAX = 2147483647;
        do
        {
           u1 = randomD.nextInt(RAND_MAX) * (1.0 / RAND_MAX);
	   u2 = randomD.nextInt(RAND_MAX) * (1.0 / RAND_MAX);
        } while(u1 <= epsilon);
	z0 = Math.sqrt(-2.0 * lnE(u1)) * Math.cos(twoPI * u2);
	z1 = Math.sqrt(-2.0 * lnE(u1)) * Math.sin(twoPI * u2);
        return z0 * sigma + mu;
    }
    //this function calculates the base 2 logarithm
    public int log2n(int n)//integer rounding @ floor
    {
        return n > 1 ? 1 + log2n(n / 2) : 0;
    }
    //this function calculates power y of base x
    public double power(double x, int y) //O(logN) optimised
    { 
        double temp; 
        if( y == 0) 
            return 1; 
        temp = power(x, y/2);  
          
        if (y%2 == 0) 
            return temp*temp; 
        else
        { 
            if(y > 0) 
                return x * temp * temp; 
            else
                return (temp * temp) / x; 
        } 
    }  
    
    public double lnE(double x)
    {//Taylor series implementation of natural logarithm using 100 1st terms sum
     //works OK for x in (0.0 , 1.0]
        double result = 0.0;
        if(x == 1.0)
            return 0.0;
        if(x == E)
            return 1.0;
        for(int k = 1; k < 100; ++k)
            result += power(-1, k - 1) * power(x - 1, k) / k;
        return result;
    }
    //Main MIDlet function
    protected void startApp() throws MIDletStateChangeException 
    {
        try {
            System.out.println("SensorNode2019 MIDlet STARTING ...");
            RadiostreamConnection conn =
                    (RadiostreamConnection)Connector.open("radiostream://0A00.020F.0000.1003:100");
            DataOutputStream dos = conn.openDataOutputStream();
            try {
                System.out.println("USING FIXED MEAN : 0.0");
                System.out.println("USING FIXED STD : 1.0");
                double[] inputBoxM = new double[8];
                System.out.println("INITIATING DATA MINING ...");
                while(startDataCreation == true)
                {
                    for(int i = 0; i < 8; ++i)
                    {
                        inputBoxM[i] = gngBoxMuller(0.0, 1.0);//fixed mean and std
                        System.out.println("DATA_PART[" + i + "] = " + inputBoxM[i]);
                        try//gather data every second
                        {
                            Thread.sleep(1000);
                        }
                        catch(InterruptedException ex)
                        {
                            Thread.currentThread().interrupt();
                        }
                    }
                    System.out.println("SENDING DATA TO AGGREGATOR ...");
                    for(int i = 0; i < 8; ++i)
                    {
                        dos.writeDouble(inputBoxM[i]);
                        dos.flush();
                    }
                    System.out.println("DATA SET SUCCESSFULLY SENT ...");
                }
            } catch (NoRouteException e) {
                System.out.println ("NO ROUTE TO AGGREGATOR ...");
            } finally {
                dos.close();
                conn.close();
            }
            System.out.println("SensorNode2019 MIDlet EXITING ...");
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
