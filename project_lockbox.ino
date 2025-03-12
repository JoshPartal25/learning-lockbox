#define TRIGGERPIN 10 //this is the pin that sends out the pulse
#define ECHOPIN 9 //this is the pin that reads the distance

#define GREEN_PIN 2
#define YELLOW_PIN 3
#define RED_PIN 4

int red = 4;
int green = 3;
int blue = 2;



void setup() {
  // put your setup code here, to run once:

 Serial.begin(9600);
  pinMode(TRIGGERPIN, OUTPUT); //send pulse
  pinMode(ECHOPIN, INPUT); //read pulse
  pinMode(GREEN_PIN, OUTPUT); //sets the green pin as output
  pinMode(YELLOW_PIN, OUTPUT); //sets the yellow pin as output
  pinMode(RED_PIN, OUTPUT); //sets the red pin as output

}

void loop() {
  // put your main code here, to run repeatedly:

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
    Serial.println("Good to back up"); //let's user know that you can continue to move closer to sensor
    
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
  }
  
  if(distance < 10 && distance > 5){
    Serial.println("Getting close!");

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
