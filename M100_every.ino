#include <stdint.h>
#include <stdlib.h>  
#include <ros.h>
#include <std_msgs/Float32.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Vector3Stamped.h>


const int led_pin = 13;
const int rele_pin = 7;



ros::NodeHandle  nh;

geometry_msgs::Vector3Stamped joy_msg;
ros::Publisher pub_current( "/M100_current", &joy_msg);


void joydata ( const sensor_msgs::Joy& joy){


  if ((joy.axes[4]) <= 0 ){
     digitalWrite(led_pin, HIGH);
     digitalWrite(rele_pin, HIGH);

  }else{ // regresa a P
    digitalWrite(led_pin, LOW);
    digitalWrite(rele_pin, HIGH);
    
  }  

}

ros::Subscriber<sensor_msgs::Joy> sub1("/dji_sdk/rc", joydata);    

void setup()
{
    pinMode(led_pin, OUTPUT);
    nh.initNode();
    nh.subscribe(sub1);
    nh.advertise(pub_current);
}

long range_time;

void loop()  {
    //publish the adc value every 50 milliseconds
  //since it takes that long for the sensor to stablize


  float sensorValue1 = analogRead(A0);
  float sensorValue2 = analogRead(A1);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage1 = sensorValue1 * (5.0 / 1023.0);
  float voltage2 = sensorValue2 * (5.0 / 1023.0);


  float corriente1 = (float) 1000*(voltage1 - 2.5) / 0.066;
  float corriente2 = (float) 1000*(voltage2 - 2.5) / 0.066;

  if ( millis() >= range_time ){
 

    //joy_msg.axes = [0 0 0 0];
    joy_msg.vector.x = voltage1;
    joy_msg.vector.y = voltage2;
    joy_msg.vector.z = range_time;
    
    joy_msg.header.stamp = nh.now();
    pub_current.publish(&joy_msg);
    range_time =  millis() + 50;
  }
  

 nh.spinOnce();
 delay(1);
}