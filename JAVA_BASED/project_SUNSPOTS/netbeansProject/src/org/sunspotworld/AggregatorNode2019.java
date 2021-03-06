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
//MIDlet AggregatorNode2019 implementation class
public class AggregatorNode2019 extends MIDlet
{   //des ti paizei me ta threads sto google kai ftiakse ta 2 thread
    public final double PI = Math.PI;
    public boolean controlPhase = true;
    public boolean thresholdFlag = true;
    public boolean controlFlag = true;
    public final boolean startDataAggregation = true;
    public final double THRESHOLD = 2.0;//ADJUSTABLE ALGORITHM STRICTNESS
    
    //logarithm base 2 function 
    public int log2n(int n)//integer rounding @ floor
    {
        return n > 1 ? 1 + log2n(n / 2) : 0;
    }
    //helper class to be used as input for FFT
    public class Complex
    {
        public double re;
        public double im;
        public Complex()
        {
            this(0.0, 0.0);
        }
        public Complex(double r, double i) 
        {
            re = r;
            im = i;
        }
        public Complex add(Complex b)
        {
            return new Complex(this.re + b.re, this.im + b.im);
        }
        public Complex sub(Complex b)
        {
            return new Complex(this.re - b.re, this.im - b.im);
        }
        public Complex mult(Complex b)
        {
            return new Complex(this.re * b.re - this.im * b.im,
            this.re * b.im + this.im * b.re);
        }
    }
    //fft helper function
    public int bitReverse(int n, int bits) 
    {
        int reversedN = n;
        int count = bits - 1;
        n >>= 1;
        while (n > 0)
        {
            reversedN = (reversedN << 1) | (n & 1);
            count--;
            n >>= 1;
        }
        return ((reversedN << count) & ((1 << bits) - 1));
    }
    //Fast fourier transform
    public void fft(Complex[] buffer) 
    {
        int bits = (int) (log2n(buffer.length) / log2n(2));
        //System.out.println("bits calculated : " + bits);
        for (int j = 1; j < buffer.length / 2; ++j) 
        {
            
            int swapPos = bitReverse(j, bits);
            Complex temp = buffer[j];
            buffer[j] = buffer[swapPos];
            buffer[swapPos] = temp;
        }
        for (int N = 2; N <= buffer.length; N <<= 1) 
        {
            for (int i = 0; i < buffer.length; i += N) 
            {
                for (int k = 0; k < N / 2; ++k) 
                {
                    int evenIndex = i + k;
                    int oddIndex = i + k + (N / 2);
                    Complex even = buffer[evenIndex];
                    Complex odd = buffer[oddIndex];
                    double term = (-2 * PI * k) / (double) N;
                    Complex exp = (new Complex(Math.cos(term), Math.sin(term)).mult(odd));
                    buffer[evenIndex] = even.add(exp);
                    buffer[oddIndex] = even.sub(exp);
                }
            }
        }
    }
    //quicksort helper function
    public static int partition(Complex[] array, int begin, int end)
    {
        int pivot = end;
        int counter = begin;
        for(int i = begin; i < end; ++i)
        {
            if(Math.sqrt(array[i].re * array[i].re + array[i].im * array[i].im)< Math.sqrt(array[pivot].re * array[pivot].re + array[pivot].im * array[pivot].im))
            {
                double tempRe = array[counter].re;
                double tempIm = array[counter].im;
                array[counter].re = array[i].re;
                array[counter].im = array[i].im;
                array[i].re = tempRe;
                array[i].im = tempIm;
                counter++;
            }
        }
        double tempRe = array[pivot].re;
        array[pivot].re = array[counter].re;
        array[counter].re = tempRe;
        double tempIm = array[pivot].im;
        array[pivot].im = array[counter].im;
        array[counter].im = tempIm;
        return counter;
    }
    //quicksort to get the 4 lowest spectrum values
    public static void quickSort(Complex[] array, int begin, int end)
    {
        if(end <= begin)
            return;
        int pivot = partition(array, begin, end);
        quickSort(array, begin, pivot-1);
        quickSort(array, pivot+1, end);
    }
    
    //Main MIDlet function
    protected void startApp() throws MIDletStateChangeException 
    {
        try {
            System.out.println("AggregatorNode2019 MIDlet STARTING ...");
            RadiostreamConnection conn1 =
                    (RadiostreamConnection)Connector.open("radiostream://0A00.020F.0000.1001:100");
            RadiostreamConnection conn2 =
                    (RadiostreamConnection)Connector.open("radiostream://0A00.020F.0000.1002:100");
            RadiostreamConnection conn3 =
                    (RadiostreamConnection)Connector.open("radiostream://0A00.020F.0000.1004:100");
            DataInputStream dis1 = conn1.openDataInputStream();
            DataInputStream dis2 = conn2.openDataInputStream();
            DataOutputStream sinkDos = conn3.openDataOutputStream();
            double[] input1 = new double[8];
            double[] input2 = new double[8];
            try {
                while(startDataAggregation == true)
                {
                    while(controlPhase == true)//get 8 doubles from each sensor, and run fft
                    {
                        System.out.println("INITIATING CONTROL PHASE ...");
                        for(int i = 0; i < 8; ++i)
                        {
                            input1[i] = dis1.readDouble();
                            input2[i] = dis2.readDouble();
                        }
                        System.out.println("INITIATING FFT TRANSFORMATION ON SAMPLES ...");
                        Complex[] cinput1 = new Complex[8];
                        for (int i = 0; i < 8; ++i)
                            cinput1[i] = new Complex(input1[i], 0.0);
                        /*
                        System.out.println("INPUT 1 VALUES BEFORE FFT ...");
                        for (int i = 0; i < 8; ++i)
                            System.out.println("(" + cinput1[i].re + " , " + cinput1[i].im + " j)");
                        */
                        Complex[] cinput2 = new Complex[8];
                        for (int i = 0; i < 8; ++i)
                            cinput2[i] = new Complex(input2[i], 0.0);
                        /*
                        System.out.println("INPUT 2 VALUES BEFORE FFT ...");
                        for (int i = 0; i < 8; ++i)
                            System.out.println("(" + cinput2[i].re + " , " + cinput2[i].im + " j)");
                        */
                        fft(cinput1);//run fft on input set1
                        /*
                        System.out.println("OUTPUT 1 VALUES AFTER FFT ...");
                        for (int i = 0; i < 8; ++i)
                            System.out.println("(" + cinput1[i].re + " , " + cinput1[i].im + " j)"); 
                        */
                        fft(cinput2);//run fft on input set2
                        /*
                        System.out.println("OUTPUT 2 VALUES AFTER FFT ...");
                        for (int i = 0; i < 8; ++i)
                            System.out.println("(" + cinput2[i].re + " , " + cinput2[i].im + " j)");
                        */
                        System.out.println("FFT TRANSFORMATION COMPLETED ...");
                        
                        quickSort(cinput1, 0, 7);
                        /*
                        System.out.println("OUTPUT 1 VALUES AFTER SORTING ...");
                        for (int i = 0; i < 8; ++i)
                            System.out.println("(" + cinput1[i].re + " , " + cinput1[i].im + " j)");
                        */
                        quickSort(cinput2, 0, 7);
                        /*
                        System.out.println("OUTPUT 2 VALUES AFTER SORTING ...");
                        for (int i = 0; i < 8; ++i)
                            System.out.println("(" + cinput2[i].re + " , " + cinput2[i].im + " j)");
                        */
                        System.out.println("CALCULATING EUCLIDEAN DISTANCE BETWEEN STREAMS...");
                        double euDist;
                        double deltaRe;
                        double deltaIm;
                        controlFlag = true;
                        thresholdFlag = true;
                        for(int i = 0; i < 4; ++i)//use the 4 lowest spectrum values
                        {
                            deltaRe = cinput1[i].re - cinput2[i].re;
                            deltaIm = cinput1[i].im - cinput2[i].im;
                            euDist = Math.sqrt((deltaRe * deltaRe) + (deltaIm * deltaIm));
                            System.out.println("EUCLIDEAN DISTANCE[" + i + "] = " + euDist);
                            if(controlFlag == true)
                            {
                                if(euDist > THRESHOLD)//RE ENTER CONTROL PHASE
                                {
                                    controlFlag = false;
                                    thresholdFlag = false;
                                    System.out.println("THRESHOLD EXCEEDED ...");
                                }
                            }
                        }
                        if(thresholdFlag == true)//streams got synced
                        {
                            System.out.println("SENDING MERGED STREAM TO SINK ...");
                            sinkDos.writeBoolean(true);
                            sinkDos.flush();
                            for(int i = 0; i < 8; ++i)
                            {
                                sinkDos.writeDouble((input1[i] + input2[i]) / 2.0);
                                sinkDos.flush();
                            }
                            controlPhase = false;
                        }
                        else
                        {
                            System.out.println("SENDING BOTH STREAMS TO SINK ...");
                            sinkDos.writeBoolean(false);
                            sinkDos.flush();
                            for(int i = 0; i < 8; ++i)
                            {
                                sinkDos.writeDouble(input1[i]);
                                sinkDos.flush();
                            }
                            for(int i = 0; i < 8; ++i)
                            {
                                sinkDos.writeDouble(input2[i]);
                                sinkDos.flush();
                            }
                        }
                        System.out.println("CONTROL PHASE COMPLETED ...");
                    }
                    System.out.println("INITIATING EXECUTION PHASE ...");
                    for(int i = 0; i < 8; ++i)
                    {
                        for(int j = 0; j < 8; ++j)
                        {
                            //comment in the 2 lines below to display input set values
                            input1[j] = dis1.readDouble();
                            //System.out.println(input1[j] + " RECEIVED FROM S1");
                            input2[j] = dis2.readDouble();
                            //System.out.println(input2[j] + " RECEIVED FROM S2");
                        }
                        //totalDataR += 16;
                        System.out.println("SENDING MERGED STREAM TO SINK ...");
                        sinkDos.writeBoolean(true);
                        sinkDos.flush();
                        for(int j = 0; j < 8; ++j)
                        {
                            sinkDos.writeDouble((input1[j] + input2[j]) / 2.0);
                            sinkDos.flush();
                        }
                        System.out.println("EXECUTION_PART[" + i + "] COMPLETED ...");
                    }
                    System.out.println("EXECUTION PHASE COMPLETED ...");
                    controlPhase = true;
                }
            } catch (NoRouteException e) {
                System.out.println ("NO ROUTE TO SENSORS ...");
            } finally {
                dis1.close();
                conn1.close();
                dis2.close();
                conn2.close();
                sinkDos.close();
                conn3.close();
            }
            System.out.println("AggregatorNode2019 MIDlet EXITING ...");
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
