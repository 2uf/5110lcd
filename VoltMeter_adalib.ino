#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Declare LCD object for software SPI
// Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// variable declarations
float voltage = 0.0;
int sensorPin = A0;  
float sensorValue = 0.0f;  

float vout = 0.0;
float vin = 0.0; //maximum voltage that can be measured is: 15 V (based on 3 x 18650 batteries: 12.6 V when fully charged)

float R1 = 2000.0; //fixed values in Ohms of resistors used in the voltage divider
float R2 = 1000.0; 
 
int period = 500;
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
  display.print("LOAD:");
  display.setCursor(45, 15); //set the coordinates to start writing text
  display.print("grams");

  display.setCursor(0, 30);
  display.print("BATTERY:");
  display.setCursor(45, 40);
  display.print("Volts");

}

void loop() {


  if(millis() > time_now + period){
        time_now = millis();
        voltage = readVoltage();
        
        //draw_value(12, 15, load);  // print voltage every 1 second on LCD
        //Serial.println(load);  //print voltage every 1 second on Serial Monitor
        
        draw_value(12, 40, voltage);  // print load every 1 second on LCD
        Serial.println(voltage);  //print load every 1 second on Serial Monitor
        
    }
  

}

float readVoltage()
{
   sensorValue = analogRead(sensorPin); //receives the analogue voltage value between 0 V and 5 V and converts it using ADC.
   vout = (sensorValue * 5.0) / 1024.0; //converts the discrete value between 0 and 1024 into a corresponding voltage
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
