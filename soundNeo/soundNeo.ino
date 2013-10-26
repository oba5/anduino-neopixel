#include <Adafruit_NeoPixel.h>

#define PIN 5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(185, PIN, NEO_GRB + NEO_KHZ800); //first number controls the amount of pixels you have (add 4 so the drip falls off the edge)

int bright = 1;    //select 1 thru 10
int wait = 0;//20;    //speed of leds

//values for MSGEQ7 Chip
int analogPin = 0; // read from multiplexer using analog input 0
int strobePin = 2; // strobe is attached to digital pin 2
int resetPin = 3; // reset is attached to digital pin 3
int spectrumValue[7]; // to hold a2d values
//int j =0;

int disablePin = 7; //pin to send 0s to output

uint32_t beginTime = 0;
uint32_t endTime = 0;

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

pinMode(disablePin, INPUT); 
  digitalWrite(disablePin, HIGH); //Turn on pull up resistors

  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  analogReference(DEFAULT);
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
 // Serial.println("MSGEQ7 test by J Skoba");

}
void loop() {

  beginTime = millis();
  while ( (millis()-beginTime) <= 60000)
   vuMeter_2();  

  beginTime = millis();
  while ( (millis()-beginTime) <= 60000)
   vuMeter();  
  
  beginTime = millis();
  while ( (millis()-beginTime) <= 60000)
   musicRainbowLoop_1();
   
/*  beginTime = millis();
  while ( (millis()-beginTime) <= 60000)
   musicRainbowLoop_2();*/
} 

void musicRainbowLoop_1(){
 
 int i = 0, j= 0;
  
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

//grabs readings from chip
  for (i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
//    delayMicroseconds(30); // to allow the output to settle
    spectrumValue[i] = analogRead(analogPin);

    digitalWrite(strobePin, HIGH);
    
    //makes 10 bit value 8 bit
    spectrumValue[i] /= 4;
    
    if(spectrumValue[i] > 255)
     spectrumValue[i] = 255;
   
   for (j=0; j<27; j++)
    { 
      if (spectrumValue[i] <= 55)
       spectrumValue[i] = 0;
    if (i == 0)
     nextLed(spectrumValue[i], 0,0, wait);
    else if (i == 1)
     nextLed(0, spectrumValue[i], 0, wait);
    else if (i == 2)
     nextLed(0,0, spectrumValue[i], wait);
    else if (i == 3)
     nextLed(spectrumValue[i], 0, spectrumValue[i], wait);
    else if (i == 4)
     nextLed(0, spectrumValue[i], spectrumValue[i], wait);
    else if (i == 5)
     nextLed(spectrumValue[i], spectrumValue[i],0, wait);
    else 
     nextLed(spectrumValue[i], spectrumValue[i], spectrumValue[i], wait);
    }
    delay(2);
   }
}

void musicRainbowLoop_2(){
  int i;
  int loopEnd;

  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

//grabs readings from chip
  for (i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
//    delayMicroseconds(30); // to allow the output to settle
    spectrumValue[i] = analogRead(analogPin);

    digitalWrite(strobePin, HIGH);
    
    //makes 10 bit value 8 bit
    spectrumValue[i] /= 4;
    
    if(spectrumValue[i] > 255)
     spectrumValue[i] = 255; 
   
   if (spectrumValue[i] >= 40)
    spectrumValue[i] = 0;  
  }
  
 loopEnd = 27;
 for (i = 0;i<loopEnd; i++)
  strip.setPixelColor(i, spectrumValue[0], 0, 0);
  
 loopEnd = i + 27; 
 for (;i<loopEnd; i++)
  strip.setPixelColor(i,0, spectrumValue[1], 0);
  
 loopEnd = i + 27; 
 for (;i<loopEnd; i++)
  strip.setPixelColor(i,0,0, spectrumValue[2]);  
  
 loopEnd = i + 27; 
 for (;i<loopEnd; i++)
  strip.setPixelColor(i,spectrumValue[3], 0,spectrumValue[3]); 
 
 loopEnd = i + 27; 
 for (;i<loopEnd; i++)
  strip.setPixelColor(i,spectrumValue[4], spectrumValue[4], 0);  
  
 loopEnd = i + 27; 
 for (;i<loopEnd; i++)
  strip.setPixelColor(i,(int)spectrumValue[5], (int)spectrumValue[5]/2, 0);  
  
   loopEnd = i + 27; 
 for (;i<loopEnd; i++)
  strip.setPixelColor(i,spectrumValue[6], spectrumValue[6], spectrumValue[6]);
  
  strip.show();
  
}

void vuMeter(){
  int i;
  int j;
  int loopEnd;
  int numPixels = strip.numPixels() - 5;
  double pixRatio = (float)numPixels / 255;
  double channelRatio = pixRatio / 7;
  float weight =1;

  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

//grabs readings from chip
  for (i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
//    delayMicroseconds(30); // to allow the output to settle
    spectrumValue[i] = analogRead(analogPin);

    digitalWrite(strobePin, HIGH);

    //weight = 1;
    if (i < 3)
     weight = 1.5;
    else 
      weight = 1; 
      //makes 10 bit value 8 bit
    spectrumValue[i] /= 4;
    spectrumValue[i] = (int)( (double)spectrumValue[i] * (channelRatio * weight) );

    if(spectrumValue[i] > 255)
     spectrumValue[i] = 255;

   /*if (spectrumValue[i] >= 40)
    spectrumValue[i] = 0;*/
  }


  
 for (i = 0;i<spectrumValue[0] && i< numPixels; i++)
  strip.setPixelColor(i, 255, 0, 0);

 loopEnd = i + spectrumValue[1];
 for (;i<loopEnd && i< numPixels; i++)
  strip.setPixelColor(i,0, 255, 0);

 loopEnd = i + spectrumValue[2];
 for (;i<loopEnd && i< numPixels; i++)
  strip.setPixelColor(i,0,0, 255);

 loopEnd = i + spectrumValue[3];
 for (;i<loopEnd && i< numPixels; i++)
  strip.setPixelColor(i,255, 0,255);

 loopEnd = i + spectrumValue[4];
 for (;i<loopEnd && i< numPixels; i++)
  strip.setPixelColor(i,255, 255, 0);

 loopEnd = i + spectrumValue[5];
 for (;i<loopEnd && i< numPixels; i++)
  strip.setPixelColor(i,255, 128, 0);

 loopEnd = i + spectrumValue[6];
 for (;i<loopEnd && i< numPixels; i++)
  strip.setPixelColor(i,255,255,255);
  
   
  for(;i<numPixels; i++)
  strip.setPixelColor(i,0,0,0);
  
  for (i = numPixels;i>numPixels-spectrumValue[0] && i>0; i--)
  strip.setPixelColor(i, 255, 0, 0);

 loopEnd = i - spectrumValue[1];
 for (;i>loopEnd && i>0; i--)
  strip.setPixelColor(i,0, 255, 0);

 loopEnd = i - spectrumValue[2];
 for (;i>loopEnd && i>0; i--)
  strip.setPixelColor(i,0,0, 255);

 loopEnd = i - spectrumValue[3];
 for (;i>loopEnd && i>0; i--)
  strip.setPixelColor(i,255, 0,255);

 loopEnd = i - spectrumValue[4];
 for (;i>loopEnd && i>0; i--)
  strip.setPixelColor(i,255, 255, 0);

 loopEnd = i - spectrumValue[5];
 for (;i>loopEnd && i>0; i--)
  strip.setPixelColor(i,255, 128, 0);

 loopEnd = i - spectrumValue[6];
 for (;i>loopEnd && i>0; i--)
  strip.setPixelColor(i,255,255,255);
  
  strip.show();

}


void musicRainbowLoop_3(){
 
 int i = 0, j= 0;
  
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

//grabs readings from chip
  for (i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
//    delayMicroseconds(30); // to allow the output to settle
    spectrumValue[i] = analogRead(analogPin);

    digitalWrite(strobePin, HIGH);
    
    //makes 10 bit value 8 bit
    spectrumValue[i] /= 4;
    
    if(spectrumValue[i] > 255)
     spectrumValue[i] = 255;

      if (spectrumValue[i] <= 55)
       spectrumValue[i] = 0;

     spectrumValue[i] = (int)(spectrumValue[i] / 7);

   
   for (j=0; j<27; j++)
    { 
    if (i == 0 && spectrumValue[i] >=j)
     nextLed(spectrumValue[i], 0,0, wait);
    else if (i == 1 && spectrumValue[i] >=j)
     nextLed(0, 255, 0, wait);
    else if (i == 2 && spectrumValue[i] >=j)
     nextLed(0,0, 255, wait);
    else if (i == 3 && spectrumValue[i] >=j)
     nextLed(255, 0, 255, wait);
    else if (i == 4 && spectrumValue[i] >=j)
     nextLed(0, 255,255, wait);
    else if (i == 5 && spectrumValue[i] >=j)
     nextLed(255, 255,0, wait);
    else if (i == 6 && spectrumValue[i] >=j)
     nextLed(255,255,255, wait);
    else
     nextLed(0,0,0, wait);
    }
    delay(2);
   }
}



void vuMeter_2(){
 
 int i = 0, j= 0,k =0;
  
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

//grabs readings from chip
  for (i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
//    delayMicroseconds(30); // to allow the output to settle
    spectrumValue[i] = analogRead(analogPin);

    digitalWrite(strobePin, HIGH);
    
    //makes 10 bit value 8 bit
    spectrumValue[i] /= 4;
    
    if(spectrumValue[i] > 255)
     spectrumValue[i] = 255;

      if (spectrumValue[i] <= 55)
       spectrumValue[i] = 0;

     spectrumValue[i] = (int)(spectrumValue[i] / 7);
     
     if (i == 0)
      spectrumValue[i] *= 1.5;
     else 
      spectrumValue[i] *= 1.2;
     
     

   
   for (j=0; j<27; j++)
    { 
    if (i == 0 && spectrumValue[i] >=j)
     strip.setPixelColor(k++,255, 0,0);
    else if (i == 1 && spectrumValue[i] >=j)
     strip.setPixelColor(k++,0, 255, 0);
    else if (i == 2 && spectrumValue[i] >=j)
     strip.setPixelColor(k++,0,0, 255);
    else if (i == 3 && spectrumValue[i] >=j)
     strip.setPixelColor(k++,255, 0, 255);
    else if (i == 4 && spectrumValue[i] >=j)
     strip.setPixelColor(k++,0, 255,255);
    else if (i == 5 && spectrumValue[i] >=j)
     strip.setPixelColor(k++,255, 255,0);
    else if (i == 6 && spectrumValue[i] >=j)
     strip.setPixelColor(k++,255,255,255);
    else
     strip.setPixelColor(k++,0,0,0);
    }
    delay(2);
   }
 
      strip.show();
}



void nextLed(int r, int g, int b, int wait){
static int currentLed = 0;


strip.setPixelColor(currentLed, r, g, b);
delay(wait);

//if (currentLed % 4 == 0)
 // strip.show();

currentLed++;
// currentLed += 4;

if (currentLed >= strip.numPixels() ){
  currentLed = 0;
  strip.show();
}
  
}
