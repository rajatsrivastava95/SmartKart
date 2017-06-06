/**************************************************************
 Course:        Purdue University ME-475 Senior Design
 Project Title: KartPRO
 Team:          MOSIAC
 Description:   This C file computes the distance using the
                Ultrasonic Sensor HC-SR04.
                Code Tailored for RaspberryPI 3 Model B v1.2
 Last Modified: April 05, 2017
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>

#define TRIG 2  // GPIO Output
#define ECHO 0  // GPIO Input
#define RESET 3 // RESET SWITCH
#define RED_LED 27
#define GREEN_LED 28
#define BLUE_LED 29

int setup() {
    //Initialize GPIO pins to map to wiringPi numbers
    if (wiringPiSetup() == 01) {
        return 0; //return error status (initialization failed)
    }
    
    // Enabling Pin Modes for GPIO Pins
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(RESET, INPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    
    //TRIG pin must start LOW
    digitalWrite(TRIG, LOW);
    delay(30);
    
    return 1;
}

int getDistance() {
    //Send TRIG 40kHz pulse
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIG, LOW);
    
    //Wait for echo start
    while(digitalRead(ECHO) == LOW);  // Echo Starts at LOW, so wait till its LOW
    
    //Wait for echo end
    long startTime = micros();      // Gets the current CPU TimeStamp in microseconds
    while(digitalRead(ECHO) == HIGH);       // ECHO is HIGH when signal is recieved back
    long time = micros() - startTime; // Time Spent to receive signal
    
    //Get distance in cm
    int distance = (34300 * time) / (2000000);
    
    return distance;
}

int main(void) {
    int set = setup();
    if (set == 0) {
        return 0;
    }
    int dist;
    int reset;
    
    while(1) {
        dist = getDistance();
        printf("Distance: %dcm\n", dist);
        if(dist <= 50) {
            digitalWrite(RED_LED, HIGH);
            printf("RED\n");
        }
        else if((dist > 50) && (dist <= 100)) {
            digitalWrite(BLUE_LED, HIGH);
            printf("BLUE\n");
        }
        else {
            digitalWrite(GREEN_LED, HIGH);
            printf("GREEN\n");
        }
        
        // Reset when RESET button is pushed
        reset = digitalRead(RESET);
        
        if (reset == HIGH) {
            // Initializing to LEDs to LOW
            printf("RESET button pressed!\n");
            digitalWrite(RED_LED, LOW);
            digitalWrite(BLUE_LED, LOW);
            digitalWrite(GREEN_LED, LOW);
            return 0;
        }
        delay(100);     // Necessary Delay for avoiding errors
        // Reset Colors after Delay
        digitalWrite(RED_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        
    }
    //printf("Distance: %dcm\n", distance());
    
    return 0;
}





