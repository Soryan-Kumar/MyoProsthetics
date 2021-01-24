#include <Wire.h>
#include <Adafruit_MotorShield.h>

float thresh = 3;
int node = A0;
int weight_node = A3;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *Motor = AFMS.getMotor(3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //muscle contraction detection
  Serial.println(analogRead(node)* (5.0 / 1023.0));

  AFMS.begin();  // create with the default frequency 1.6KHz

  Motor->setSpeed(0);

}

void loop() {
  // put your main code here, to run repeatedly:

  //always start at rest
  Motor->setSpeed(0);
  float volt = analogRead(node)* (5.0 / 1023.0);
  //float volt = 4;
  Serial.println(volt);

  //capture action potential
  if (volt >= thresh) {
    //Serial.println("DETECTED");
    //compute weight factor
    float weight = analogRead(weight_node)* (5.0 / 1023.0);
    Motor->setSpeed(90*weight*1.2);
    Motor->run(BACKWARD);
    delay(300);
    
    //check whether to stay or start falling
    float volt2 = analogRead(node)* (5.0 / 1023.0);
    Serial.println(volt2);
    while (volt2 >= thresh) {
      //Serial.println("HOLDING");
      Motor->setSpeed(0);
      delay(2000);
      volt2 = analogRead(node)* (5.0 / 1023.0);
      Serial.println(volt2);   
    }

    //Serial.println("RELAXING");

    //WITH ARM ATTACHMENT
    weight = analogRead(weight_node)* (5.0 / 1023.0);
    Motor->setSpeed(70*weight*1.2);
    Motor->run(FORWARD);
    delay(600);
    
  }

  Motor->setSpeed(0);
  delay(1000);

  


}
