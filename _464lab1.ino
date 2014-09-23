/*****

BME 464 Arduino ADC/DAC Lab
Kirsty, Lauren, Claire, Elena
DUE: 9/17/14

*****/

// include the library code:
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

//// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

int pinIN = A1; // pin that reads in analog signal
int pinOUT = 9; // pin that writes analog value for output signal
float signalIN; // averaged analog signal 
float voltageIN; // mapped voltage (0-5 V)
float signal; // actual voltage from original signal (-5-5 V)
boolean not_printing = true;
int t_calibration = 0; // initialize time of calibration for use in setup and loop
boolean negative_voltage = false;
int bin_vals[] = {0, 0, 0};

void setup (){
  // Debugging output.  Begin communication with computer.
  Serial.begin(9600);
  // Set up for LCD shield
  lcd.begin(16,2);
  lcd.setBacklight(VIOLET);
  // Initialize pins as input/output
  pinMode(pinIN, INPUT);
  pinMode(pinOUT, OUTPUT);  
  lcd.setCursor(0,0);
  lcd.print("Output Voltage:");
  lcd.setCursor(5,1);
  lcd.print("Volts");
}

void loop(){
  // Serial.println(not_printing);
  int t_start = millis(); 
  while (not_printing == true){
      signalIN = analogRead(pinIN);
      // ******************** MLP COMMENT *********************************** //
      // The value '5' is hard-coded in the next few lines.  '5' should be a
      // variable that is declared above and then referenced here since it is a
      // design spec.
      // ******************************************************************** //
   // Map analog values (0-1023) to voltage values (0-5 V)
      voltageIN = signalIN * (5.0/1023.0);
  // Shift and scale mapped voltage to find actual signal (-5-5 V)
      signal = (voltageIN*2) - 5;
        
  // Map voltageIN (0-5 V) to analog values (0-255) for analogwrite
  // Circuitry will take care of the shifting and scaling to get original signal
      int signalOUT = round(voltageIN * (255/5));
      analogWrite(pinOUT,signalOUT);
  // Find binary voltage
  // set the first index (i=0) for all potential input voltages
      // ******************* MLP COMMENT *********************************** //
      // Like my earlier comment, there are hard-coded values here related 
      // to your discretization for the binary output; this should all be 
      // done as variables, not hard-coded values.
      // ******************************************************************** //
      // ******************* MLP COMMENT *********************************** //
      // All of this conditional logic can / should be broken out as a 
      // separate function.  This will serve to:
      // (1) Make the main loop() easier to read for logic (not bogged down
      //     all of the tested statements, and
      // (2) easily swap in different log if needed for modified specs 
      // ******************************************************************** //
   if (abs(round(signal)) < 3.5){ // corresponds to V = +/-0-3V, MSB of binary value is always 0
     bin_vals[0] = 0;
   } 
   if (abs(round(signal)) >= 3.5){ // corresponds to V = +/-4-5V, MSB of binary value is always 1
     bin_vals[0] = 1;
   }
   // set the second index (i=1) for all potential input voltages
   if ((abs(round(signal)) < 1.5) || (abs(round(signal)) >= 3.5)){ // corresponds to V = +/- 0,1,4,5V
     bin_vals[1] = 0;
   }
   if ((abs(round(signal)) >= 1.5) && (abs(round(signal)) < 3.5)){ // corresponds to V = +/- 2,3V
     bin_vals[1] = 1;
   }
   if ((abs(round(signal)) < .5) || ((abs(round(signal)) >= 1.5) && (abs(round(signal)) < 2.5)) || ((abs(round(signal)) >= 3.5) && (abs(round(signal)) < 4.5))){
     bin_vals[2] = 0;
   }
    if (((abs(round(signal)) >= .5) && (abs(round(signal)) < 1.5)) || ((abs(round(signal)) >= 2.5) && (abs(round(signal)) < 3.5)) || (abs(round(signal)) >= 4.5)){
     bin_vals[2] = 1;
   }    
      // ******************* MLP COMMENT *********************************** //
      // Again, break out as subfunc
      // ******************************************************************** //
  // has it been 1 second? should we lcd print yet?
  
  int t_now = millis();
  if (t_now >= (t_start+1000)) {
    not_printing = false;
    //lcd.clear();
    }
  }
  //Serial.println(not_printing);
   if(round(signal)<0) {
     negative_voltage = true; } 
     
   if(round(signal)>0) {
     negative_voltage = false; }
  
  lcd.setCursor(0,1); 
  if(negative_voltage==true){
    lcd.print('-'); }
  if(negative_voltage==false){
    lcd.print('+'); }
    
    
  // Serial.println(negative_voltage);
  lcd.setCursor(1,1);
  for (int i=0; i <= 2; i++){
  lcd.print(bin_vals[i]);
  lcd.setCursor(i+2,1);
  }
  not_printing = true;
}


    
    
  
  
  

