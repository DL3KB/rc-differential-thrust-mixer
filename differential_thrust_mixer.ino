#include <Servo.h>

/* Begin configuration settings */
const int min_command = 1000; // The minimum signal to be sent to the ESC
const int max_command = 2000; // The maximum signal to be sent to the ESC

const byte throttle_pin = 2; // The pin that the throttle input signal is plugged into
const byte yaw_pin = 3; // yaw input pin
const byte l_motor_pin = 9; // Left motor output pin
const byte r_motor_pin = 10; // Right motor output pin

const int reverse_yaw = -1; // 1 or -1 to reverse the yaw channel. 
const int yaw_deadband = 20; // ignore this much jitter on yaw channel
const float mix_strength = 0.5; // value from 0 (no mix) to 1 (100%) for the mix strength. May eventually set this value from another switch/channel on the receiver



int throttle, yaw = 1000;
float yaw_pct, modifyer = 0;

Servo l_motor, r_motor;


void setup() {
  // put your setup code here, to run once:

  pinMode(throttle_pin, INPUT);
  pinMode(yaw_pin, INPUT);

  l_motor.attach(l_motor_pin);
  r_motor.attach(r_motor_pin);




  
  Serial.begin(115200);
}

void loop() {
  framecounter++;
  // put your main code here, to run repeatedly:

  // read input pwm from throttle and yaw
  throttle = pulseIn(throttle_pin, HIGH);
  yaw = pulseIn(yaw_pin,HIGH);

  
  if(yaw > 1500 + yaw_deadband || yaw < 1500 - yaw_deadband){
    yaw_pct = ((yaw-1500)/(float)500) * reverse_yaw;
  }else{
    yaw_pct = 0;
  }
  
  // calculate transformation
  modifyer = abs(yaw_pct) * (throttle - 1000) * mix_strength;
//  Serial.println("yaw pct"+(String) yaw_pct);
  
  // calculate & write left and right motor mixed throttle/yaw values
  if(yaw_pct < 0){
    l_motor.writeMicroseconds((throttle - modifyer) < min_command ? min_command : (throttle - modifyer));
    r_motor.writeMicroseconds((throttle + modifyer) > max_command ? max_command : (throttle + modifyer));
//    Serial.println("yaw left: "+ (String) (modifyer));
  }else{
    l_motor.writeMicroseconds((throttle + modifyer) > max_command ? max_command : (throttle + modifyer));
    r_motor.writeMicroseconds((throttle - modifyer) < min_command ? min_command : (throttle - modifyer));
//    Serial.println("yaw right: "+ (String) (modifyer));
  }

  String t;
  Serial.println("T: "+(String) throttle + "  Y: "+ (String) yaw + "  LM: " + (String) l_motor.readMicroseconds() + "  RM: "+ r_motor.readMicroseconds());



}
