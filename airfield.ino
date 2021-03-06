#include <Adafruit_NeoPixel.h>

/****************************************
Example Sound Level Sketch for the 
Adafruit Microphone Amplifier
****************************************/

#define SAMPLES 5
#define N_PIXELS  150  // Number of pixels in strand
#define MIC_PIN   A0  // Microphone is attached to this analog pin
#define LED_PIN    6  // NeoPixel LED strand is connected to this pin

const int sampleWindow = 20; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

int vol[SAMPLES];
int volCount = 0;
int total = 0;
int avg = 0;
int avg_int = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

 
void setup() 
{
   Serial.begin(9600);
   analogReference(EXTERNAL);
   memset(vol, 0, sizeof(vol));
   strip.begin();
}
 
 
void loop() 
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(MIC_PIN);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
//   int volts = (peakToPeak * 3.3) / 1024;  // convert to volts

   total = total - vol[volCount];
   vol[volCount] = peakToPeak;                      // Save sample for dynamic levelin
   total = total + vol[volCount];
   volCount += 1;
   if(volCount >= SAMPLES) volCount = 0; // Advance/rollover sample counter
   avg = total / SAMPLES;
   avg_int = map(constrain(avg, 100, 900), 100, 900, 0, 255);
//   Serial.print(avg);
//   Serial.print(",");

    uint32_t color = strip.Color(0, 0, 0);
    
    if (avg_int < 32) {
      color = strip.Color(255, 255, 255); // white  
    } else if (avg_int < 64) {
      color = strip.Color(0, 0, 102); // blue
    } else if (avg_int < 96) {
      color = strip.Color(76, 153, 0); // verde
    } else if (avg_int < 128) {
      color = strip.Color(0, 204, 204); // turcoaz
    } else if (avg_int < 160) {
      color = strip.Color(255, 0, 127); 
    } else if (avg_int < 192) {
      color = strip.Color(204, 0, 0);
    } else if (avg_int < 224) {
      color = strip.Color(255, 0, 0);
    } else {
      color = strip.Color(255, 51, 255);
    }
    
    for (int i = 0; i < N_PIXELS; i++) {
      strip.setPixelColor(i, color);
    }
    strip.show();
   Serial.print(avg_int);
   Serial.println();
}
