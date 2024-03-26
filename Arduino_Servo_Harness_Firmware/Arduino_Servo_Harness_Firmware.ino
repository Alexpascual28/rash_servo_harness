/*
  Arduino Servo Harness Firmware
  Mike Angus 2021/22

  Written to form a basis for the MSc Intelligent Robotics robot gripper project.
  Provides various basic functions which may be extended, modified or replaced as desired.
  There are additional breakout functions on the circuit board which are not implemented here,
  but this can easily be achieved using the Arduino documentation and the markings on the board for reference.
*/

#include <Servo.h>

//Pin numbers
#define INDICATOR 3
#define BUTTON1 A3
#define BUTTON2 A2
#define POT1 A7
#define POT2 A6
#define SERVO1 12
#define SERVO2 11
#define SERVO3 10
#define SERVO4 9
#define SERVO5 8
#define SERVO6 7

//Gripper state
#define OPEN 0
#define CLOSED 1

//Loop time. Main loop will sleep for this number of milliseconds every cycle.
//Note this is in addition to the time taken to execute the code, which may be substantial if there is a lot of serial communication
#define LOOP_TIME_MS 50

//Set servo positions for closed and open grippers - can determine these by calibration (see calibrate() function)
int openPositions[6] = {0, 0, 0, 0, 0, 0};
int closedPositions[6] = {180, 180, 180, 180, 180, 180};

//Heartbeat rate. Change this dynamically if you like to give an indication of what state your software is in. If it stops, your code is stuck somewhere.
//LED brightness will adjust by this increment every cycle.
int heartbeatRate = 20;

//Heartbeat state
bool heartbeatState = 1; //1 is increasing brightness, 0 is decreasing
int heartbeatValue = 0;

//Potentiometer readings, updated every loop
int pot1Reading;
int pot2Reading;
int pot1ReadingPrev;
int pot2ReadingPrev;

//Current state of the buttons and gripper
bool button1Pressed = false;
bool button2Pressed = false;
bool gripperState = OPEN;

//Array of servos and individual enable flags
Servo servos[6];
bool servoEnableFlags[6] = {1, 1, 1, 1, 1, 1};

//The setup function runs once when you press reset or power the board
void setup() {
  
  //Setup output pins
  pinMode(INDICATOR, OUTPUT);
  pinMode(SERVO1, OUTPUT);
  pinMode(SERVO2, OUTPUT);
  pinMode(SERVO3, OUTPUT);
  pinMode(SERVO4, OUTPUT);
  pinMode(SERVO5, OUTPUT);
  pinMode(SERVO6, OUTPUT);

  //Setup servo PWM signals
  servos[0].attach(SERVO1);
  servos[1].attach(SERVO2);
  servos[2].attach(SERVO3);
  servos[3].attach(SERVO4);
  servos[4].attach(SERVO5);
  servos[5].attach(SERVO6);

  //Start serial port at baudrate 115200
  Serial.begin(115200);
}

//The loop function repeats forever
void loop() {

  //Update potentiometer readings
  pot1ReadingPrev = pot1Reading;
  pot2ReadingPrev = pot2Reading; //Update prev values with readings taken last cycle
  pot1Reading = analogRead(POT1);
  pot2Reading = analogRead(POT2);

  //Calibration routine (example code). Copy/modify these lines to change which servos are calibrated and in which way
  //Printed output values can then be used in closedPositions and openPositions to set the desired exact range for each servo.
  //Comment out these lines when not needed.
  calibrate(1, POT1, CLOSED); //Calibrate Servo 1 CLOSED position using Pot 1
  calibrate(2, POT1, CLOSED); //Calibrate Servo 2 CLOSED position using Pot 1
  calibrate(3, POT1, CLOSED); //Calibrate Servo 3 CLOSED position using Pot 1
  calibrate(4, POT1, CLOSED); //Calibrate Servo 4 CLOSED position using Pot 1
  calibrate(1, POT2, OPEN); //Calibrate Servo 1 OPEN position using Pot 2
  calibrate(2, POT2, OPEN); //Calibrate Servo 2 OPEN position using Pot 2
  calibrate(3, POT2, OPEN); //Calibrate Servo 3 OPEN position using Pot 2
  calibrate(4, POT2, OPEN); //Calibrate Servo 4 OPEN position using Pot 2

  //Button 1 triggers gripper close
  if (digitalRead(BUTTON1) == LOW) { //Button has been pressed
    if(button1Pressed == false) { //Only trigger once
      button1Pressed = true;

      //Desired behaviour on button 1 press (can be changed)
      Serial.println("CLOSE");
      closeGripper();
    }
  } else { //Button has been released
    button1Pressed = false; 
  }

  //Button 2 triggers gripper open
  if (digitalRead(BUTTON2) == LOW) { //Button has been pressed
    if(button2Pressed == false) { //Only trigger once
      button2Pressed = true;

      //Desired behaviour on button 2 press (can be changed)
      Serial.println("OPEN");
      openGripper();
    }
  } else { //Button has been released
    button2Pressed = false;
  }

  //Update the heartbeat
  updateHeartbeat();

  //Wait for the specified loop delay time
  delay(LOOP_TIME_MS);
}

/**SETPOSITION
 * 
 * @brief Set the position of a single servo, if enabled.
 * @param servoId - The ID of the servo to be moved, from 1 to 6
 * @param position - The target position in degrees from 0 to 180
 */
void setPosition(int servoId, int position) {
  if (servoEnableFlags[servoId-1] == true) {
    servos[servoId-1].write(position);
  }
}

/**CLOSEGRIPPER
 * 
 * @brief Set all enabled servos to 'closed' position
 */
void closeGripper() {
  for (int i=0; i<6; ++i) {
    setPosition(i+1, closedPositions[i]); //Set each servo to its closed position
    gripperState = CLOSED;
  }
}

/**OPENGRIPPER
 * 
 * @brief Set all enabled servos to 'open' position
 */
void openGripper() {
  for (int i=0; i<6; ++i) {
    setPosition(i+1, openPositions[i]); //Set each servo to its open position
    gripperState = OPEN;
  }
}

/**CALIBRATE
 * 
 * @brief Use an onboard potentiometer to calibrate the open or closed position of a gripper servo
 * @param servoId - The ID of the servo to be calibrated, from 1 to 6
 * @param pot - The potentiometer to use for calibrating, either POT1 or POT2
 * @param targetState - The gripper position to be calibrated, either OPEN or CLOSED
 */
void calibrate(int servoId, int pot, bool targetState) {

  //Flag to determine if anything needs updating (avoids excessive updates)
  bool updateRequired = false;

  //Generate target angle from potentiometer reading. Using prevTargetAngle to determine if servo position actually changes.
  float targetAngle, prevTargetAngle;
  if (pot == POT1) {  //Tuning with pot 1
    targetAngle = pot1Reading * 180.0f/1023;
    prevTargetAngle = pot1ReadingPrev * 180.0f/1023;
  } else {  //Tuning with pot 2
    targetAngle = pot2Reading * 180.0f/1023;
    prevTargetAngle = pot2ReadingPrev * 180.0f/1023;
  }

  //Determine if target angle has changed since the last reading
  if ((int)targetAngle != (int)prevTargetAngle) {
    updateRequired = true;
  }

  //Only do anything else if there is a new reading to be implemented
  if (updateRequired) {
    //Update the set position accordingly
    if (targetState == OPEN) {
      openPositions[servoId-1] = (int)targetAngle;
    } else {
      closedPositions[servoId-1] = (int)targetAngle;
    }

    //Update servo position if gripper is currently in the state being calibrated
    if (gripperState == targetState) {
      if (targetState == OPEN) {
        setPosition(servoId, openPositions[servoId-1]);
      } else {
        setPosition(servoId, closedPositions[servoId-1]);
      }
    }

    //Display the new setting in the terminal
    Serial.print("Servo ");
    Serial.print(servoId);
    if (targetState == OPEN) {
      Serial.print(" open ");
    } else {
      Serial.print(" closed ");
    }
    Serial.print("position set to: ");
    Serial.println(targetAngle, 0); //Output integer value
  }
}

/**UPDATEHEARTBEAT
 * 
 * @brief Updates the indicator LED heartbeat. Call this function each cycle to implement.
 */
void updateHeartbeat(void) {

  //Increase or decrease the brightness setting
  if (heartbeatState == 1) {
    heartbeatValue += heartbeatRate;
    if (heartbeatValue > 255) {
      heartbeatValue = 255; //Limit to max
      heartbeatState = 0; //Reverse direction if faded all the way up
    }
  } else {
    heartbeatValue -= heartbeatRate;
    if (heartbeatValue < 0) {
      heartbeatValue = 0; //Limit to min
      heartbeatState = 1; //Reverse direction if faded all the way dowm
    }
  }

  //Write the brightness setting to the LED
  analogWrite(INDICATOR, heartbeatValue);
}
