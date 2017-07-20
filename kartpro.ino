/**************************************************************
 Course:        Purdue University ME-475 Senior Design
 Project Title: KartPRO
 Team:          MOSIAC
 Description:   This C performs the main following function 
                of the pixy
                Code Tailored for Arduino UNO
 Last Modified: April 22, 2017
 **************************************************************/

#include <SPI.h>  
#include "Pixy.h"

// Declarations
Pixy pixy;
uint16_t x = 0;         // Saves returned x coordinate from getBlocks()
int pixelsWidth;   //read by the camera
int pixelsHeight; //read by the camera
uint16_t angle = 0;
uint16_t area = 0;
int sig = 0;            // Returns Signature if the Object

static int k = 0;
static int detection = 0;

const int calWidth = 101; //Calibrated width reading
const int calHeight = 105; //Calibrated height reading
const int calDistance = 13.5; //in inches 24inches or 2 foot
float distanceWidth;   //calculated distance based on the width of the object
float distanceHeight;  //calculated distance based on the height of the object 
float widthOfObject = 5.375; //inches ACTUAL SIZE OF OBJECT
float heightOfObject = 5.75; //inches ACTUAL SIZE OF OBJECT
int focalLengthWidth;  //calculated focal length for width
int focalLengthHeight; //calculated focal length for height
int avg;
int feet;
int inches;

// Minimum Distance Threshold
int minDist = 18;
int minX = 120;
int maxX = 250;

// Motor 1
int dir1PinA = 2;
int dir2PinA = 3;
int speedPinA = 9; // Needs to be a PWM pin to be able to control motor speed

// Motor 2
int dir1PinB = 4;
int dir2PinB = 5;
int speedPinB = 10; // Needs to be a PWM pin to be able to control motor spee

int speed; // Local variable

void setup()
{
  Serial.begin(9600);
  Serial.print("Starting...\n");
  pixy.init();          // Initialize Pixy

  // Calculate focal length
  focalLengthWidth = (calWidth * calDistance) / widthOfObject;
  focalLengthHeight = (calHeight * calDistance) / heightOfObject;

  //Define L298N Dual H-Bridge Motor Controller Pins
  pinMode(dir1PinA,OUTPUT);
  pinMode(dir2PinA,OUTPUT);
  pinMode(speedPinA,OUTPUT);
  pinMode(dir1PinB,OUTPUT);
  pinMode(dir2PinB,OUTPUT);
  pinMode(speedPinB,OUTPUT);

}

void loop()
{ 
  /*
  while(millis()<5000) {
    uint16_t blocks = pixy.getBlocks();
    getObjectData(blocks);
    minX = x - (pixelsWidth/2);
    maxX = x + (pixelsWidth/2);
    minDist = getDistance(blocks);
  }
   */
  // Distance Calculations
  static int i = 0;
  int j;
  uint16_t blocks;

  blocks = pixy.getBlocks();

  if (blocks) {
    i++;
    detection = 1;
    // do this (print) every 50 frames because printing every
    // frame would bog down the Arduino
    if (i % 50 == 0) {          
      for (j = 0; j < blocks; j++) {
        pixelsWidth = pixy.blocks[j].width;
        pixelsHeight = pixy.blocks[j].height;
        distanceWidth = (widthOfObject * focalLengthWidth) / pixelsWidth;
        distanceHeight = (heightOfObject * focalLengthHeight) / pixelsHeight;
        avg = (distanceWidth + distanceHeight)/2;
        avg = round(avg);
        feet = avg/12;
        inches = int(avg) % 12;
        x = pixy.blocks[j].x; 
      }
    }
    k = 0;
  }
  
  else {
    k += 1;
  }
  //Serial.print(k);
  if (k == 8) {
    Serial.println("No Detection");
    halt();
    detection = 0;
  }
  
    

  if (detection == 1) {
    
    // Condition for Forward
    if ((avg > minDist) && (x > minX) && (x < maxX) ) {
      Serial.println("Moving Forward!");
      forward();
    }
    // Condition for Left
    else if ((avg > minDist) && (x < minX)) {
      Serial.println("Turning Left!");
      left();
    }
    // Condition for Right
    else if ((avg > minDist) && (x > maxX)) { 
      Serial.println("Turning Right!");
      right();
    }
  
    // Condition for Hault 
    else if (avg < minDist) {
      Serial.println("Stop!");
      halt();
    }
  
  }

}

// DC Motor Control
void forward() {
  // Motor 1
  analogWrite(speedPinA, 225);//Sets speed variable via PWM 
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
 

  // Motor 2
  analogWrite(speedPinB, 225);
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
 
}


void right() {
   // Motor 1
  analogWrite(speedPinA, 1);//Sets speed variable via PWM 
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
  
  // Motor 2
  analogWrite(speedPinB, 230);
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
  
  
}

void left() {
  // Motor 1
  analogWrite(speedPinA, 230);//Sets speed variable via PWM 
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
  

  // Motor 2
  analogWrite(speedPinB, 1);
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
  
  
}

void halt() {
 // Motor 1
  analogWrite(speedPinA, 0);//Sets speed variable via PWM 
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
  

  // Motor 2
  analogWrite(speedPinB, 0);
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
  
}


//Formula = FocalLengthWidth = (pixels * knowdistanceininches) / widthOfObject
//Distance = (widthOfObject * FocalLengthWidth) / pixelsWidth
//focal length and distance for height is calculated the same way replacing width with height values



