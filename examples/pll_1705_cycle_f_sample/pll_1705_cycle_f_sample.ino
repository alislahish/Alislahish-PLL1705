/**
* This sketch demonstrates how to choose the output (sampling) frequency of a 
* Texas Instruments PLL1705 3.3v phase-locked-loop controlled from an MCP23017
* See: http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en023499
*
* In this example the pin connections are as follows 
*    ESP8266    ->    MCP23017
*    GPIO 4(SDA)->    Pin 13 
*    GPIO 5(SCL)->    Pin 12
*    
*    MCP23017
*    Pin 9 -> 3.3V, Pin 10 -> GND, 
*    Pin 15, 16, 17 (A0, A1, A2) -> GND, Pin 18 (!RESET) -> Vcc via 10K resistor 
*
*    MCP23017        ->    PLL1705
*    Pin 1 (GBP0)    ->    Pin 5 (FS1) 
*    Pin 2 (GPB1)    ->    Pin 6 (FS2)
*    Pin 3 (GBP2)    ->    Pin 7 (SR)
*    Pin 4 (GBP3)    ->    Pin 12 (CSEL)  
*    You can also connect PLL1705 CSEL to ground or high 
*    if you don't need the MCKO1 or MCKO2 output control  
*
*    PLL1705 - This circuit is taken from the datasheet:
*              http://www.ti.com/product/pll1705
*    Connect a 27 MHz crystal across pins 10 & 11 (XT1 & XT2), 
*    then add two equal-valued  capacitors from pins 10 and 11 to ground to create a pi network.
*    Consult your crystal datasheet for load capacitance and calculate appropriate pi network values:
*    See: http://www.mecxtal.com/pdf/te_notes/tn-021.pdf
*
*    Bridge the following pin pairs with individual 0.1 uF ceramic capacitors:
*            pins 1 & 4 (Vdd1 & DGND1),
*            pins 8 & 9 (Vcc & AGND), 
*            pins 13 & 16 (Vdd2 & DGND2), and
*            pins 17 & 20 (DGND3 & Vdd3)
*
*    Bridge the following pin pairs with individual 10 uF aluminum electrolytic capacitors:
*            pins 8 & 9 (Vcc & AGND), 
*            pins 1 & 4 (Vdd & DGND1) 
*
*    Connect Pins 1, 8 (Vdd, Vcc) to 3.3V supply. 
*    Connect pins 4, 9, 16, 17 (DGND1, AGND, DGND2, DGND3) to ground. 
*
*    Connect your logic analyzer or oscilloscope to pin 2 (SCKO2 - 256fs) of the PLL1705
*    You should see a signal that cycles through the following frequencies:
*    8.192Mhz, 11.2896MHz, 12.288MHz, 16.384MHz, 22.5792MHz, 24.576MHz
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <ICUsingMCP23017.h>
#include <Alislahish_PLL1705.h>

//PLL1705
#define FS1_PIN 8 //GPB0 - pin 1
#define FS2_PIN 9 //GPB1 - pin 2
#define SR_PIN 10 //GPB2 - pin 3
#define CSEL_PIN 11 //GPB3 -pin 4

#define FREQ_HOLD_TIME 4181 //how long to hold the sample frequency at each rate

Alislahish_PLL1705 pll(CSEL_PIN, FS1_PIN, FS2_PIN, SR_PIN);
Adafruit_MCP23017 mcp;
uint8_t mcpAddr = 0;
uint8_t fCount = 0;
PLLSamplingFrequencies currentF = PLLSamplingFrequencies::KHZ_32000;
long lastMillis = 0L;
uint8_t led = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output    
  //pinMode(LED_BUILTIN, OUTPUT);     // Arduino    
  //start the MCP23017
  mcp.begin(mcpAddr);
  //make the PLL1705 use the MCP23017
  pll.setMCP23017(true, &mcp, mcpAddr);
  //finally start using
  pll.begin(currentF);
    
}

void loop() {
    //switch to new frequency every interval of FREQ_HOLD_TIME milliseconds
    if (millis() >= (lastMillis + FREQ_HOLD_TIME)) {
    lastMillis=millis();
    fCount+=1;
    fCount%=6;
    currentF = static_cast<PLLSamplingFrequencies>(fCount%6);
    pll.selectSamplingFrequency(currentF);
    
    //toggle led
    if(led==HIGH){
        led=LOW;
    } else {
        led=HIGH;
    }
    digitalWrite(BUILTIN_LED, led);
    //digitalWrite(LED_BUILTIN, led); //arduino
  }
}