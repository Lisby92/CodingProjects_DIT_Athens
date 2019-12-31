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
import java.util.*;
/**************************************************
 * @author     Vasileios Mavrommatis 1115201200106*
 * @revision   30/8/2019                           *
 **************************************************/
//MIDlet AggregatorNode2019 implementation class
public class ThreadAggregator extends MIDlet
{   //des ti paizei me ta threads sto google kai ftiakse ta 2 thread
    public final double PI = Math.PI;
    public final boolean startDataAggregation = true;
    public final double THRESHOLD = 2.0;//ADJUSTABLE ALGORITHM STRICTNESS
    public double[] input1 = new double[8];
    public double[] input2 = new double[8];
    public double[] output1 = new double[8];
    public double[] output2 = new double[8];
    public final int vectorSize = 16;
    public final Vector input = new Vector();
    //logarithm base 2 function 
    public static int log2n(int n)//integer rounding @ floor
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
    public static int bitReverse(int n, int bits) 
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
    
    class Collector implements Runnable
    {
        private final int inputSize;

        public Collector(int bufferSize)
        {
            this.inputSize = bufferSize;
        }
        
        public void run()
        {
            try
            {
                RadiostreamConnection conn1 =
                    (RadiostreamConnection)Connector.open("radiostream://0A00.020F.0000.1001:100");
                RadiostreamConnection conn2 =
                    (RadiostreamConnection)Connector.open("radiostream://0A00.020F.0000.1002:100");
                DataInputStream dis1 = conn1.openDataInputStream();
                DataInputStream dis2 = conn2.openDataInputStream();
                while(true)
                {
                    try
                    {
                        collect(dis1, dis2);
                    }
                    catch (InterruptedException ex){}
                }
            }
            catch(IOException ex) {}
        }
   
        private void collect(DataInputStream dis1, DataInputStream dis2)throws InterruptedException, IOException
        {
            //wait if queue is full
            //input.removeAllElements();
            System.out.println("inside collect Input size = " + input.size());
            while (input.size() == inputSize)
            {
                synchronized (input)
                {
                    System.out.println("Queue is full " + Thread.currentThread().getName()
                                        + " is waiting , size: " + input.size());
                    input.wait();
                }
            }
            //collect data and notify aggregator thread
            synchronized (input)
            {
                for(int i = 0; i < 8; ++i)
                {
                    input1[i] = dis1.readDouble();
                    input.addElement(new Double(input1[i]));
                }
                for(int i = 0; i < 8; ++i)
                {
                    input2[i] = dis2.readDouble();
                    input.addElement(new Double(input2[i]));
                }
                input.notifyAll();
            }
        }
    }
    
    class Aggregator implements Runnable
    {

        public Aggregator()
        {}

        public void run()
        {
            try
            {
                RadiostreamConnection conn3 =
                    (RadiostreamConnection)Connector.open("radiostream://0A00.020F.0000.1004:100");
                DataOutputStream sinkDos = conn3.openDataOutputStream();
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException ex) {}
                while(true)
                {
                    try
                    {
                        boolean aggregateFlag = control();
                        if(aggregateFlag == true)//send merged stream
                        {
                            System.out.println("SENDING MERGED STREAM TO SINK ...");
                            sinkDos.writeBoolean(true);
                            sinkDos.flush();
                            for(int i = 0; i < 8; ++i)
                            {
                                sinkDos.writeDouble((output1[i] + output2[i]) / 2.0);
                                sinkDos.flush();
                            }
                            for(int i = 0; i < 8; ++i)
                            {
                                execute();
                                System.out.println("SENDING MERGED STREAM TO SINK ...");
                                sinkDos.writeBoolean(true);
                                sinkDos.flush();
                                for(int j = 0; j < 8; ++j)
                                {
                                    sinkDos.writeDouble((output1[j] + output2[j]) / 2.0);
                                    sinkDos.flush();
                                }
                            }
                        }
                        else//send both streams
                        {
                            System.out.println("SENDING BOTH STREAMS TO SINK ...");
                            sinkDos.writeBoolean(false);
                            sinkDos.flush();
                            for(int i = 0; i < 8; ++i)
                            {
                                sinkDos.writeDouble(output1[i]);
                                sinkDos.flush();
                            }
                            for(int i = 0; i < 8; ++i)
                            {
                                sinkDos.writeDouble(output2[i]);
                                sinkDos.flush();
                            }
                        }
                    }
                    catch (InterruptedException ex) {}
                }
            }
            catch (IOException ex) {}
        }

        private boolean control() throws InterruptedException
        {
            //wait if queue is empty
            System.out.println("inside control Input size = " + input.size());
            while (input.isEmpty())
            {
                synchronized (input)
                {
                    System.out.println("Queue is empty " + Thread.currentThread().getName()
                                        + " is waiting , size: " + input.size());
                    input.wait();
                }
            }
            //Otherwise get the data and notify waiting collector thread
            synchronized (input)
            {
                for(int i = 0; i < 8; ++i)
                {
                    output1[i] = input1[i];
                    output2[i] = input2[i];
                }
                input.removeAllElements();//clear the input data
                input.notifyAll();//notify the collector
            }
            //Work on the data
            Complex[] cinput1 = new Complex[8];
            Complex[] cinput2 = new Complex[8];
            for (int i = 0; i < 8; ++i)
            {
                cinput1[i] = new Complex(output1[i], 0.0);
                cinput2[i] = new Complex(output2[i], 0.0);
            }
            fft(cinput1);//run fft on input set1
            fft(cinput2);//run fft on input set2
            System.out.println("FFT TRANSFORMATION COMPLETED ...");
            quickSort(cinput1, 0, 7);
            quickSort(cinput2, 0, 7);
            System.out.println("CALCULATING EUCLIDEAN DISTANCE BETWEEN STREAMS...");
            double euDist;
            double deltaRe;
            double deltaIm;
            for(int i = 0; i < 4; ++i)//use the 4 lowest spectrum values
            {
                deltaRe = cinput1[i].re - cinput2[i].re;
                deltaIm = cinput1[i].im - cinput2[i].im;
                euDist = Math.sqrt((deltaRe * deltaRe) + (deltaIm * deltaIm));
                System.out.println("EUCLIDEAN DISTANCE[" + i + "] = " + euDist);
                if(euDist > THRESHOLD)
                {
                    System.out.println("THRESHOLD EXCEEDED ...");
                    return false;
                }
            }
            return true;
        }
        
        private void execute() throws InterruptedException
        {
            //wait if queue is empty
            while (input.isEmpty())
            {
                synchronized (input)
                {
                    System.out.println("Queue is empty " + Thread.currentThread().getName()
                                        + " is waiting , size: " + input.size());
                    input.wait();
                }
            }
            //Otherwise get the data and notify waiting collector thread
            synchronized (input)
            {
                for(int i = 0; i < 8; ++i)
                {
                    output1[i] = input1[i];
                    output2[i] = input2[i];
                }
                input.removeAllElements();//clear the input data
                input.notifyAll();//notify the collector
            }
        }
    }
    
    //Main MIDlet function
    protected void startApp() throws MIDletStateChangeException 
    {
        System.out.println("AggregatorNode2019 MIDlet STARTING ...");
        Thread t1Collector = new Thread(new Collector(vectorSize), "Collector");
        Thread t2Aggregator = new Thread(new Aggregator(), "Aggregator");
        t1Collector.start();
        t2Aggregator.start();
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
