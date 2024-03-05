
#include <Arduino.h>
#include <RotaryEncoder.h>
#include <Versatile_RotaryEncoder.h>
#include <I2C_LCD.h>
#include <NMEAGPS.h>
#include <FastLED.h>
#include <GPSport.h>

#define PIN_IN1 6
#define PIN_IN2 7

#define ROTARYSTEPS .5
#define ROTARYMIN 1
#define ROTARYMAX 9

RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);
int lastPos = 0;

// SET READING PINS ACCORDINGLY TO YOUR ENCODER TO BOARD CONNECTIONS
// Set here your encoder reading pins (Ex.: EC11 with breakout board)
#define clk 7  // (A3)
#define dt 6   // (A2)
#define sw 5   // (A4)

// Functions prototyping to be handled on each Encoder Event
void handleRotate(int8_t rotation);
void handlePress();
void handleDoublePress();

// Create a global pointer for the encoder object
Versatile_RotaryEncoder *versatile_encoder;


#define BACKLIGHT_PIN   3
#define En_pin          2
#define Rw_pin          1
#define Rs_pin          0
#define D4_pin          4
#define D5_pin          5
#define D6_pin          6
#define D7_pin          7

I2C_LCD lcd(39);

#define NUM_LEDS 1
#define DATA_PIN 4
CRGB leds[NUM_LEDS];

float measures;
int counterClick;


#if !defined( NMEAGPS_PARSE_RMC )
  #error You must uncomment NMEAGPS_PARSE_RMC in NMEAGPS_cfg.h!
#endif

#if !defined( GPS_FIX_TIME )
  #error You must uncomment GPS_FIX_TIME in GPSfix_cfg.h!
#endif

#if !defined( GPS_FIX_LOCATION )
  #error You must uncomment GPS_FIX_LOCATION in GPSfix_cfg.h!
#endif

#if !defined( GPS_FIX_SPEED )
  #error You must uncomment GPS_FIX_SPEED in GPSfix_cfg.h!
#endif

#if !defined( GPS_FIX_SATELLITES )
  #error You must uncomment GPS_FIX_SATELLITES in GPSfix_cfg.h!
#endif

#ifdef NMEAGPS_INTERRUPT_PROCESSING
  #error You must *NOT* define NMEAGPS_INTERRUPT_PROCESSING in NMEAGPS_cfg.h!
#endif

static NMEAGPS  gps; // This parses the GPS characters



static void printL( Print & outs, int32_t degE7 );
static void printL( Print & outs, int32_t degE7 )
{
  // Extract and print negative sign
  if (degE7 < 0) {
    degE7 = -degE7;
    outs.print( '-' );
  }

  // Whole degrees
  int32_t deg = degE7 / 10000000L;
  outs.print( deg );
  outs.print( '.' );

  // Get fractional degrees
  degE7 -= deg*10000000L;

  // Print leading zeroes, if needed
  int32_t factor = 1000000L;
  while ((degE7 < factor) && (factor > 1L)){
    outs.print( '0' );
    factor /= 10L;
  }
  
  // Print fractional degrees
  outs.print( degE7 );
}

static void doSomeWork( const gps_fix & fix );
static void doSomeWork( const gps_fix & fix )
{
  //  This is the best place to do your time-consuming work, right after
  //     the RMC sentence was received.  If you do anything in "loop()",
  //     you could cause GPS characters to be lost, and you will not
  //     get a good lat/lon.
  //  For this example, we just print the lat/lon.  If you print too much,
  //     this routine will not get back to "loop()" in time to process
  //     the next set of GPS data.


  if (!fix.valid.location) {
    leds[0] = CRGB::Red;
      FastLED.show();
      delay(500);
      // Now turn the LED off, then pause
      leds[0] = CRGB::Black;
      FastLED.show();
      delay(500); 

  }
  
  if (fix.valid.location) {
      leds[0] = CRGB::Green;
      FastLED.show();
      delay(500);
      // Now turn the LED off, then pause
      leds[0] = CRGB::Black;
      FastLED.show();
      delay(500); 


    if ( fix.dateTime.seconds < 10 )
      DEBUG_PORT.print( '0' );
    DEBUG_PORT.print( fix.dateTime.seconds );
    DEBUG_PORT.print( ',' );

    // DEBUG_PORT.print( fix.latitude(), 6 ); // floating-point display
    // DEBUG_PORT.print( fix.latitudeL() ); // integer display
    printL( DEBUG_PORT, fix.latitudeL() ); // prints int like a float
    DEBUG_PORT.print( ',' );
    // DEBUG_PORT.print( fix.longitude(), 6 ); // floating-point display
    // DEBUG_PORT.print( fix.longitudeL() );  // integer display
    printL( DEBUG_PORT, fix.longitudeL() ); // prints int like a float

    DEBUG_PORT.print( ',' );
    if (fix.valid.satellites)
      DEBUG_PORT.print( fix.satellites );

    DEBUG_PORT.print( ',' );
    DEBUG_PORT.print( fix.speed(), 6 );
    DEBUG_PORT.print( F(" kn = ") );
    DEBUG_PORT.print( fix.speed_mph(), 6 );
    DEBUG_PORT.print( F(" mph") );

  } else {
    // No valid location data yet!
    DEBUG_PORT.print( '?' );
  }

  DEBUG_PORT.println();

} // doSomeWork

//------------------------------------

static void GPSloop();
static void GPSloop()
{
  while (gps.available( gpsPort ))
    doSomeWork( gps.read() );
  

} // GPSloop
  
//--------------------------



void setup() {
counterClick=-1;
    Serial.begin(9600);
	versatile_encoder = new Versatile_RotaryEncoder(clk, dt, sw);

    // Load to the encoder all nedded handle functions here (up to 9 functions)
    versatile_encoder->setHandleRotate(handleRotate);
    versatile_encoder->setHandlePress(handlePress);
    versatile_encoder->setHandleDoublePress(handleDoublePress);

    Serial.println("Ready!");
      encoder.setPosition( 7/ROTARYSTEPS); // start with the value of 10.

      while (!Serial);
  Serial.println(__FILE__);
  Serial.print("I2C_LCD_LIB_VERSION: ");
  Serial.println(I2C_LCD_LIB_VERSION);
  Serial.println();

  lcd.config(39, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin, BACKLIGHT_PIN, POSITIVE);

  Wire.begin();
  Wire.setClock(9600);
  lcd.begin(16, 2);

  lcd.display();
  delay(1000);
  lcd.noDisplay();
  delay(1000);
  lcd.display();

  lcd.clear();

  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("welcome to OFK");

    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed

      DEBUG_PORT.begin(9600);
  while (!DEBUG_PORT);

  DEBUG_PORT.print( F("NMEAloc.INO: started\n") );
  DEBUG_PORT.print( F("fix object size = ") );
  DEBUG_PORT.println( sizeof(gps.fix()) );
  DEBUG_PORT.print( F("NMEAGPS object size = ") );
  DEBUG_PORT.println( sizeof(gps) );
  DEBUG_PORT.println( F("Looking for GPS device on " GPS_PORT_NAME) );

  #ifdef NMEAGPS_NO_MERGING
    DEBUG_PORT.println( F("Only displaying data from xxRMC sentences.\n  Other sentences may be parsed, but their data will not be displayed.") );
  #endif
  DEBUG_PORT.flush();
  gpsPort.begin(9600);

}

void loop() {
          encoder.tick();
        GPSloop();
    // Do the encoder reading and processing
    if (versatile_encoder->ReadEncoder()) {
    }


}

// Implement your functions here accordingly to your needs

void handleRotate(int8_t rotation) {
	// get the current physical position and calc the logical position
  int newPos = encoder.getPosition() * ROTARYSTEPS;
  if (newPos < ROTARYMIN) {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    newPos = ROTARYMIN;
  } else if (newPos > ROTARYMAX) {
    encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
    newPos = ROTARYMAX;
  } // if
  if (lastPos != newPos) {
    Serial.print(newPos);
    Serial.println();
    lastPos = newPos;
    lcd.setCursor(7, 1);
    lcd.display();
    lcd.print(newPos);
    
  } // if

  if(counterClick == -1 || counterClick == 5){
    lcd.clear();
  }
}


void handlePress() {
    counterClick++;

  switch (counterClick) {

  case 0:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Valore pH");
  	Serial.print("Ph e l'indice dell'array è:");
    Serial.println(counterClick);
    break;

  case 1:
    lcd.setCursor(0, 0); 
    lcd.clear();
    lcd.print("Valore N");
    Serial.print("n e l'indice dell'array è:");
    Serial.println(counterClick);
    break;

  case 2:
    lcd.setCursor(0, 0); 
    lcd.clear();
    lcd.print("Valore P");
    Serial.print("n e l'indice dell'array è:");
    Serial.println(counterClick);
    break;

  case 3:
    lcd.setCursor(0, 0); 
    lcd.clear();
    lcd.print("Valore K");
    Serial.print("n e l'indice dell'array è:");
    Serial.println(counterClick);
    break;

  default:
    lcd.setCursor(0, 0); 
    lcd.clear();
    lcd.print("welcome to OFK");
    Serial.print("n e l'indice dell'array è:");
    Serial.println(counterClick);
    break;
}

    if ( counterClick==4 ){
      
         	lcd.clear();
          lcd.setCursor(0, 0); 
          lcd.print("Invio dati in ");
          Serial.println("Sending data...");
          
          lcd.setCursor(0, 1);
           lcd.print("corso. Attendi");
          Serial.println("Sending data...");
          Serial.println(counterClick);

          delay(2500);

          lcd.clear();
          lcd.setCursor(0, 0); 
          lcd.print("Invio dati ");
          Serial.print("Sending data...");
          
          lcd.setCursor(0, 1);
           lcd.print("completo. :)");
          Serial.print("Sending data...");
          Serial.print(counterClick);

            counterClick=-1;

    }


}

void handleDoublePress() {
  counterClick=-1;
  lcd.clear();
  lcd.home();
}
