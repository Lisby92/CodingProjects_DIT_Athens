package edgeServer;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

public class MQTTclientHandler implements MqttCallback
{
  private String topic;
  private int qos;
  private MqttClient client;
  private boolean msgRdy;
  private String actualMessage;

  //constructor
  public MQTTclientHandler(String topic, int qos, String broker, String clientId) throws MqttException
  {
    //System.out.println("I just created a MQTTclientHandler!");
    this.topic = topic;
    this.qos = qos;
    this.msgRdy = false;
    MemoryPersistence persistence = new MemoryPersistence();
    client = new MqttClient(broker, clientId, persistence);
    MqttConnectOptions connOpts = new MqttConnectOptions();
    connOpts.setCleanSession(true);
    System.out.println("Connecting to MQTT Mosquito broker: "+broker);
    client.connect(connOpts);
    client.setCallback(this);
    System.out.println("Connected");
  }
  //publish method
  public void MQTTpublish(String content) throws MqttException
  {
    MqttMessage message = new MqttMessage(content.getBytes());
    message.setQos(qos);
    client.publish(topic, message);
    System.out.println("Publishing @ topic: " + topic + ", message: "+content);
  }
  //subscribe method
  public void MQTTsubscribe() throws MqttException
  {
    client.subscribe(topic, qos);
  }

  public void MQTTdisconnect() throws MqttException
  {
    client.disconnect();
    System.out.println("Disconnected successfully");
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
  public void connectionLost(Throwable throwable)
  {
    System.out.println("Connection with the internet lost!");
  }

  @Override
  public void messageArrived(String s, MqttMessage mqttMessage) throws Exception
  {
    setMsg(new String(mqttMessage.getPayload()));
    //filter out messages from self
    int count = actualMessage.length() - actualMessage.replace(",", "").length();
    if(count != 4 && !actualMessage.contains("Could not locate your position,"))
      msgRdy = true;
  }

  @Override
  public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken)
  {
    //System.out.println("I just sent a message!");
  }
}
