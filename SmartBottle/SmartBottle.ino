#include <FastLED.h>
#include <colorutils.h>

FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif
#define DATA_PIN    5
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    16
CRGB leds[NUM_LEDS];
#define MAX_BRIGHTNESS     255
#define MIN_BRIGHTNESS     10
#define FRAMES_PER_SECOND  144
// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 3;     // the number of the pushbutton pin
const int ledPin =  LED_BUILTIN;      // the number of the LED pin for turning on and off, not the LEDs that will be changing colours
const int vibePin = 6;
uint8_t gHue = 0;
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
long changemillis = 0;  // to check how often the button is changing

long lastReminder = 0;
long maxReminderTime = 20000;
long fadeTime;
bool fading = false;

int flashState = 0;
float flashPoint = 1.0;
int flashRateOn = 100;
int flashRateOff = 100;
long flashTime = 0;

void setup() {
  delay(3000); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(vibePin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  // insert a delay to keep the framerate modest
  //FastLED.delay(1000 / FRAMES_PER_SECOND);
  //delay(6000);
  buttonState = digitalRead(buttonPin);
  //counter++;
  //Serial.println(counter);
  float t = constrain(((millis() - lastReminder)*1.0) / ( maxReminderTime *1.0), 0.0, 1.5);
  Serial.println(t);

  //Change brightness as time goes on
  //FastLED.setBrightness( constrain(round( MAX_BRIGHTNESS * t ), MIN_BRIGHTNESS, MAX_BRIGHTNESS));

  int blue = constrain(round(t * 255), 0, 255);
  int green = 255 - constrain(round(t * 255), 0, 255);

  if(t >= 1.5){
      flash(CRGB :: Blue);
  } else if(t >=0.0 && t < 0.50){
    fill(CRGB :: Green);
  } else if(t >=0.50 && t < 0.53){
    fadeToBlack();
  } else if(t >=0.53 && t < 0.56){
    fadeToYellow();
  } else if(t >= 0.56 && t < 1.0){
    fill(CRGB :: Yellow);
  } else if(t >= 1.0 && t < 1.03){
    fadeToBlack();
  } else if(t >= 1.03 && t < 1.06){
    fadeToBlue();
  } else if(t >= 1.06 && t < 1.5){
    RunningLights(0x00,0x00,0xff, 120);
  } else{
    //Error
    flash(CRGB :: Red);
  }

/*
  if(t > flashPoint)
  {
     Serial.println("Flashing");
     flash(CRGB(0, green, red));
  }
  else 
  if(t >= 0.0 && t < 0.25){
    fill(CRGB(red, green, 0));
  }
  else if(t >= 0.25 && t < 0.5){
    fill_3(CRGB(red, green, 0));
  }
  else if(t >= 0.5 && t < 0.75){
    fill_2(CRGB(red, green, 0));
  }
  else if(t >= 0.75 && t < 0.95){
    fill_1(CRGB(red, green, 0));
  }
  else{
     sinelon();
     //fill(CRGB(0, green, red));
  }
  */
  
  FastLED.show();


  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == LOW) {
    // turn LED on:
    digitalWrite(ledPin, LOW);
    if (millis() > changemillis + 2000) {
      resetTimer();
      changemillis = millis();
    }
  } else {
    // turn LED off:
    digitalWrite(ledPin, HIGH);
  }
}

void resetTimer()
{
  lastReminder = millis();
  FastLED.setBrightness(MAX_BRIGHTNESS);
  fill(CRGB::Green);
  flashPoint = 1.0;
  flashState = 0;
  flashTime = 0;
  digitalWrite(vibePin, LOW);    

}

void flash(CRGB color)
{
  flashTime = flashTime + 1;
  
  if(flashState == 0)
  {
    fill(CRGB::Black);
    digitalWrite(vibePin, LOW);
    Serial.println(digitalRead(vibePin));

    if(flashTime >= flashRateOff)
    {
      flashState = 1;
      flashTime = 0;
    }
  }
  else
  {
    fill(CRGB :: Blue);

    Serial.println(digitalRead(vibePin));
    
    digitalWrite(vibePin, HIGH);    
    if(flashTime >= flashRateOn)
    {
      flashState = 0;
      flashTime = 0;
    }
  }
}

void fill(CRGB colour){
  for(int i = 0; i < NUM_LEDS; i++){
      leds[i] = colour;
  }
}
void fadeToBlack(){
  if(!fading)
  {
    fadeTime = millis();
    fading = true;
  }
  float dt = (millis() - fadeTime) / 600.0;
  Serial.println(dt);

  int brightness =  constrain(round( MAX_BRIGHTNESS * (1 - dt) ), 0, MAX_BRIGHTNESS);
  FastLED.setBrightness(brightness);
 
  Serial.println(brightness);
  
}

void fadeToYellow(){
  if(fading)
  {
    fadeTime = millis();
    fading = false;
  }
  fill(CRGB::Yellow);
  float dt = (millis() - fadeTime) / 600.0;
  Serial.println(dt);

  int brightness =  constrain(round( MAX_BRIGHTNESS * dt ), 0, MAX_BRIGHTNESS);
  FastLED.setBrightness(brightness);
 
  Serial.println(brightness);
}

void fadeToBlue(){
  if(fading)
  {
    fadeTime = millis();
    fading = false;
  }
  fill(CRGB::Blue);
  float dt = (millis() - fadeTime) / 600.0;
  Serial.println(dt);

  int brightness =  constrain(round( MAX_BRIGHTNESS * dt ), 0, MAX_BRIGHTNESS);
  FastLED.setBrightness(brightness);
 
  Serial.println(brightness);
}
/*
void fill(CRGB color)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = color;
    }
}

void fill_3(CRGB color){
  for(int i = 0; i < NUM_LEDS - 6; i++){
    leds[i] = color;
  }
  for(int i = NUM_LEDS - 6; i < NUM_LEDS; i ++){
    leds[i] = CRGB :: Blue;
  }
}

void fill_2(CRGB color){
  for(int i = 0; i < NUM_LEDS - 12; i++){
    leds[i] = color;
  }
  for(int i = NUM_LEDS - 12; i < NUM_LEDS; i ++){
    leds[i] = CRGB :: Blue;
  }
}

void fill_1(CRGB color){
  for(int i = 0; i < NUM_LEDS - 18; i ++){
    leds[i] = color;
  }
  for(int i = NUM_LEDS - 18; i < NUM_LEDS; i++){
    leds[i] = CRGB :: Blue;
  }
}
*/
void sinelon()
{
  fadeToBlackBy( leds, (NUM_LEDS/2), 20);
 int pos = beatsin16( 600, 0, (NUM_LEDS/2)-1);
 leds[pos] += CRGB :: Blue;
}

void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
  
  for(int j=0; j<NUM_LEDS*2; j++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<NUM_LEDS; i++) {
        leds[i]= CRGB(((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
      
    FastLED.show();
    delay(WaveDelay);
  }
}
