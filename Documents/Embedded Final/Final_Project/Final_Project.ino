//Erica Chou
//5/17/20
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;


//Globals

//Assuming user does not have any tilt
float pastX = 9.81;
float pastY = 0;
float pastZ = 0;

//we're going to switch to different states
//state 0: initial state, we have not reccorded a movement input yet. If user presses button, it'll record input and move to state 1
//state 1: a pattern has been recorded and we must now await the user to try and input the recorded password. We will stay in this state until we fail or succeed
//state 2: User has Failed, and we indicate to user we have failed. We then move to state 3 (Succeed takes place in state 1 but we skip state 2 in that case)
//state 3: We have succeeded, and an LED will blink to indicate you have correctly inputted the movements

int state = 0;

float threshold = 3;
enum Direction {NONE, UP, DOWN, LEFT, RIGHT, FORWARDS, BACKWARDS};
Direction dirArrayRec[4] = {NONE, NONE, NONE, NONE};
Direction dirArrayCheck[4] = {NONE, NONE, NONE, NONE};
int arrayIndexMax = 0;
int arrayIndex = 0;


//BUTTON VALUES
int Button = 4; //button pin
int buttonValue = 0; //button input value

void setup ()
{
  
  //for the LED connected to the Arduino. Tells us when Arduino is recording movements for code vs comparing
  unsigned char * portDDRB ;
  portDDRB = (unsigned char *) 0x24;
  *portDDRB |= 0x20;

  unsigned char * portB ;
  portB = (unsigned char *) 0x25;
  *portB |= 0x20;


  //Button
  pinMode(Button, INPUT);

  //LEDs
  pinMode(5, OUTPUT); //LED1
  pinMode(10, OUTPUT); //LED2
  pinMode(3, OUTPUT); //LED 3

  //Accelerometer set up
  Serial.begin(9600);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println("+- 2000 deg/s");
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
  }

   //Calibrate 
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  pastX = a.acceleration.x;
  pastY = a.acceleration.y;
  pastZ = a.acceleration.z;

  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");
  
  Serial.println("");
  delay(100);
}

void loop ()
{

  buttonValue = digitalRead(Button);
  float diffX = 0;
  float diffY = 0;
  float diffZ = 0;
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);


  diffX = abs((a.acceleration.x) - (pastX));
  diffY = abs(a.acceleration.y - pastY);
  diffZ = abs(a.acceleration.z - pastZ);

  if (buttonValue > 0 && arrayIndex < 4 && state == 0) {
    //RECORD INPUTS AS PASSCODE
    Serial.print("STATE 0: ");

    Serial.print("Acceleration X: ");
    Serial.print(a.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
    Serial.println(" m/s^2");

    unsigned char * portB ;
    portB = (unsigned char *) 0x25;
    *portB ^= 0x20; //toggle to show chip is recording
    
    digitalWrite(5, HIGH); //Tells us we are reading inputs
    if (diffX >= diffY && diffX >= diffZ) {
      if (diffX >= threshold) {
        digitalWrite(10, LOW);
        digitalWrite(3, HIGH);
        if (a.acceleration.x > pastX) {
          digitalWrite(5, HIGH);
          Serial.println("UP");
          dirArrayRec[arrayIndex] = UP;
          arrayIndex++;
        }

        else {
          digitalWrite(5, LOW);
          Serial.println("DOWN");
          dirArrayRec[arrayIndex] = DOWN;
          arrayIndex++;
        }
      }
    }
    if (diffY >= diffX && diffY >= diffZ) {
      if (diffY >= threshold) {
        digitalWrite(3, LOW);
        digitalWrite(10, HIGH);
        if (a.acceleration.y > pastY) {
          digitalWrite(5, HIGH);
          Serial.println("RIGHT");
          dirArrayRec[arrayIndex] = RIGHT;
          arrayIndex++;
        }

        else {
          digitalWrite(5, LOW);
          Serial.println("LEFT");
          dirArrayRec[arrayIndex] = LEFT;
          arrayIndex++;
        }
      }
    }

    if (diffZ >= diffX && diffZ >= diffY) {
      if (diffZ >= threshold) {
        digitalWrite(3, HIGH);
        digitalWrite(10, HIGH);
        if (a.acceleration.z > pastZ) {
          digitalWrite(5, HIGH);
          Serial.println("FORWARDS");
          dirArrayRec[arrayIndex] = FORWARDS;
          arrayIndex++;
        }

        else {
          digitalWrite(5, LOW);
          Serial.println("BACKWARDS");
          dirArrayRec[arrayIndex] = BACKWARDS;
          arrayIndex++;
        }
      }
    }
    if (diffX <= threshold && diffY <= threshold && diffZ <= threshold ) {
      digitalWrite(5, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(10, LOW);
      Serial.println("NO CHANGE");
    }

    Serial.println("");
    delay(400);
  }

  else if (buttonValue > 0 && state == 1 && arrayIndex < arrayIndexMax) {
    //RECORD INPUTS AND COMPARE TO PASSCODE
    Serial.print("STATE 1: ");

    Serial.print("Acceleration X: ");
    Serial.print(a.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
    Serial.println(" m/s^2");

    unsigned char * portB ;
    portB = (unsigned char *) 0x25;
    *portB |= 0x20; 

    
    digitalWrite(5, HIGH); //Tells us we are in state 1
    if (diffX >= diffY && diffX >= diffZ) {
      if (diffX >= threshold) {
        digitalWrite(10, LOW);
        digitalWrite(3, HIGH);
        if (a.acceleration.x > pastX) {
          digitalWrite(5, HIGH);
          Serial.println("UP");
          dirArrayCheck[arrayIndex] = UP;
        }

        else {
          digitalWrite(5, LOW);
          Serial.println("DOWN");
          dirArrayCheck[arrayIndex] = DOWN;
        }
        if (dirArrayCheck[arrayIndex] != dirArrayRec[arrayIndex]) {
          state = 2 ;
        }
        
        arrayIndex++;
      }
    }
    if (diffY >= diffX && diffY >= diffZ) {
      if (diffY >= threshold) {
        digitalWrite(3, LOW);
        digitalWrite(10, HIGH);
        if (a.acceleration.y > pastY) {
          digitalWrite(5, HIGH);
          Serial.println("RIGHT");
          dirArrayCheck[arrayIndex] = RIGHT;
        }

        else {
          digitalWrite(5, LOW);
          Serial.println("LEFT");
          dirArrayCheck[arrayIndex] = LEFT;
        }
        if (dirArrayCheck[arrayIndex] != dirArrayRec[arrayIndex]) {
          state = 2 ;
        }
     
        arrayIndex++;
      
      }
    }

    if (diffZ >= diffX && diffZ >= diffY) {
      if (diffZ >= threshold) {
        digitalWrite(3, HIGH);
        digitalWrite(10, HIGH);
        if (a.acceleration.z > pastZ) {
          digitalWrite(5, HIGH);
          Serial.println("FORWARDS");
          dirArrayCheck[arrayIndex] = FORWARDS;
        }

        else {
          digitalWrite(5, LOW);
          Serial.println("BACKWARDS");
          dirArrayCheck[arrayIndex] = BACKWARDS;
        }
        if (dirArrayCheck[arrayIndex] != dirArrayRec[arrayIndex]) {
          state = 2 ;
        }
        
        arrayIndex++;
       
      }
    }
    if (diffX <= threshold && diffY <= threshold && diffZ <= threshold ) {
      digitalWrite(5, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(10, LOW);
      Serial.println("NO CHANGE");
    }

    if (state == 2) {
      Serial.println("FAIL! YOU FAILED AT INDEX:");
      Serial.println(arrayIndex);
    }

    Serial.println("");
    delay(400);
  }

  else if (state == 1 && arrayIndexMax <= arrayIndex) { //It means we succeeded
    Serial.println("SUCCEED!");
    digitalWrite(5, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(10, LOW);
    state = 3;
    delay(1000);

  }

   else if (state == 2 ) { //It means we Failed
    Serial.println("FAIL!");
    digitalWrite(3, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(5, LOW);
    state = 3;
    delay(1000);

  }
  
  else {
    if (buttonValue == 0 && state >= 2) {
      //We might be forced into this state again while someone is holding the button
      state = 1;
    }
    if (arrayIndex != 0 ) {
      Serial.println("DIRECTIONS RECORDED: ");
      for (int i = 0; i < arrayIndex; i++) {
        Serial.println("DIRECTION # RECCORDED");
        Serial.println(i);
        Serial.println(": ");
        switch (dirArrayRec[i]) {
          case UP:
            Serial.println("UP");
            break;
          case DOWN:
            Serial.println("DOWN");
            break;
          case FORWARDS:
            Serial.println("FORWARDS");
            break;
          case BACKWARDS:
            Serial.println("BACKWARDS");
            break;
          case LEFT:
            Serial.println("LEFT");
            break;
          case RIGHT:
            Serial.println("RIGHT");
            break;
        }
      }
      for (int j = 0; j < arrayIndex; j++) {
        Serial.println("DIRECTION # CHECK");
        Serial.println(j);
        Serial.println(": ");
        switch (dirArrayCheck[j]) {
          case UP:
            Serial.println("UP");
            break;
          case DOWN:
            Serial.println("DOWN");
            break;
          case FORWARDS:
            Serial.println("FORWARDS");
            break;
          case BACKWARDS:
            Serial.println("BACKWARDS");
            break;
          case LEFT:
            Serial.println("LEFT");
            break;
          case RIGHT:
            Serial.println("RIGHT");
            break;
        }
        //RESTARTING CHECK GOING TO PUT EVERYTHING AS NONE AGAIN
        dirArrayCheck[j] = NONE;
      }
      if (state == 0) {
        arrayIndexMax = arrayIndex;
        state = 1;
      }
      arrayIndex = 0;
    }
    unsigned char * portB ;
    portB = (unsigned char *) 0x25;
    *portB |= 0x20;
    //THIS IS IDLE. YOU CAN ONLY HOLD THE BUTTON AFTER THE LED FLASHES GREEN 
    digitalWrite(5, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(3, LOW);
    delay(500);
    digitalWrite(10, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(3, LOW);
    delay(500);
    digitalWrite(3, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(10, LOW);
    delay(500);
  }
}
