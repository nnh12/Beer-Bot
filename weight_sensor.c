#include <Energia.h>
#include "HX711.h"

const int DATA_PIN = 2;                 // Define the data pint
const int CLOCK_PIN = 3;                // Define the clock pin

HX711 scale;

int count = 1;
int sum = 0;
int average = 0;
int num = 0;

/*
*  A function to intitialize everything 
*  with the correct pinouts - data pins and clock pins.
*
*/
void setup() {
    
  Serial.begin(57600);                    // Set the Baud rate and the Serial Communication
  Serial.println("Initializing the scale");
  pinMode(13, OUTPUT);
  scale.begin(DATA_PIN, CLOCK_PIN);        // Inititalize the correct data and clock pin

  Serial.println("Calibrate the scael");
  Serial.print("read: \t\t");
  Serial.println(scale.read());             // Print the current scale of the weight

  Serial.print("Obtain the average: \t\t");
  Serial.println(scale.read_average(10));   // print the average of 20 readings from the ADC

  Serial.print("Average value: \t\t");
  Serial.println(scale.get_value(5));       // Get the average of 5 values

  Serial.print("Average: \t\t");
  Serial.println(scale.get_units(5), 1);    // print the average of 5 readings from the ADC minus tare weight (not set) divide
    
  scale.set_scale(-459.542);                // Set the current send of the scale
  scale.tare();                             // reset the scale to 0
}


/*
*  A loop to go through and sample the current weight every 1000 ms
*  If the weight passes over the threshold, then it will set a GPIO pin to be
*  HIGH. We have another MSP430 that will take an input and drive the motors if it 
*  if HIGH. Else, it will not move when it is LOW.
*/
void loop() {
 
  Serial.print("one reading:\t");

  Serial.print(scale.get_units(), 1);

  // Sets it to high if a weight is detected
  if (scale.get_units() < -30) {
      digitalWrite(6, HIGH);
  }

  // Sets it to LOW if a weight it not detected
  else {
      digitalWrite(6, LOW); 
  }

  // Add the units to the scale.
  num = scale.get_units();
  sum += scale.get_units();

  // Increment the count
  average = (sum/count);
  count++; 

  // Print the average
  Serial.print("\t| average:\t");
  Serial.println(average, 5);

  // Delay the 
  delay(1000);
}
