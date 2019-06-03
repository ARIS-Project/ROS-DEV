/* 
 * rosserial Ultrasound Example
 * 
 * This example is for the ARIS Project developed at Hochschule Darmstadt.
 */
#include <ros.h>
#include <ros/time.h>
#include <std_msgs/Bool.h>

#define MIN_SAFE_DISTANCE  2
#define MAX_SAFE_DISTANCE 15
#define NUM_OF_SENSORS    10

  ros::NodeHandle  nh;
  
  std_msgs::Bool ultrasonic_msg;
  std_msgs::Bool ack_msg;
  std_msgs::Bool estop_msg;
  
  ros::Publisher pub_ack( "/aris_ack", &ack_msg);
  ros::Publisher pub_estop( "/aris_estop", &estop_msg);
  ros::Publisher pub_us( "/aris_US", &ultrasonic_msg);
  
  // this constant won't change. It's the pin number of the sensor's output:
  const int pingPin_1 = 24;
  const int echoPin_1 = 25;
  
  const int pingPin_2 = 26;
  const int echoPin_2 = 27;
  
  const int pingPin_3 = 28;
  const int echoPin_3 = 29;
  
  const int pingPin_4 = 30;
  const int echoPin_4 = 31;
  
  const int pingPin_5 = 32;
  const int echoPin_5 = 33;
  
  
  const int estopPin = 35;
  const int ackPin = 36;
  
  
  const int pingPin_6 = 38;
  const int echoPin_6 = 39;
  
  const int pingPin_7 = 40;
  const int echoPin_7 = 41;
  
  const int pingPin_8 = 42;
  const int echoPin_8 = 43;
  
  const int pingPin_9 = 44;
  const int echoPin_9 = 45;
  
  const int pingPin_10 = 46;
  const int echoPin_10 = 47;
  
  bool sensorStatus1 = false,sensorStatus2= false,sensorStatus3= false,sensorStatus4= false,
  sensorStatus5= false,sensorStatus6= false,sensorStatus7= false,
  sensorStatus8= false,sensorStatus9= false,sensorStatus10= false;

void setup()
{

  nh.initNode();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  nh.advertise(pub_us);
  nh.advertise(pub_ack);
  nh.advertise(pub_estop);

  pinMode(echoPin_1, INPUT);
  pinMode(pingPin_1, OUTPUT);
  
  pinMode(echoPin_2, INPUT);
  pinMode(pingPin_2, OUTPUT);

  pinMode(echoPin_3, INPUT);
  pinMode(pingPin_3, OUTPUT);

  pinMode(echoPin_4, INPUT);
  pinMode(pingPin_4, OUTPUT);

  pinMode(echoPin_5, INPUT);
  pinMode(pingPin_5, OUTPUT);

  pinMode(echoPin_6, INPUT);
  pinMode(pingPin_6, OUTPUT);
  
  pinMode(echoPin_7, INPUT);
  pinMode(pingPin_7, OUTPUT);

  pinMode(echoPin_8, INPUT);
  pinMode(pingPin_8, OUTPUT);

  pinMode(echoPin_9, INPUT);
  pinMode(pingPin_9, OUTPUT);

  pinMode(echoPin_10, INPUT);
  pinMode(pingPin_10, OUTPUT);
  
  pinMode(estopPin, INPUT);
  pinMode(ackPin, INPUT);
  
  //initialize serial communication:
  Serial.begin(57600);
}


long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}


  /* Read the status of each Ultra Sonic Sensors
   * The return value is true if the sensor detects 
   * an object within 2 to 15 cms.
   */
bool US_status(int ping, int echo)
{
  long duration,cm;

  bool obstacleDetected = false;

  digitalWrite(ping, LOW);
  delayMicroseconds(2);
  digitalWrite(ping, HIGH);
  delayMicroseconds(5);
  digitalWrite(ping, LOW);

  duration = pulseIn(echo, HIGH);
  
  cm = microsecondsToCentimeters(duration);

  if((cm < MAX_SAFE_DISTANCE) && (cm > MIN_SAFE_DISTANCE) )
     obstacleDetected = true;
  else
    obstacleDetected = false;
      
  return obstacleDetected;
}

  /* Check if ACK has been pressed by user. 
   * return TRUE if pressed else FALSE
   */
bool ackStatus()
{
  return ((digitalRead(ackPin)) == HIGH);     
}


  /* Check the final status of all ultra sonic sensors 
   * If any one sensor detected an object within 2 to 15 cms,
   * return true to STOP the robot.
   */
bool sensorStatus()
{
  if(sensorStatus1 || sensorStatus2 || sensorStatus3 || sensorStatus4 || sensorStatus5
    || sensorStatus6 || sensorStatus7 || sensorStatus8 || sensorStatus9 || sensorStatus10)
  {
    Serial.print("obstacle detected\n");
    digitalWrite(LED_BUILTIN, HIGH);

    return true;
  }
  else
  {
    Serial.print("No obstacle detected\n");
    digitalWrite(LED_BUILTIN, LOW);
    return false;
  }
}


void loop()
{

  /* Read the status of each Ultra Sonic Sensors
   * The return value is true if the sensor detects 
   * an object within 2 to 15 cms.
   */
  sensorStatus1 = US_status(pingPin_1,echoPin_1);
  sensorStatus2 = US_status(pingPin_2,echoPin_2);
  sensorStatus3 = US_status(pingPin_3,echoPin_3);
  sensorStatus4 = US_status(pingPin_4,echoPin_4);
  sensorStatus5 = US_status(pingPin_5,echoPin_5);
  sensorStatus6 = US_status(pingPin_6,echoPin_6);
  sensorStatus7 = US_status(pingPin_7,echoPin_7);
  /*sensorStatus8 = US_status(pingPin_8,echoPin_8);
  sensorStatus9 = US_status(pingPin_9,echoPin_9);
  sensorStatus10 = US_status(pingPin_10,echoPin_10);*/


  /* Check if ESTOP has been pressed. 
   * Once Pressed, the status remains true until RESET
   */
  if((digitalRead(estopPin)) == HIGH)
   estop_msg.data = true;


  /* Check the final status of all ultra sonic sensors 
   * If any one sensor detected an object within 2 to 15 cms,
   * return true to STOP the robot.
   */
  ultrasonic_msg.data = sensorStatus();


  /* Check if ACK has been pressed by user. 
   * return TRUE if pressed else FALSE
   */
  ack_msg.data = ackStatus();


  /*  Publish the ROS messages to ARIS
   *  The published messages are of type std::Bool which contains the 
   *  status of ACK switch, ESTOP switch and Ultra sonic sensors.
   */
  pub_us.publish(&ultrasonic_msg);   
  pub_ack.publish(&ack_msg);
  pub_estop.publish(&estop_msg);
  
  delay(100);
  nh.spinOnce();
}
