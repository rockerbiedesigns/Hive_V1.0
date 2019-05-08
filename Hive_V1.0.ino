//////////////////////////////////////////////
//  HIVE operational code V2                //
//////////////////////////////////////////////
// CREATED BY                               //
// Phillip Rockerbie                        //
//////////////////////////////////////////////
//                                          //
//Project Background:                       //
//http://www.rockerbiedesigns.com/hive-2/   //
//                                          //
//////////////////////////////////////////////

#include <Wire.h>

//MPU Data Access//
const int MPU_addr = 0x68; int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
//Max. and Min. Ac values recorded from MPU. This may need to change
//if an MPU-6050 clone
int minVal = 265; 
int maxVal = 402;

//Rotation values. Hive focuses on z-axis, but you can change these
//if you mount your MPU differently than shown in the PROJECT BACKGROUND
double x; double y; double z;

//RBG Values//
int red = 0;
int green = 0;
int blue = 0;

//RGB Pins//
int RPin = 3;
int GPin = 5;
int BPin = 6;

//Speaker Pin//
int piezoPin = 4;

//Checks for colour//
boolean rGate = true;
boolean oGate = false;
boolean yGate = false;
boolean gGate = false;
boolean bGate = false;
boolean pGate = false;
boolean mGate = false;

//Checks for fade between colours//
boolean ROGate = false;
boolean OYGate = false;
boolean YGGate = false;
boolean GBGate = false;
boolean BPGate = false;
boolean PMGate = false;
boolean MRGate = false;

void setup() {
//Initialize MPU
 Wire.begin();
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x6B);
 Wire.write(0);
 Wire.endTransmission(true);
//Debug
 Serial.begin(9600);
//Initialize speaker
 pinMode(7, OUTPUT); //PIN 7 transmits a pulse//
}

void loop() {
//Start MPU
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x3B);
 Wire.endTransmission(false);
 Wire.requestFrom(MPU_addr, 14, true);
 
 //Access AcX from MPU
 //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
 AcX = Wire.read() << 8;
 AcX |= Wire.read();
 
 //Access AcY from MPU
 //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
 AcY = Wire.read() << 8;
 AcY |= Wire.read();
 
 //Access AcZ from MPU
 //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
 AcZ = Wire.read() << 8;
 AcZ |= Wire.read();
 
 //Map the raw data so its usable
 int xAng = map(AcX, minVal, maxVal, -90, 90);
 int yAng = map(AcY, minVal, maxVal, -90, 90);
 int zAng = map(AcZ, minVal, maxVal, -90, 90);
 
 //Integrate it from angle/sec to angle
 x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
 y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
 z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
 
 //Checks the Z angle which has been intergrated in the above sequence
 Serial.print("AngleZ= "); 
 Serial.println(z);
 
 //GATE system
 //Below are gate checks. When a colour approaches 360(or 0) degrees, it
 //compares direction between 270 and 360 using T/F statements to determine 
 //what colour the LEDs should change to.
 
 //RED GATE 1//
 if (z >= 0 && z < 90 && rGate == true)
 {
 red = 255;
 green = 0;
 blue = 0;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = true;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = false;
 ROGate = false;
 OYGate = false;
 YGGate = false;
 GBGate = false;
 BPGate = false;
 PMGate = false;
 MRGate = true;
 // Serial.println("Red");
 }
 //RED GATE 2//
 if (z >= 90 && z < 180 && rGate == true)
 {
 red = 255;
 green = 0;
 blue = 0;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = true;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = false;
 ROGate = true;
 OYGate = false;
 YGGate = false;
 GBGate = false;
 BPGate = false;
 PMGate = false;
 MRGate = false;
 Serial.println("Red");
 }
 //RED<->ORANGE GATE 1//
 if (z >= 180 && z < 270 && ROGate == true && OYGate == false && YGGate == false && GBGate == false &&
BPGate == false && PMGate == false && MRGate == false)
 {
 red = 255;
 green = (z - 180) * 0.4; //range equation to reach orange
 blue = 0;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = true;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = false;
 Serial.println("RO1");
 }
 //RED<->ORANGE GATE 2//
 if (z >= 270 && z < 360 && ROGate == true && OYGate == false && YGGate == false && GBGate == false &&
BPGate == false && PMGate == false && MRGate == false)
 {
 red = 255;
 green = (z - 180) * 0.4;
 blue = 0;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = true;
 yGate = false;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = false;
 Serial.println("RO2");
 }
 //ORANGE GATE 1//
 if (z >= 0 && z < 90 && oGate == true)
 {
 red = 255;
 green = 72; //calculated green % to attain orange
 blue = 0;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = true;
 yGate = false;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = false;
 ROGate = true;
 OYGate = false;
 YGGate = false;
 GBGate = false;
 BPGate = false;
 PMGate = false;
 MRGate = false;
 Serial.println("Orange");
 }
 //ORANGE GATE 2//
 if (z >= 90 && z < 180 && oGate == true)
 {
 red = 255;
 green = 72;
 blue = 0;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = true;
 yGate = false;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = false;
 ROGate = false;
 OYGate = true;
 YGGate = false;
 GBGate = false;
 BPGate = false;
 PMGate = false;
 MRGate = false;
 Serial.println("Orange");
 }
 //ORANGE<->YELLOW GATE 1//
 if (z >= 180 && z < 270 && ROGate == false && OYGate == true && YGGate == false && GBGate == false &&
BPGate == false && PMGate == false && MRGate == false)
 {
 red = 255;
 green = 72 + (z - 180); //Equation to transition to yellow
 blue = 0;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = true;
 yGate = false;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = false;
 Serial.println("OY1");
 }
 //ORANGE<->YELLOW GATE 2//
 if (z >= 270 && z < 360 && ROGate == false && OYGate == true && YGGate == false && GBGate == false &&
BPGate == false && PMGate == false && MRGate == false)
 {
 red = 255;
 green = 72 + (z - 180);
 blue = 0;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = true;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = false;
 Serial.println("OY2");
 }
 //YELLOW GATE 1//
 if (z >= 0 && z < 90 && yGate == true)
 {
 red = 255;
 green = 252;
 blue = 0;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = true;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = false;
 ROGate = false;
 OYGate = true;
 YGGate = false;
 GBGate = false;
 BPGate = false;
 PMGate = false;
 MRGate = false;
 Serial.println("Yellow");
 }
 if (z >= 90 && z < 180 && yGate == true) //YELLOW GATE 2
 {
 red = 255;
 green = 253;
 blue = 0;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = true;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = false;
 ROGate = false;
 OYGate = false;
 YGGate = true;
  GBGate = false;
 BPGate = false;
 PMGate = false;
 MRGate = false;
 Serial.println("Yellow");
 }
 //YELLOW<->GREEN GATE 1//
 if (z >= 180 && z < 270 && ROGate == false && OYGate == false && YGGate == true && GBGate == false &&
BPGate == false && PMGate == false && MRGate == false)
 {
 red = ((360 - z) * 1.42);
 green = 254;
 blue = 0;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = true;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = false;
 Serial.println("YG1");
 }
 //YELLOW<->GREEN GATE 2//
 if (z >= 270 && z < 360 && ROGate == false && OYGate == false && YGGate == true && GBGate == false &&
BPGate == false && PMGate == false && MRGate == false)
 {
 red = ((360 - z) * 1.42);
 green = 255;
 blue = 0;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = true;
 bGate = false;
 pGate = false;
 mGate = false;
 Serial.println("YG2");
 }
 //GREEN GATE 1//
 if (z >= 0 && z < 90 && gGate == true)
 {
 red = 0;
 green = 255;
 blue = 0;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = true;
  bGate = false;
 pGate = false;
 mGate = false;
 ROGate = false;
 OYGate = false;
 YGGate = true;
 GBGate = false;
 BPGate = false;
 PMGate = false;
 MRGate = false;
 Serial.println("Green");
 }
 //GREEN GATE 2//
 if (z >= 90 && z < 180 && gGate == true)
 {
 red = 0;
 green = 255;
 blue = 0;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = true;
 bGate = false;
 pGate = false;
 mGate = false;
 ROGate = false;
 OYGate = false;
 YGGate = false;
 GBGate = true;
 BPGate = false;
 PMGate = false;
 MRGate = false;
 Serial.println("Green");
 }
 //GREEN<->BLUE GATE 1//
 if (z >= 180 && z < 270 && ROGate == false && OYGate == false && YGGate == false && GBGate == true &&
BPGate == false && PMGate == false && MRGate == false)
 {
 red = 0;
 green = ((360 - z) * 1.42);
 blue = (z - 180) * 1.42;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = true;
 bGate = false;
 pGate = false;
 mGate = false;
 Serial.println("GB1");
 }
 //GREEN<->BLUE GATE 2//
  if (z >= 270 && z < 360 && ROGate == false && OYGate == false && YGGate == false && GBGate == true &&
BPGate == false && PMGate == false && MRGate == false)
 {
 red = 0;
 green = ((360 - z) * 1.42);
 blue = (z - 180) * 1.42;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = true;
 pGate = false;
 mGate = false;
 Serial.println("GB2");
 }
 //BLUE GATE 1//
 if (z >= 0 && z < 90 && bGate == true)
 {
 red = 0;
 green = 0;
 blue = 255;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = true;
 pGate = false;
 mGate = false;
 ROGate = false;
 OYGate = false;
 YGGate = false;
 GBGate = true;
 BPGate = false;
 PMGate = false;
 MRGate = false;
 Serial.println("Blue");
 }
 //BLUE GATE 2//
 if (z >= 90 && z < 180 && bGate == true)
 {
 red = 0;
 green = 0;
 blue = 255;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = true;
 pGate = false;
 mGate = false;
 ROGate = false;
 OYGate = false;
 YGGate = false;
 GBGate = false;
 BPGate = true;
 PMGate = false;
 MRGate = false;
 Serial.println("Blue");
 }
 //BLUE<->PURPLE GATE 1//
 if (z >= 180 && z < 270 && ROGate == false && OYGate == false && YGGate == false && GBGate == false &&
BPGate == true && PMGate == false && MRGate == false)
 {
 red = (z - 180) * 0.4;
 green = 0;
 blue = 255;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = true;
 pGate = false;
 mGate = false;
 Serial.println("BP1");
 }
 //BLUE<->PURPLE GATE 2//
 if (z >= 270 && z < 360 && ROGate == false && OYGate == false && YGGate == false && GBGate == false &&
BPGate == true && PMGate == false && MRGate == false)
 {
 red = (z - 180) * 0.4;
 green = 0;
 blue = 255;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = false;
 pGate = true;
 mGate = false;
 Serial.println("BP2");
 }
 //PURPLE GATE 1//
 if (z >= 0 && z < 90 && pGate == true)
 {
 red = 72;
 green = 0;
 blue = 255;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = false;
 pGate = true;
 mGate = false;
 ROGate = false;
 OYGate = false;
 YGGate = false;
 GBGate = false;
 BPGate = true;
 PMGate = false;
 MRGate = false;
 Serial.println("Purple");
 }
 //PURPLE GATE 2//
 if (z >= 90 && z < 180 && pGate == true)
 {
 red = 72;
 green = 0;
 blue = 255;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = false;
 pGate = true;
 mGate = false;
 ROGate = false;
 OYGate = false;
 YGGate = false;
 GBGate = false;
 BPGate = false;
 PMGate = true;
 MRGate = false;
 Serial.println("Purple");
 }
 //PURPLE<->MAGENTA GATE 1//
 if (z >= 180 && z < 270 && ROGate == false && OYGate == false && YGGate == false && GBGate == false &&
BPGate == false && PMGate == true && MRGate == false)
 {
 red = 72 + ((z - 180) * 1.01);
 green = 0;
 blue = 255;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = false;
  pGate = true;
 mGate = false;
 Serial.println("PM1");
 }
 //PURPLE<->MAGENTA GATE 2//
 if (z >= 270 && z < 360 && ROGate == false && OYGate == false && YGGate == false && GBGate == false &&
BPGate == false && PMGate == true && MRGate == false)
 {
 red = 72 + ((z - 180) * 1.01);
 green = 0;
 blue = 255;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = true;
 Serial.println("PM2");
 }
 //MAGENTA GATE 1//
 if (z >= 0 && z < 90 && mGate == true)
 {
 red = 255;
 green = 0;
 blue = 255;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = true;
 ROGate = false;
 OYGate = false;
 YGGate = false;
 GBGate = false;
 BPGate = false;
 PMGate = true;
 MRGate = false;
 Serial.println("Magenta");
 }
 //MAGENTA GATE 2//
 if (z >= 90 && z < 180 && mGate == true)
 {
 red = 255;
 green = 0;
 blue = 255;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = true;
 ROGate = false;
 OYGate = false;
 YGGate = false;
 GBGate = false;
 BPGate = false;
 PMGate = false;
 MRGate = true;
 Serial.println("Magenta");
 }
 //MAGENTA<->RED GATE 1//
 if (z >= 180 && z < 270 && ROGate == false && OYGate == false && YGGate == false && GBGate == false &&
BPGate == false && PMGate == false && MRGate == true)
 {
 red = 255;
 green = 0;
 blue = ((360 - z) * 1.42);;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = false;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = true;
 Serial.println("MR1");
 }
 //MAGENTA<->RED GATE 2//
 if (z >= 270 && z < 360 && ROGate == false && OYGate == false && YGGate == false && GBGate == false &&
BPGate == false && PMGate == false && MRGate == true)
 {
 red = 255;
 green = 0;
 blue = ((360 - z) * 1.42);;
 analogWrite(BPin, blue);
 analogWrite(GPin, green);
 analogWrite(RPin, red);
 rGate = true;
 oGate = false;
 yGate = false;
 gGate = false;
 bGate = false;
 pGate = false;
 mGate = false;
 Serial.println("MR2");
 }
 
 delay(10);
 //Activated the touch function
 swarmTouch();
}

void swarmTouch()
{
 //This function detects if a pulse from PIN 7 registers on an Analog PIN
 //If the circuit between 7 and one of three analog PINs is complete then
 //speaker emits a random tone.
 
 //Emit pulse
 analogWrite(7, 255);
 //Detect pulse
 int currentA = analogRead(A0);
 int currentB = analogRead(A1);
 int currentC = analogRead(A2);
 Serial.println(currentA);
 
 //Single Analog connection//
 if (currentA > 1010 && currentB < 1010 && currentC < 1010)
 {
 tone(piezoPin, random(100000), 5000);
 }
 if (currentB > 1010 && currentA < 1010 & currentC < 1010)
 {
 tone(piezoPin, random(100000), 1000);
 }
 if (currentC > 1010 && currentA < 1010 && currentB < 1010)
 {
 tone(piezoPin, random(100000), 1000);
 }
 //Double Analog connection//
 if (currentA > 1010 && currentB > 1010 && currentC < 1010)
 {
 tone(piezoPin, random(100000), 1000);
 }
 if (currentA > 1010 && currentC > 1010 && currentB < 1010)
 {
 tone(piezoPin, random(100000), 1000);
 }
 if (currentC > 1010 && currentB > 1010 && currentA < 1010)
 {
 tone(piezoPin, random(100000), 1000);
 }
 //Triple Analog connection//
 if (currentA > 1010 && currentC > 1010 && currentB > 1010)
 {
 tone(piezoPin, random(100000));
 }
 else
 {
 tone(piezoPin, 1, 1);
 }
}
