package gr.di.uoa;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import java.io.File;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MainActivity extends AppCompatActivity
{
  private String userIP = "";
  private String userTopic = "";
  private int userPort = -1;
  private int userTimesteps = -1;
  private String userCSV = "";

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    Toolbar toolbar = findViewById(R.id.toolbar);
    setSupportActionBar(toolbar);
    final Button buttonOne = findViewById(R.id.initButton);
    buttonOne.setOnClickListener(new View.OnClickListener()
    {
      public void onClick(View v)
      {
        Intent activity2Intent = new Intent(getApplicationContext(), ConnectionActivity.class);
        if(userIP.equals("") || userPort == -1 || userTopic.equals("") || userCSV.equals("") || userTimesteps == -1)
          Toast.makeText(MainActivity.this, "Set \"Application Parameters\" first, before connecting!", Toast.LENGTH_SHORT).show();
        else//pass the given data to the next activity
        {
          activity2Intent.putExtra("ip", userIP);
          activity2Intent.putExtra("port", userPort);
          activity2Intent.putExtra("timesteps", userTimesteps);
          activity2Intent.putExtra("topic", userTopic);
          activity2Intent.putExtra("csv", userCSV);
          startActivity(activity2Intent);
        }
      }
    });
  }

  @Override
  public boolean onCreateOptionsMenu(Menu menu)
  {
    // Inflate the menu; this adds items to the action bar if it is present.
    getMenuInflater().inflate(R.menu.menu_main, menu);
    return true;
  }

  @Override
  public boolean onOptionsItemSelected(MenuItem item)
  {
    // Handle action bar item clicks here. The action bar will
    // automatically handle clicks on the Home/Up button, so long
    // as you specify a parent activity in AndroidManifest.xml.
    //show the selected items on click
    switch(item.getItemId())
    {
      case R.id.networkParameters:
        netDataValidation();
        return true;

      case R.id.timesteps:
        timeDataValidation();
        return true;

      case R.id.exit:
        exitValidation();
        return true;

      default:
        return super.onOptionsItemSelected(item);
    }
  }

  @Override
  public void onBackPressed()
  {
    exitValidation();
  }

  //this method checks if given string is a positive integer
  private boolean isPosNum(String strNum)
  {
    try
    {
      int x = Integer.parseInt(strNum);
      if(x <= 0)
        return false;
    }
    catch(NumberFormatException | NullPointerException nfe)
    {
      return false;
    }
    return true;
  }

  //this method checks if IP address is valid
  private boolean isValidInet4Address(String ip)
  {
    if(ip == null)
      return false;
    //this regex is used for checking if IP address format is right
    final String IPv4_REGEX = "^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."+
                              "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\." +
                              "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\." +
                              "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$";
    final Pattern IPv4_PATTERN = Pattern.compile(IPv4_REGEX);
    Matcher matcher = IPv4_PATTERN.matcher(ip);
    return matcher.matches();
  }

  //method to validate network parameters
  private void netDataValidation()
  {
    AlertDialog.Builder netbuilder = new AlertDialog.Builder(this);
    netbuilder.setCancelable(false);
    // Get the layout inflater
    LayoutInflater netinflater = LayoutInflater.from(this);
    final View netView = netinflater.inflate(R.layout.networkparamsinput, null);
    // Inflate and set the layout for the dialog
    // Pass null as the parent view because its going in the dialog layout
    netbuilder.setView(netView)
            .setPositiveButton(R.string.apply, new DialogInterface.OnClickListener()
            {
              public void onClick(DialogInterface dialog, int id)
              {
                final EditText port = netView.findViewById(R.id.port);
                final EditText ip = netView.findViewById(R.id.IP);
                final EditText topic = netView.findViewById(R.id.topic);
                final String portStr = port.getText().toString();
                if(isPosNum(portStr))
                  userPort = Integer.parseInt(port.getText().toString());
                else
                  Toast.makeText(MainActivity.this, "Given port format is wrong", Toast.LENGTH_SHORT).show();
                userIP = ip.getText().toString();
                if(!isValidInet4Address(userIP))
                {
                  Toast.makeText(MainActivity.this, "Given IP format is wrong", Toast.LENGTH_SHORT).show();
                  userIP = "";
                }
                userTopic = topic.getText().toString();
                if(userTopic.equals(""))
                  Toast.makeText(MainActivity.this, "Given topic is empty", Toast.LENGTH_SHORT).show();
                dialog.cancel();
              }
            })
            .setNegativeButton(R.string.cancel, new DialogInterface.OnClickListener()
            {
              public void onClick(DialogInterface dialog, int id)
              {
                dialog.cancel();
              }
            });
    AlertDialog NetPopup = netbuilder.create();
    NetPopup.show();
  }

  //method to validate timesteps and file parameters
  private void timeDataValidation()
  {
    AlertDialog.Builder timebuilder = new AlertDialog.Builder(this);
    timebuilder.setCancelable(false);
    // Get the layout inflater
    LayoutInflater timeinflater = LayoutInflater.from(this);
    final View timeView = timeinflater.inflate(R.layout.timesteps, null);
    // Inflate and set the layout for the dialog
    // Pass null as the parent view because its going in the dialog layout
    timebuilder.setView(timeView)// Add action buttons
            .setPositiveButton(R.string.apply, new DialogInterface.OnClickListener()
            {
              public void onClick(DialogInterface dialog, int id)
              {
                final EditText timesteps = timeView.findViewById(R.id.time);
                String timestepsStr = timesteps.getText().toString();
                final EditText file = timeView.findViewById(R.id.file);
                userCSV = file.getText().toString();
                if(timestepsStr.equals(""))
                  userTimesteps = 0;
                else if(isPosNum(timestepsStr))
                  userTimesteps = Integer.parseInt(timestepsStr);
                else
                  Toast.makeText(MainActivity.this, "Wrong timesteps format", Toast.LENGTH_SHORT).show();
                if(userCSV.equals(""))
                  Toast.makeText(MainActivity.this, "Please specify a valid CSV file to read data", Toast.LENGTH_SHORT).show();
                else
                {
                  File f = new File("/data/data/gr.di.uoa/files/" + userCSV);
                  if(!(f.exists() && !f.isDirectory()))
                  {
                    Toast.makeText(MainActivity.this, "Given file doesn't exist", Toast.LENGTH_SHORT).show();
                    userCSV = "";
                  }
                }
                dialog.cancel();
              }
            })
            .setNegativeButton(R.string.cancel, new DialogInterface.OnClickListener()
            {
              public void onClick(DialogInterface dialog, int id)
              {
                dialog.cancel();
              }
            });
    AlertDialog TimePopup = timebuilder.create();
    TimePopup.show();
  }

  //method to validate exit from the app
  private void exitValidation()
  {
    AlertDialog.Builder builder1 = new AlertDialog.Builder(this);
    builder1.setMessage("Are you sure you want to exit?");
    builder1.setCancelable(false);
    builder1.setPositiveButton("YES",new DialogInterface.OnClickListener()
    {
      public void onClick(DialogInterface dialog, int id)
      {
        finish();
      }
    });
    builder1.setNegativeButton("NO", new DialogInterface.OnClickListener()
    {
      public void onClick(DialogInterface dialog, int id)
      {
        dialog.cancel();
      }
    });
    AlertDialog ExitPopup = builder1.create();
    ExitPopup.show();
  }
}
