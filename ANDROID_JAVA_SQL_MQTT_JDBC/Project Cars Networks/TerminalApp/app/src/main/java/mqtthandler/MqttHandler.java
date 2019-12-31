package mqtthandler;

import android.content.Context;
import android.widget.Toast;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class MqttHandler implements MqttCallbackExtended
{
  private String brokerIP;
  private int brokerPort;
  private String brokerTopic;
  private int brokerQOS;
  private String clientID;
  private MqttAndroidClient client;
  private Context callerActivityContext;
  private boolean msgRdy;
  private String actualMessage;

  public MqttHandler(String brokerIP, int brokerPort, String brokerTopic, String clientID, int brokerQOS, Context context)
  {
    this.brokerIP = brokerIP;
    this.brokerPort = brokerPort;
    this.brokerTopic = brokerTopic;
    this.clientID = clientID;
    this.brokerQOS = brokerQOS;
    this.client = null;
    this.callerActivityContext = context;
    this.msgRdy = false;
  }

  public MqttAndroidClient mosqGetClient()
  {
    return client;
  }

  public void mosqSetClient(final MqttAndroidClient newClient)
  {
    this.client = newClient;
  }

  //this method publishes the given message @ given topic
  public void mosqPublish(String message)
  {
    if(client != null)
    {
      try
      {
        client.publish(brokerTopic, message.getBytes(), brokerQOS, false);
      }
      catch(MqttException e)
      {
        e.printStackTrace();
      }
    }
  }

  //this method connects to the broker and subscribes to the topic
  public void mosqConnect()
  {
    client = new MqttAndroidClient(callerActivityContext, "tcp://" + brokerIP + ":" + brokerPort, clientID);
    client.setCallback(this);
    MqttConnectOptions options = new MqttConnectOptions();
    try
    {
      IMqttToken token = client.connect(options);
      token.setActionCallback(new IMqttActionListener()
      {
        @Override
        public void onSuccess(IMqttToken asyncActionToken)
        {
          //We are connected
          Toast.makeText(callerActivityContext, "Connected with MQTT broker successfully!", Toast.LENGTH_SHORT).show();
          try
          {
            client.subscribe(brokerTopic, brokerQOS, null, new IMqttActionListener()
            {
              @Override
              public void onSuccess(IMqttToken iMqttToken)
              {
                Toast.makeText(callerActivityContext, "Subscribed to topic successfully!", Toast.LENGTH_SHORT).show();
              }

              @Override
              public void onFailure(IMqttToken iMqttToken, Throwable throwable)
              {
                Toast.makeText(callerActivityContext, "Failed to subscribe to topic!", Toast.LENGTH_SHORT).show();
              }
            });
          }
          catch(MqttException e)
          {
            e.printStackTrace();
          }
        }

        @Override
        public void onFailure(IMqttToken asyncActionToken, Throwable exception)
        {
          //Something went wrong e.g. connection timeout or firewall problems
          Toast.makeText(callerActivityContext, "Connection with MQTT broker failed!", Toast.LENGTH_SHORT).show();
        }
      });
    }
    catch(MqttException e)
    {
      e.printStackTrace();
    }
  }

  //this method disconnects the client from the broker
  public void mosqDisconnect()
  {
    try
    {
      IMqttToken disconToken = client.disconnect();
      disconToken.setActionCallback(new IMqttActionListener()
      {
        @Override
        public void onSuccess(IMqttToken asyncActionToken)
        {
          // we are now successfully disconnected
          Toast.makeText(callerActivityContext, "Disconnected from MQTT broker successfully!", Toast.LENGTH_SHORT).show();
        }

        @Override
        public void onFailure(IMqttToken asyncActionToken, Throwable exception)
        {
          // something went wrong, but probably we are disconnected anyway
          Toast.makeText(callerActivityContext, "Failed to disconnect from MQTT broker", Toast.LENGTH_SHORT).show();
        }
      });
    }
    catch(MqttException e)
    {
      e.printStackTrace();
    }
  }

  //this method unsubscribes the client from the topic
  public void mosqUnsubscribe()
  {
    try
    {
      IMqttToken unsubToken = client.unsubscribe(brokerTopic);
      unsubToken.setActionCallback(new IMqttActionListener()
      {
        @Override
        public void onSuccess(IMqttToken asyncActionToken)
        {
          //The subscription could successfully be removed from the client
          Toast.makeText(callerActivityContext, "Unsubscribed from topic :" + brokerTopic + " successfully!", Toast.LENGTH_SHORT).show();
        }

        @Override
        public void onFailure(IMqttToken asyncActionToken, Throwable exception)
        {
          // some error occurred, this is very unlikely as even if the client
          // did not had a subscription to the topic the unsubscribe action
          // will be successfully
          Toast.makeText(callerActivityContext, "Failed to unsubscribe from topic : " + brokerTopic, Toast.LENGTH_SHORT).show();
        }
      });
    }
    catch(MqttException e)
    {
      e.printStackTrace();
    }
  }

  public boolean getRdy()
  {
    return msgRdy;
  }

  public String getMsg()
  {
    msgRdy = false;
    return actualMessage;
  }

  public void setMsg(String curMsg)
  {
    actualMessage = curMsg;
  }

  @Override
  public void connectComplete(boolean b, String s)
  {
    //not used currently
  }

  @Override
  public void connectionLost(Throwable throwable)
  {
    //not used currently
  }

  @Override
  public void messageArrived(String s, MqttMessage mqttMessage) throws Exception
  {
    setMsg(new String(mqttMessage.getPayload()));
    msgRdy = true;
    //Toast.makeText(callerActivityContext, "message received : " + getMsg(), Toast.LENGTH_SHORT).show();
  }

  @Override
  public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken)
  {
    //not used currently
  }
}
