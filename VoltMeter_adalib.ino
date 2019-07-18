#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Declare LCD object for software SPI
// Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);
//Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3); //default
Adafruit_PCD8544 display = Adafruit_PCD8544(11, 2, 5, 4, 3);  //works for shield. tested!


//Digital voltmeter, which can safely measure DC battery voltage in 0 to 8.33 V range. 
// variable declarations
float voltage = 0;
int sensorPin = A0;  
float sensorValue = 0;

float vout = 0.0;
float vin = 0.0; //maximum voltage that can be measured is: 15 V (based on 3 x 18650 batteries: 12.6 V when fully charged)
                // or 8.4 V for 2 x 18650 batteries.

float R1 = 7000; //fixed values in Ohms of resistors used in the voltage divider
float R2 = 10000; 
//with these values of resistors for the resistor divider, the max battery voltage should not exceed 8.4999 V, otherwise this will
//give a resulting voltage of more than 5V to the analog input pin on the Arduino which will damage the pin due to overvoltage.

 
int period = 500; //execuse millis() loop every 500 milliseconds
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
        
        //draw_value(12, 15, load);  // print load every 0.5 seconds on LCD
        //Serial.println(load);  //print load every 0.5 seconds on Serial Monitor
        
        draw_value(12, 40, voltage);  // print voltage every 0.5 seconds on LCD
        Serial.println(voltage);  //print voltage every 0.5 seconds on Serial Monitor
        
    }
  

}

float readVoltage()
{
   sensorValue = analogRead(sensorPin); //receives the analogue voltage value between 0 V and 5 V and converts it using 10-bit ADC to an integer value between 0 and 1023.
   
   vout = (sensorValue * 5.0) / 1024.0; //Reads the 10-bit value from the specified analog pin and converts the value into a corresponding voltage.
   //2^10, or 1024 divisions (0 to 1023), of the reference voltage, 5 V.
   //10-bit analog to digital converter will map input voltages between 0 and the operating voltage(5V or 3.3V) into integer values between 0 and 1023.
   //The ADC is 10-bit. The function analogRead() returns a value from 0 up to 1023.
   //https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
   
   vin = ((vout * (R1+R2)) / R2); //voltage divider formula to find the actual battery voltage
   // vin = vout / (R2/(R1+R2));   //original formula

   return vin;
}


// print value on the LCD
void draw_value(byte x_pos, byte y_pos, float value)
{
  display.setCursor(x_pos, y_pos); //set the coordinates to start writing text
  display.print(value,2); //display to 2 decimal places
  display.display(); //call this method for the changes to make effect
}
