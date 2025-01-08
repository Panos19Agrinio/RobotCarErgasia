#include "Ultrasonic.h"
#include <Servo.h>

//Constants
const int motorR1 = 6;
const int motorR2 = 5;
const int motorL1 = 11;
const int motorL2 = 10;
const int servo = 3;
const int rightSW = 8;
const int leftSW = 4;
const int led 	 = 13;
const int buzzer = 12;

Ultrasonic ultrasonic(A0,A1); // Trig and Echo
Servo myservo;  			  

//Variables
int distance=100;
int rightDistance;
int leftDistance;
int rightSWState;
int leftSWState;
int pos = 90; // for servo, 90 deg looking forward

void setup()
{
	//Define inputs and outputs
	pinMode(motorR1, OUTPUT);
	pinMode(motorR2, OUTPUT);
	pinMode(motorL1, OUTPUT);
	pinMode(motorL2, OUTPUT);
	pinMode(leftSW, INPUT_PULLUP);
	pinMode(rightSW, INPUT_PULLUP);
  pinMode(led, 	OUTPUT);
	pinMode(buzzer, OUTPUT);
	
	//Init servo and starting position
	myservo.attach(servo); 
	myservo.write(pos); 
	
	//Pause for 2 seconds before starting
	delay(2000);
}

void loop()
{
	distance = ultrasonic.Ranging(CM);
	
	//If an obstacle is detected at 10cm, stop the car and maneuver.
	if (distance <= 10){
		stop();
    tone(buzzer,500); // play a tone
		digitalWrite(led,HIGH); // turn the led on
		//Look right side
		for (pos = 0; pos <= 180; pos += 1) { // go from 0 to 180 degrees
    		myservo.write(pos);               // tell servo go to position in variable 'pos'
    		delay(5);                         // wait 5ms for the servo to reach the position
  		}
  		//Read the new distance from the right side
  		leftDistance = ultrasonic.Ranging(CM);
  		delay(50);
  		//Look left side
  		for (pos = 180; pos >= 0; pos -= 1) { // go from 180 to 0 degrees
    		myservo.write(pos);               // tell servo go to position in variable 'pos'
    		delay(5);                         // wait 5ms for the servo to reach the position
  		}
  		//Read the new distance from the left side
  		rightDistance = ultrasonic.Ranging(CM);
  		delay(50);
  		pos = 90; // look forward again
  		myservo.write(pos);
  		
  		//Compare left and right distances and make the best turn decision
  		if (leftDistance > rightDistance){
        backward();
        delay(200);
  			left();
  			delay(300); // In this setup this time corresponds to approx. 90 deg. turn
  		}
  		else if (leftDistance < rightDistance){
        backward();
        delay(200);
  			right();
  			delay(300); // In this setup this time corresponds to approx. 90 deg. turn
  		}
  		else{ //when two distances are exactly equal (rare scenario)
  			backward();
  			delay(500);// Reverse for half a second 
  			right(); 	// and turn slightly to the right in order to reorientate
  			delay(100);
  		}
  		
	}
	//Check frontal contact switches and if clear move forward
	else{
		
		leftSWState = digitalRead(leftSW);
		rightSWState = digitalRead(rightSW);
		
		//Check for contact and if contact is detected move backwards and turn in order to avoid the obstacle!
		if (leftSWState == LOW){
			tone(buzzer,1000); // play a tone
      digitalWrite(led,HIGH); // turn the led on
      backward();
			delay(500);
			right();
			delay(250);
		}
		if (rightSWState == LOW){
      tone(buzzer,1000); // play a tone
      digitalWrite(led,HIGH); // turn the led on
			backward();
			delay(500);
			left();
			delay(250);
		}
		
		forward();
    noTone(buzzer);
		digitalWrite(led,LOW);
	}
	
}

//Movement and Stop functions

void backward(){
	digitalWrite(motorR1, HIGH);
	digitalWrite(motorR2, LOW);
	digitalWrite(motorL1, HIGH);
	digitalWrite(motorL2, LOW);
}

void forward(){
	digitalWrite(motorR1, LOW);
	digitalWrite(motorR2, HIGH);
	digitalWrite(motorL1, LOW);
	digitalWrite(motorL2, HIGH);
}

void left(){
	digitalWrite(motorR1, LOW);
	digitalWrite(motorR2, HIGH);
	digitalWrite(motorL1, HIGH);
	digitalWrite(motorL2, LOW);
}

void right(){
	digitalWrite(motorR1, HIGH);
	digitalWrite(motorR2, LOW);
	digitalWrite(motorL1, LOW);
	digitalWrite(motorL2, HIGH);
}

void stop(){
	digitalWrite(motorR1, LOW);
	digitalWrite(motorR2, LOW);
	digitalWrite(motorL1, LOW);
	digitalWrite(motorL2, LOW);
}