#include <FastLED.h>
#include <colorutils.h>

FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif
#define DATA_PIN    5
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    25
CRGB leds[NUM_LEDS];
#define MAX_BRIGHTNESS     90
#define MIN_BRIGHTNESS     10
#define FRAMES_PER_SECOND  144
// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 3;     // the number of the pushbutton pin
const int ledPin =  LED_BUILTIN;      // the number of the LED pin for turning on and off, not the LEDs that will be changing colours
const int vibePin = 6;
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
long changemillis = 0;  // to check how often the button is changing

long lastReminder = 0;
long maxReminderTime = 20000;

int flashState = 0;
float flashPoint = 1.0;
int flashRateOn = 100;
int flashRateOff = 100;
long flashTime = 0;

void setup() {
  delay(3000); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(MIN_BRIGHTNESS);
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
  buttonState = digitalRead(buttonPin);
  
  float t = constrain(((millis() - lastReminder)*1.0) / ( maxReminderTime *1.0), 0.0, 1.5);
  Serial.println(t);

  //Change brightness as time goes on
  FastLED.setBrightness( constrain(round( MAX_BRIGHTNESS * t ), MIN_BRIGHTNESS, MAX_BRIGHTNESS));

  int red = constrain(round(t * 255), 0, 255);
  int green = 255 - constrain(round(t * 255), 0, 255);

  if(t > flashPoint)
  {
     Serial.println("Flashing");
     flash(CRGB(red, green, 0));
  }
  else
  {
    
     fill(CRGB(red, green, 0));
  }
  
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
  FastLED.setBrightness(MIN_BRIGHTNESS);
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
    fill(color);

    Serial.println(digitalRead(vibePin));
    
    digitalWrite(vibePin, HIGH);    
    if(flashTime >= flashRateOn)
    {
      flashState = 0;
      flashTime = 0;
    }
  }
}

void fill(CRGB color)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = color;
    }
}
