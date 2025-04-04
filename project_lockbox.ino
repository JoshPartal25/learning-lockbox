//LED library
#define GREEN_PIN 2 //this is the pin for the green LED
#define YELLOW_PIN 3 //this is the pin for the yellow LED
#define RED_PIN 4 //this is the pin for the red LED

int red = 4;
int green = 3;
int blue = 2;


//ultrasonic sensor library
#define TRIGGERPIN 5 //this is the pin that sends out the pulse
#define ECHOPIN 6 //this is the pin that reads the distance


//servo motor library
#include <Servo.h>
#define MICHAEL_PIN A5 //this is the pin that connects to servo motor






void setup() {
  
// put your setup code here, to run once:
 Serial.begin(9600);
  pinMode(TRIGGERPIN, OUTPUT); //send pulse
  pinMode(ECHOPIN, INPUT); //read pulse
  pinMode(GREEN_PIN, OUTPUT); //sets the green pin as output
  pinMode(YELLOW_PIN, OUTPUT); //sets the yellow pin as output
  pinMode(RED_PIN, OUTPUT); //sets the red pin as output


// servo motor setup code
  michael.attach(MICHAEL_PIN); //connecting the servo object to the pin



}

void loop() {
  
  // ultrasonic sensor main code
  digitalWrite(TRIGGERPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGERPIN, HIGH);
  delayMicroseconds(10);

  float durration = pulseIn(ECHOPIN, HIGH); //tell us the time from pulse send to pulse received
  Serial.println(durration);

  //distance = speed * durration
  float speed = 0.034; //measured in cm/microseconds
  float distance = speed * durration /2;

  Serial.print("distance");
  Serial.println(distance);
  delay(100);

  if(distance > 10){
    Serial.println("Good to back up"); //tells user that you can continue to move closer to sensor
    
    digitalWrite(GREEN_PIN, HIGH); //physical green LED will flash letting user know to continue to move closer to sensor
    delay(100);
    digitalWrite(GREEN_PIN, LOW);
    delay(100);
    digitalWrite(GREEN_PIN, HIGH); 
    delay(100);
    digitalWrite(GREEN_PIN, LOW);
    delay(100);

    digitalWrite(YELLOW_PIN, LOW); //only green LED on here
    digitalWrite(RED_PIN, LOW); //only green LED on here
    delay(200);

    //servo motor moves slightly to start opening up the lock
    michael.write(100); //start of motor extension at 0 degrees
    michael.write(180); //continuous full rotations until told otherwise
    delay(1000); //wait 1 seconds, because 1000 equates to 1 second
    michael.write(100); //start of motor extension at 0 degrees
    delay(1000); //wait 1 seconds, because 1000 equates to 1 second
    michael.write(180); //continuous full rotations until told otherwise
    delay(1000); //wait 1 seconds, because 1000 equates to 1 second

    
  }
  
  if(distance < 10 && distance > 5){
    Serial.println("Getting close!"); //tells use that you can continue to move closer with caution

    digitalWrite(YELLOW_PIN, HIGH); //physical yellow LED will flash letting user know that they are getting closer and closer to the sensor
    delay(100);
    digitalWrite(YELLOW_PIN, LOW);
    delay(100);
    digitalWrite(YELLOW_PIN, HIGH); 
    delay(100);
    digitalWrite(YELLOW_PIN, LOW);
    delay(100);

    digitalWrite(GREEN_PIN, LOW); //only yellow LED on here
    digitalWrite(RED_PIN, LOW); //only yellow LED on here
    delay(200);
  }

  if(distance < 5){
    Serial.println("Ahh! Stop!");

    digitalWrite(RED_PIN, HIGH); //physical red LED will light up letting user know that they are extremely close to the sensor
    delay(100);
    digitalWrite(RED_PIN, LOW);
    delay(100);
    digitalWrite(RED_PIN, HIGH); 
    delay(100);
    digitalWrite(RED_PIN, LOW);
    delay(100);

    digitalWrite(GREEN_PIN, LOW); //only red LED on here
    digitalWrite(YELLOW_PIN, LOW); //only red LED on here
    delay(200);
  }



}
