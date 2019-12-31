package gr.di.uoa;

import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Toast;
import android.widget.ToggleButton;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.Dot;
import com.google.android.gms.maps.model.Gap;
import com.google.android.gms.maps.model.JointType;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;
import com.google.android.gms.maps.model.PatternItem;
import com.google.android.gms.maps.model.Polygon;
import com.google.android.gms.maps.model.PolygonOptions;
import com.google.android.gms.maps.model.Polyline;
import com.google.android.gms.maps.model.PolylineOptions;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;

import mqtthandler.MqttHandler;
import helpers.CustomInfoWindowAdapter;


public class ConnectionActivity extends    AppCompatActivity
                                implements GoogleMap.OnMarkerClickListener,
                                           GoogleMap.OnInfoWindowClickListener
{
  private volatile boolean sending = false;
  private boolean alertNotif = false;
  private volatile boolean killThread = false;
  private volatile MqttHandler mosqHandler = null;
  private GoogleMap realMap = null;
  private GoogleMap predMap = null;
  private SupportMapFragment mapFragmentReal = null;
  private SupportMapFragment mapFragmentPredicted = null;
  private LatLng realOldPos = null;
  private LatLng realNewPos = null;
  private LatLng predOldPos = null;
  private LatLng predNewPos = null;
  private final double minLon = 23.7647600;
  private final double maxLon = 23.7753900;
  private final double minLat = 37.9668800;
  private final double maxLat = 37.9686200;
  private String terminalID;

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_connection);
    Intent intent = getIntent();
    final String brokerIP = intent.getStringExtra("ip");
    final int brokerPort = intent.getIntExtra("port", 1883);
    final int totalTimesteps = intent.getIntExtra("timesteps", 0);
    final String brokerTopic = intent.getStringExtra("topic");
    if(Objects.requireNonNull(brokerTopic).contains("topic26"))
      terminalID = "26";
    else if(brokerTopic.contains("topic27"))
      terminalID = "27";
    final String csvFile = intent.getStringExtra("csv");
    final String clientID = getAlphaNumericString(32);//generate a 32 char random client id
    final Context appContext = getApplicationContext();
    //create the MQTT client handler
    mosqHandler = new MqttHandler(brokerIP, brokerPort, brokerTopic, clientID, 2, appContext);
    //get the SupportMapFragment and request notification when both maps are ready to be used
    mapFragmentReal = (SupportMapFragment) getSupportFragmentManager().findFragmentById(R.id.mapReal);
    if(mapFragmentReal != null)
      mapFragmentReal.getMapAsync(onMapReadyCallbackReal());
    mapFragmentPredicted = (SupportMapFragment) getSupportFragmentManager().findFragmentById(R.id.mapPredicted);
    if(mapFragmentPredicted != null)
      mapFragmentPredicted.getMapAsync(onMapReadyCallbackPredicted());

    //network checker thread code
    Runnable netRunnable = new Runnable()
    {
      @Override
      public void run()
      {
        while(true)
        {
          try
          {
            Thread.sleep(5000);//periodic check every 5 seconds
            if(!hasNetwork())
              showAlert();
          }
          catch(InterruptedException e)
          {
            e.printStackTrace();
          }
        }
      }
    };
    Thread netCheckThread = new Thread(netRunnable);
    netCheckThread.start();

    //create the connect with edge server button
    final Button buttonOne = findViewById(R.id.connectButton);
    buttonOne.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View v)
      {
        if(mosqHandler.mosqGetClient() != null)
          Toast.makeText(ConnectionActivity.this, "Already connected to Edge Server", Toast.LENGTH_SHORT).show();
        else
        {
          mosqHandler.mosqConnect();
          //msg checker thread code handles data received from edge server
          Runnable msgRunnable = new Runnable()
          {
            @Override
            public void run()
            {
              killThread = false;//spawn just 1 thread
              while(!killThread)//read published data from edge server
              {
                if(!mosqHandler.getRdy())
                {
                  try
                  {
                    Thread.sleep(0);
                  }
                  catch(InterruptedException e)
                  {
                    e.printStackTrace();
                  }
                }
                else
                {
                  String curMsg = mosqHandler.getMsg();
                  int count = curMsg.length() - curMsg.replace(",", "").length();
                  if(count != 7 && !curMsg.contains("END OF DATA"))//filter out messages from self
                  {
                    //showToast("Received message from edge server = " + curMsg);
                    addMarksOnMap(curMsg, predMap, false);
                  }
                }
              }
            }
          };
          Thread msgCheckThread = new Thread(msgRunnable);
          msgCheckThread.start();
        }
      }
    });

    //create the send data button
    final Button buttonTwo = findViewById(R.id.sendButton);
    buttonTwo.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View v)
      {
        if(sending)
          Toast.makeText(ConnectionActivity.this, "Sending data already to Edge Server", Toast.LENGTH_SHORT).show();
        else if(mosqHandler.mosqGetClient() == null)
          Toast.makeText(ConnectionActivity.this, "Connect to Edge Server first", Toast.LENGTH_SHORT).show();
        else
        { //use a worker thread since this task is time costly
          Runnable runnable = new Runnable()
          {
            @Override
            public void run()
            {
              Context ctx = getApplicationContext();
              FileInputStream fileInputStream = null;
              try
              {
                fileInputStream = ctx.openFileInput(csvFile);
              }
              catch(FileNotFoundException e)
              {
                e.printStackTrace();
              }
              InputStreamReader inputStreamReader = new InputStreamReader(Objects.requireNonNull(fileInputStream));
              BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
              sending = true;
              if(totalTimesteps == 0)//send all
              {
                try
                {
                  String lineData;
                  while(true)
                  {
                    lineData = bufferedReader.readLine();
                    if((!sending) || lineData == null)
                      break;
                    addMarksOnMap(lineData, realMap, true);//add the marker on the real map
                    mosqHandler.mosqPublish(lineData);
                    Thread.sleep(1000);//send with 1hz frequency
                  }
                  if(mosqHandler.mosqGetClient() != null)
                    mosqHandler.mosqPublish("END OF DATA");
                  sending = false;
                  realOldPos = null;
                  predOldPos = null;
                }
                catch(IOException | InterruptedException e)
                {
                  e.printStackTrace();
                }
              }
              else
              {
                try
                {
                  String lineData;
                  int counter = 0;
                  while(counter < totalTimesteps)//send based on timesteps
                  {
                    lineData = bufferedReader.readLine();
                    if((!sending) || lineData == null)
                      break;
                    addMarksOnMap(lineData, realMap, true);//add the marker on the real map
                    mosqHandler.mosqPublish(lineData);
                    Thread.sleep(1000);//send with 1hz frequency
                    ++counter;
                  }
                  if(mosqHandler.mosqGetClient() != null)
                    mosqHandler.mosqPublish("END OF DATA");
                  sending = false;
                  realOldPos = null;
                  predOldPos = null;
                }
                catch(IOException | InterruptedException e)
                {
                  e.printStackTrace();
                }
              }
            }
          };
          Thread workerThread = new Thread(runnable);
          workerThread.start();
        }
      }
    });

    //create the stop button
    final Button stopButton = this.findViewById(R.id.stopButton);
    stopButton.setOnClickListener(new View.OnClickListener()
    {
      @Override
      public void onClick(View v)
      {
        if(mosqHandler.mosqGetClient() == null)
          Toast.makeText(ConnectionActivity.this, "Not connected to Edge Server", Toast.LENGTH_SHORT).show();
        else if(!sending)
          Toast.makeText(ConnectionActivity.this, "No data is currently being sent...", Toast.LENGTH_SHORT).show();
        else
        {
          sending = false;
          realOldPos = null;
          predOldPos = null;
          killThread = true;
        }
      }
    });

    //create the toggle button to change between real and predicted maps
    ToggleButton toggle = findViewById(R.id.mapToggleButton);
    toggle.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener()
    {
      public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
      {
        if(isChecked)//toggle enabled
        {
          Toast.makeText(ConnectionActivity.this, "Toggle OFF", Toast.LENGTH_SHORT).show();
          getSupportFragmentManager().beginTransaction().hide(mapFragmentReal).commit();
          getSupportFragmentManager().beginTransaction().show(mapFragmentPredicted).commit();
        }
        else//toggle disabled
        {
          Toast.makeText(ConnectionActivity.this, "Toggle ON", Toast.LENGTH_SHORT).show();
          getSupportFragmentManager().beginTransaction().hide(mapFragmentPredicted).commit();
          getSupportFragmentManager().beginTransaction().show(mapFragmentReal).commit();
        }
      }
    });
  }

  //this method is used to create the initial real map
  public OnMapReadyCallback onMapReadyCallbackReal()
  {
    return new OnMapReadyCallback()
    {
      @Override
      public void onMapReady(GoogleMap googleMap)
      {
        //get the variable instance to use in other parts of code
        realMap = googleMap;
        //set the custom info window
        realMap.setInfoWindowAdapter(new CustomInfoWindowAdapter(ConnectionActivity.this));
        //create the observed grid area
        addPolygonAreaOnMap(realMap);
        //add a marker in DI and move the map's camera to the same location
        LatLng ekpa = new LatLng(37.968481, 23.766910);
        Marker ekpaMarker = realMap.addMarker(new MarkerOptions()
                .position(ekpa)
                .title("DI ΕΚΠΑ")
                .snippet("Real Map"));
        ekpaMarker.setTag(0);
        //move the camera over it
        final float zoomLevel = 16.0f; //Zoom between 2.0f(min) and 21.0f(max)
        realMap.moveCamera(CameraUpdateFactory.newLatLngZoom(ekpa, zoomLevel));
        //set a listener for marker click
        realMap.setOnMarkerClickListener(ConnectionActivity.this);
        //set a listener for info window events
        realMap.setOnInfoWindowClickListener(ConnectionActivity.this);
      }
    };
  }

  //this method is used to create the initial predicted map
  public OnMapReadyCallback onMapReadyCallbackPredicted()
  {
    return new OnMapReadyCallback()
    {
      @Override
      public void onMapReady(GoogleMap googleMap)
      {
        //get the variable instance to use in other parts of code
        predMap = googleMap;
        //set the custom info window
        predMap.setInfoWindowAdapter(new CustomInfoWindowAdapter(ConnectionActivity.this));
        //create the observed grid area
        addPolygonAreaOnMap(predMap);
        //add a marker in DI and move the map's camera to the same location
        LatLng ekpa = new LatLng(37.968481, 23.766910);
        Marker ekpaMarker = predMap.addMarker(new MarkerOptions()
                .position(ekpa)
                .title("DI ΕΚΠΑ")
                .snippet("Predicted Map"));
        ekpaMarker.setTag(0);
        //move the camera over it
        final float zoomLevel = 16.0f; //Zoom between 2.0f(min) and 21.0f(max)
        predMap.moveCamera(CameraUpdateFactory.newLatLngZoom(ekpa, zoomLevel));
        //set a listener for marker click
        predMap.setOnMarkerClickListener(ConnectionActivity.this);
        //set a listener for info window events
        predMap.setOnInfoWindowClickListener(ConnectionActivity.this);
        //force hide the predicted map initially
        getSupportFragmentManager().beginTransaction().hide(mapFragmentPredicted).commit();
      }
    };
  }

  @Override
  public boolean onMarkerClick(final Marker marker)
  {
    // Retrieve the data from the marker.
    Integer clickCount = (Integer) marker.getTag();
    // Check if a click count was set, then display the click count.
    if(clickCount != null)
    {
      clickCount = clickCount + 1;
      marker.setTag(clickCount);
      //Toast.makeText(this, marker.getTitle() + " has been clicked " + clickCount + " times.", Toast.LENGTH_SHORT).show();
    }
    // Return false to indicate that we have not consumed the event and that we wish
    // for the default behavior to occur (which is for the camera to move such that the
    // marker is centered and for the marker's info window to open, if it has one).
    return false;
  }

  @Override
  public void onInfoWindowClick(Marker marker)
  {
    //Toast.makeText(this, "Info window clicked", Toast.LENGTH_SHORT).show();
  }

  @Override
  public void onBackPressed()
  {
    alertNotif = true;
    AlertDialog.Builder builder1 = new AlertDialog.Builder(this);
    builder1.setMessage("Are you sure you want to exit?");
    builder1.setCancelable(false);
    builder1.setPositiveButton("YES",new DialogInterface.OnClickListener()
    {
      public void onClick(DialogInterface dialog, int id)
      {
        sending = false;
        realOldPos = null;
        predOldPos = null;
        killThread = true;
        finish();
      }
    });
    builder1.setNegativeButton("NO", new DialogInterface.OnClickListener()
    {
      public void onClick(DialogInterface dialog, int id)
      {
        alertNotif = false;
        dialog.cancel();
      }
    });
    AlertDialog ExitPopup = builder1.create();
    ExitPopup.show();
  }

  //this method generates a random string of length n
  private String getAlphaNumericString(int n)
  {
    // chose a Character random from this String
    String AlphaNumericString = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                              + "0123456789"
                              + "abcdefghijklmnopqrstuvxyz";
    // create StringBuffer size of AlphaNumericString
    StringBuilder sb = new StringBuilder(n);
    for (int i = 0; i < n; ++i)
    {
      // generate a random number between
      // 0 to AlphaNumericString variable length
      int index = (int)(AlphaNumericString.length() * Math.random());
      // add Character one by one in end of sb
      sb.append(AlphaNumericString.charAt(index));
    }
    return sb.toString();
  }

  //this method checks whether the android device has internet connection
  private boolean hasNetwork()
  {
    boolean hasNet = false;
    ConnectivityManager connectivityManager = (ConnectivityManager) ConnectionActivity.this.getSystemService(Context.CONNECTIVITY_SERVICE);
    Network[] networks = Objects.requireNonNull(connectivityManager).getAllNetworks();
    NetworkInfo networkInfo;
    for(Network currentNet : networks)
    {
      networkInfo = connectivityManager.getNetworkInfo(currentNet);
      if(Objects.requireNonNull(networkInfo).getState().equals(NetworkInfo.State.CONNECTED))
        hasNet = true;
    }
    return hasNet;
  }

  //this method shows an alert prompt and is being called from within threads
  private void showAlert()
  {
    runOnUiThread(new Runnable()
    {
      @Override
      public void run()
      {
        AlertDialog.Builder conbuilder = new AlertDialog.Builder(ConnectionActivity.this);
        conbuilder.setCancelable(false);
        // Get the layout inflater
        LayoutInflater coninflater = LayoutInflater.from(ConnectionActivity.this);
        final View conView = coninflater.inflate(R.layout.connection_validate, null);
        conbuilder.setView(conView)
                .setPositiveButton(R.string.netReady, new DialogInterface.OnClickListener()
                {
                  public void onClick(DialogInterface dialog, int id)
                  {
                    alertNotif = false;
                  }
                });
        if(!alertNotif)
        {
          AlertDialog conPopup = conbuilder.create();
          conPopup.show();
          alertNotif = true;
        }
      }
    });
  }

  //this method show messages from within threads
  private void showToast(final String toast)
  {
    runOnUiThread(new Runnable()
    {
      @Override
      public void run()
      {
        Toast.makeText(ConnectionActivity.this, toast, Toast.LENGTH_SHORT).show();
      }
    });
  }

  //this method breaks down the string data and depending on the type (server data or own)
  //adds the markers on the provided map, creates the info windows and finally connects
  //the points with a line to show the terminal's movement using red for positions inside the
  //observed grid and blue for positions outside it
  private void addMarksOnMap(final String data, final GoogleMap curMap, final boolean type)
  {
    runOnUiThread(new Runnable()
    {
      @Override
      public void run()
      {
        //filter out the header, and not located in grid message
        if(data.contains("timestep, id, lat, long, angle, speed, RSSI, throughput") ||
           data.contains("Could not locate your position,"))
        {
          return;
        }
        String temp = data;
        double timestep;
        double lat;
        double lon;
        double rssi;
        double throughput;
        String vehicleID;

        if(type)//real map
        {
          //extract the timestep
          int pos = temp.indexOf(',');
          String sTimestep = temp.substring(0, pos);
          timestep = Double.parseDouble(sTimestep);
          temp = temp.substring(pos+2);
          //extract the vehicle id
          pos = temp.indexOf(',');
          vehicleID = temp.substring(0, pos);
          temp = temp.substring(pos+2);
          //extract the latitude
          pos = temp.indexOf(',');
          String sLat = temp.substring(0, pos);
          lat = Double.parseDouble(sLat);
          temp = temp.substring(pos+2);
          //extract the longitude
          pos = temp.indexOf(',');
          String sLon = temp.substring(0, pos);
          lon = Double.parseDouble(sLon);
          temp = temp.substring(pos+2);
          //skip the angle and space char
          pos = temp.indexOf(',');
          temp = temp.substring(pos+2);
          //skip the speed and space char
          pos = temp.indexOf(',');
          temp = temp.substring(pos+2);
          //extract the RSSI
          pos = temp.indexOf(',');
          String sRSSI = temp.substring(0, pos);
          rssi = Double.parseDouble(sRSSI);
          temp = temp.substring(pos+2);
          //extract the throughput
          throughput = Double.parseDouble(temp);

          //create the location and add the marker and the data
          realNewPos = new LatLng(lat, lon);
          Marker curMark = curMap.addMarker(new MarkerOptions()
                  .position(realNewPos)
                  .title("Android Terminal " + terminalID)
                  .snippet("Timestep : " + timestep + "\n" +
                           "Location : (" + lat + ", " + lon + ")\n" +
                           "RSSI : " + rssi + "\n" +
                           "Throughput : " + throughput));
          curMark.setTag(0);
          if(realOldPos == null)//dont connect
            realOldPos = realNewPos;//update
          else//connect the markers
          {
            if((realNewPos.latitude <= maxLat && realNewPos.latitude >= minLat) &&
               (realNewPos.longitude <= maxLon && realNewPos.longitude >= minLon) &&
               (realOldPos.latitude <= maxLat && realOldPos.latitude >= minLat) &&
               (realOldPos.longitude <= maxLon && realOldPos.longitude >= minLon))//in grid
            {
              Polyline connLine = curMap.addPolyline(new PolylineOptions()
                      .clickable(false)
                      .add(realOldPos, realNewPos));
              connLine.setTag(terminalID);
              connLine.setWidth(12);//line thickness
              connLine.setColor(0xffff0000);//red full opacity (ARGB values)
              connLine.setJointType(JointType.ROUND);
            }
            else//outside grid
            {
              Polyline connLine = curMap.addPolyline(new PolylineOptions()
                      .clickable(false)
                      .add(realOldPos, realNewPos));
              connLine.setTag(terminalID);
              connLine.setWidth(12);//line thickness
              connLine.setColor(0xff0000ff);//blue full opacity (ARGB values)
              connLine.setJointType(JointType.ROUND);
            }
            realOldPos = realNewPos;//update
          }
        }
        else//predicted map
        {
          //extract the timestep
          int pos = temp.indexOf(',');
          String sTimestep = temp.substring(0, pos);
          timestep = Double.parseDouble(sTimestep);
          temp = temp.substring(pos+2);
          //extract the latitude
          pos = temp.indexOf(',');
          String sLat = temp.substring(0, pos);
          lat = Double.parseDouble(sLat);
          temp = temp.substring(pos+2);
          //extract the longitude
          pos = temp.indexOf(',');
          String sLon = temp.substring(0, pos);
          lon = Double.parseDouble(sLon);
          temp = temp.substring(pos+2);
          //extract the RSSI
          pos = temp.indexOf(',');
          String sRSSI = temp.substring(0, pos);
          rssi = Double.parseDouble(sRSSI);
          temp = temp.substring(pos+2);
          //extract the throughput
          throughput = Double.parseDouble(temp);

          //create the location and add the marker and the data
          predNewPos = new LatLng(lat, lon);
          Marker curMark = curMap.addMarker(new MarkerOptions()
                  .position(predNewPos)
                  .title("Android Terminal " + terminalID)
                  .snippet("Timestep : " + timestep + "\n" +
                          "Location : (" + lat + ", " + lon + ")\n" +
                          "RSSI : " + rssi + "\n" +
                          "Throughput : " + throughput));
          curMark.setTag(0);
          if(predOldPos == null)//dont connect
            predOldPos = predNewPos;//update
          else//connect the markers
          {
            if((predNewPos.latitude <= maxLat && predNewPos.latitude >= minLat) &&
               (predNewPos.longitude <= maxLon && predNewPos.longitude >= minLon) &&
               (predOldPos.latitude <= maxLat && predOldPos.latitude >= minLat) &&
               (predOldPos.longitude <= maxLon && predOldPos.longitude >= minLon))//in grid
            {
              Polyline connLine = curMap.addPolyline(new PolylineOptions()
                      .clickable(false)
                      .add(predOldPos, predNewPos));
              connLine.setTag(terminalID);
              connLine.setWidth(12);//line thickness
              connLine.setColor(0xffff0000);//red full opacity (ARGB values)
              connLine.setJointType(JointType.ROUND);
            }
            else//outside grid
            {
              Polyline connLine = curMap.addPolyline(new PolylineOptions()
                      .clickable(false)
                      .add(predOldPos, predNewPos));
              connLine.setTag(terminalID);
              connLine.setWidth(12);//line thickness
              connLine.setColor(0xff0000ff);//blue full opacity (ARGB values)
              connLine.setJointType(JointType.ROUND);
            }
            predOldPos = predNewPos;//update
          }
        }
      }
    });
  }

  //this method marks the area on given map which is the observed area grid
  //inside which we have valid prediction data from the edge server
  private void addPolygonAreaOnMap(final GoogleMap curMap)
  {
    final PatternItem dot = new Dot();
    final PatternItem gap = new Gap(10);
    final List<PatternItem> polygonDotGap = Arrays.asList(dot, gap);
    Polygon gridArea = curMap.addPolygon(new PolygonOptions()
            .clickable(false)
            .add(new LatLng(minLat, minLon), new LatLng(minLat, maxLon),
                 new LatLng(maxLat, maxLon), new LatLng(maxLat, minLon)));
    gridArea.setTag("Simulation Grid Area");
    gridArea.setStrokeJointType(JointType.ROUND);
    gridArea.setStrokePattern(polygonDotGap);
    gridArea.setStrokeWidth(8);//line thickness
    gridArea.setStrokeColor(0xffF57F17);//orange full opacity (ARGB values)
  }
}