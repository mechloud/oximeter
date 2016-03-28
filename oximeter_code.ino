#include <math.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

const int wait = 2000;  //milliseconds LEDs are on
const int numPts = 200; //number of data points taken within the wait time
const int interval = wait / numPts; //time between every data point based on the wait time and number of points
int values[numPts];     //array to contain all data points within the wait time
double rTop; 
double rBot;
double rValues[10];     //small array to average out last 10 R Values calculated
int count = 0;          //count for R Values array

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Setup screen
  pinMode(5, OUTPUT);   //Red 650nm laser diode
  pinMode(3, OUTPUT);   //InfraRed 940nm LED
  pinMode(7, INPUT);    //Analog pin for photodiode readings
  display.display();    //Start up display
  delay(1000);
  display.clearDisplay();
}

void loop() {
  int minVal = 1023;  //minimum value taken
  int maxVal = 0;     //maximum value taken
  digitalWrite(5, 1); //Red on
  digitalWrite(3, 0); //IR off
  for (int j = 0; j < numPts; j++) {
    values[j] = analogRead(7);  //Read value of photodiode and assign to position j in values array
    if (values[j] < minVal) {   //Assign current reading to minimum value if current reading is smaller than previous minimum
      minVal = values[j];
    }
    else if (values[j] > maxVal) {  //Assign current reading to aximum value if current reading is largr than previous maximum
      maxVal = values[j];
    }
    delay(interval);
  }
 
  if (minVal == 0) { //To avoid all null values
    minVal = 1;
  }

  double rTop = log((double)minVal / (double)maxVal); //Top portion of the R Value formula ln(Redmin/Redmax)

  minVal = 1023; //Resetting min and max 
  maxVal = 0;
  digitalWrite(5, 0); //Red off
  digitalWrite(3, 1); //IR on
  for (int j = 0; j < numPts; j++) {
    values[j] = analogRead(7);
    if (values[j] < minVal) {
      minVal = values[j];
    }
    else if (values[j] > maxVal) {
      maxVal = values[j];
    }
    delay(interval);
  }

  if (minVal == 0) { //To avoid all null values
    minVal = 1;
  }

  double rBot = log((double)minVal / (double)maxVal); //Bottom portion of the R Value formula ln(IRmin/IRmax)
  double rValue = rTop / rBot; //R Value formula
  rValues[count] = rValue;     //Assign current R value to position count in array

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("SpO2 value");  //Display SpO2 value on screen

  if (rValues[9] != NULL) { //if the array is full
    double rSum = 0;
    for (int i = 0; i < 10; i++) {
      rSum = rSum + rValues[i]; //Sum of all R Values in array for average
    }
    double rAvg = rSum / 10;    //Average of R Values
    
    display.print(100 * rAvg);  //Display R Value average x100 and a percentage sign
    display.print("%"); 
    display.display();
    display.clearDisplay();
  }
  else {                        //If array not full, display current R Value
    display.print(100 * rValue);
    display.print("%");
    display.display();
    display.clearDisplay();
  }

  count++; //Increase count
  count = count % 10; //To make sure count goes from 0 to 9 and back to 0 (circular array) 
}
