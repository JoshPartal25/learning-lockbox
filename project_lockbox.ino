//LED library
#define GREEN_PIN 2   //this is the pin for the green LED
#define YELLOW_PIN 3  //this is the pin for the yellow LED
#define RED_PIN 4     //this is the pin for the red LED
#include <SoftwareSerial.h>

int red = 4;
int green = 2;
int yellow = 3;


//rfid library
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN 9  // Configurable, see typical pin layout above
#define SS_PIN 10  // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance


//bluetooth library
int const RX_PIN = 7;  //this is the rx pin, which receives the bluetooth
int const TX_PIN = 8;  //this is the tx pin, which transmits the bluetooth

SoftwareSerial tooth(TX_PIN, RX_PIN);  //makes a bluetooth object
//set tx and rx pins
//tx goes first, then rx
char davis;


//ultrasonic sensor library
#define TRIGGERPIN 5  //this is the pin that sends out the pulse
#define ECHOPIN 6     //this is the pin that reads the distance


//servo motor library
#include <Servo.h>
#define MICHAEL_PIN A5  //this is the pin that connects to servo motor

Servo michael;
//make an rfid string to hold the id

String scannedID = "";

void setup() {


  Serial.begin(9600);
  tooth.begin(9600);


  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(TRIGGERPIN, OUTPUT);  //send pulse
  pinMode(ECHOPIN, INPUT);      //read pulse
  pinMode(GREEN_PIN, OUTPUT);   //sets the green pin as output
  pinMode(YELLOW_PIN, OUTPUT);  //sets the yellow pin as output
  pinMode(RED_PIN, OUTPUT);     //sets the red pin as output


  // servo motor setup code
  michael.write(0);
  michael.attach(MICHAEL_PIN);  //connecting the servo object to the pin


  //rfid setup code
  Serial.begin(9600);  // Initialize serial communications with the PC
  while (!Serial)
    ;                                 // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();                        // Init SPI bus
  mfrc522.PCD_Init();                 // Init MFRC522
  delay(4);                           // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {



  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
    Serial.println("Reading the Card");
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
    Serial.println("Reading the Card");
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);



  Serial.print(F("RFID Tag UID:"));

  printHex(mfrc522.uid.uidByte, mfrc522.uid.size);

  Serial.println("");
  mfrc522.PICC_HaltA();  // Halt PICC

  //if stament that checks rfid id is correct (79 34 2D 03)then does something
  Serial.println(scannedID);
  if (scannedID.startsWith(" 79 34 2d 03")) {
    Serial.println("suce4ss!");

    delay(2000);
    //bluetooth loop code inside of rfid, makes it so only bluetooth works and ultrasonic work if rfid tag is scanned
    //bluetooh main code
    //bluetooth#: 51568
    if (tooth.available() > 0) {
      Serial.println("did it work");
      davis = tooth.read();
      tooth.print("reading new input: ");
      tooth.println(davis);
    }

    if (davis == 'u') {  //turns on ultrasonic sensor
    delay(100);
    Serial.println("sending signal");
      // ultrasonic sensor main code
      digitalWrite(TRIGGERPIN, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIGGERPIN, HIGH);
      delayMicroseconds(10);

      float durration = pulseIn(ECHOPIN, HIGH);  //tell us the time from pulse send to pulse received
      Serial.println(durration);

      //distance = speed * durration
      float speed = 0.034;  //measured in cm/microseconds
      float distance = speed * durration / 2;

      Serial.print("distance");
      Serial.println(distance);
      delay(100);

      if (distance > 10) {
        Serial.println("Good to back up");  //tells user that you can continue to move closer to sensor

        digitalWrite(GREEN_PIN, HIGH);  //physical green LED will flash letting user know to continue to move closer to sensor
        delay(100);
        digitalWrite(GREEN_PIN, LOW);
        delay(100);
        digitalWrite(GREEN_PIN, HIGH);
        delay(100);
        digitalWrite(GREEN_PIN, LOW);
        delay(100);

        digitalWrite(YELLOW_PIN, LOW);  //only green LED on here
        digitalWrite(RED_PIN, LOW);     //only green LED on here
        delay(200);

        michael.write(0);     //start of motor extension at 0 degrees
        michael.write(-250);  //continuous full rotations until told otherwise
        delay(1500);          //wait 0.75 seconds

      } else if (distance < 10 && distance > 5) {
        Serial.println("Getting close!");  //tells use that you can continue to move closer with caution

        digitalWrite(YELLOW_PIN, HIGH);  //physical yellow LED will flash letting user know that they are getting closer and closer to the sensor
        delay(100);
        digitalWrite(YELLOW_PIN, LOW);
        delay(100);
        digitalWrite(YELLOW_PIN, HIGH);
        delay(100);
        digitalWrite(YELLOW_PIN, LOW);
        delay(100);

        digitalWrite(GREEN_PIN, LOW);  //only yellow LED on here
        digitalWrite(RED_PIN, LOW);    //only yellow LED on here
        delay(200);

        //servo motor moves slightly to start opening up the lock
        michael.write(100);  //start of motor extension at 0 degrees
        michael.write(180);  //continuous full rotations until told otherwise
        delay(750);          //wait 0.75 seconds
      } else if (distance < 5) {
        Serial.println("Ahh! Stop!");

        digitalWrite(RED_PIN, HIGH);  //physical red LED will light up letting user know that they are extremely close to the sensor
        delay(100);
        digitalWrite(RED_PIN, LOW);
        delay(100);
        digitalWrite(RED_PIN, HIGH);
        delay(100);
        digitalWrite(RED_PIN, LOW);
        delay(100);

        digitalWrite(GREEN_PIN, LOW);   //only red LED on here
        digitalWrite(YELLOW_PIN, LOW);  //only red LED on here
        delay(200);

        //servo motor moves more to completely open up the lock
        michael.write(180);  //start of motor extension at 0 degrees
        michael.write(260);  //continuous full rotations until told otherwise
        delay(750);          //wait 0.75 seconds
      }
    }
  }
}
void printHex(byte *buffer, byte bufferSize) {
  scannedID = "";


  //Serial.begin("reading?");

  for (byte i = 0; i < bufferSize; i++) {

    Serial.print(buffer[i] < 0x10 ? " 0" : " ");

    scannedID = scannedID + String(buffer[i] < 0x10 ? " 0" : " ");

    Serial.print(buffer[i], HEX);

    scannedID = scannedID + String(buffer[i], HEX);
  }
}
