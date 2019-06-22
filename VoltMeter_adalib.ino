#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Declare LCD object for software SPI
// Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// variable declarations
float voltage = 0;
int sensorPin = A0;  
float sensorValue = 0;  

float vout = 0;
float vin = 0; //maximum voltage that can be measured is: 8.4 V, for 2 x 18650 batteries

float R1 = 4100; //fixed values of resistors used in the voltage divider
float R2 = 5900; 
 
int period = 1000;
unsigned long time_now = 0;



void setup()   {
  Serial.begin(9600);

  //Initialize Display
  display.begin();

  // you can change the contrast around to adapt the display for the best viewing!
  display.setContrast(70);

  display.clearDisplay();   // clear the screen and buffer
 
  display.setTextSize(1); //set the font size
  display.setTextColor(BLACK, WHITE);
  
  display.setCursor(0, 5); //set the coordinates to start writing text
  display.print("BATTERY:");
  display.setCursor(40, 15); //set the coordinates to start writing text
  display.print("Volts");

  display.setCursor(0, 30);
  display.print("LOAD:");
  display.setCursor(40, 40);
  display.print("grams");

}

void loop() {


  if(millis() > time_now + period){
        time_now = millis();
        voltage = readVoltage();
        
        draw_value(12, 15, voltage);  // print voltage every 1 second on LCD
        Serial.println(voltage);  //print voltage every 1 second on Serial Monitor
        
        //draw_value(12, 40, load);  // print load every 1 second on LCD
        //Serial.println(load);  //print load every 1 second on Serial Monitor
        
    }
  

}

float readVoltage()
{
   sensorValue = analogRead(sensorPin); //get the integer value between 0 and 1024 which corresponds to 0 V and 5 V, respectively.
   vout = (sensorValue * 5.0) / 1024.0; //this is the calculated value between 0 and 1024
   vin = vout / (R2/(R1+R2)); //this is the formula to find the actual input battery voltage
   return vin;
}


// print value on the LCD
void draw_value(byte x_pos, byte y_pos, float value)
{
  display.setCursor(x_pos, y_pos); //set the coordinates to start writing text
  display.print(value,2); //display to 2 decimal places
  display.display(); //call this method for the changes to make effect
}
