#include <SoftwareSerial.h>
SoftwareSerial pmsSerial(12, 11);
 #include <dht.h>
#include <FastLED.h>
#define NUM_LEDS 1
#define DATA_PIN 4
CRGB leds[NUM_LEDS];

    dht DHT;
    #define DHT22_PIN 5
float Mymessage[3]; //String data

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed

  // our debugging output
  Serial.begin(9600);
 
  // sensor baud rate is 9600
  pmsSerial.begin(9600);
}
 
struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};
 
struct pms5003data data;
    
void loop() {


  if (readPMSdata(&pmsSerial)) {
    // reading data was successful!


/*  Serial.println();
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (standard)");
    Serial.print("PM 1.0: "); Serial.print(data.pm10_standard);
    Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_standard);
    Serial.print("\t\tPM 10: "); Serial.println(data.pm100_standard);
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (environmental)");
    Serial.print("PM 1.0: "); Serial.print(data.pm10_env);
    Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_env);
    Serial.print("\t\tPM 10: "); Serial.println(data.pm100_env);
    Serial.println("---------------------------------------");
    Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(data.particles_03um);
    Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(data.particles_05um);
    Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(data.particles_10um);
    Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(data.particles_25um);
    Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(data.particles_50um);
    Serial.print("Particles > 10.0 um / 0.1L air:"); Serial.println(data.particles_100um);
    Serial.println("---------------------------------------"); */

    Mymessage[0]=data.pm10_standard;
    Mymessage[1]=data.pm25_standard;
        Serial.println("----------------PM 10-----------------------");
        Serial.println(Mymessage[0]);
        Serial.println("----------------PM 2.5-----------------------");
        Serial.println(Mymessage[1]);

  }

    if (DHT.read22(DHT22_PIN)) {
    Serial.print("ciaooooooo");
      Serial.print(DHT.humidity, 1);
      Serial.print(",\t");
      Serial.println(DHT.temperature, 1);
      Mymessage[2]=DHT.humidity;
      Mymessage[3]=DHT.temperature;
      // Turn the LED on, then pause
      leds[0] = CRGB::Green;
      FastLED.show();
      delay(500);
      // Now turn the LED off, then pause
      leds[0] = CRGB::Black;
      FastLED.show();
      delay(500);

      
    }

/*     Serial.print("DHT22, \t");
    int chk = DHT.read22(DHT22_PIN); 
      switch (chk)
      {
        case DHTLIB_OK:
                    Serial.print("OK,\t");
                    break;
        case DHTLIB_ERROR_CHECKSUM:
                    Serial.print("Checksum error,\t");
                    break;
        case DHTLIB_ERROR_TIMEOUT:
                    Serial.print("Time out error,\t");
                    break;
        default:
                    Serial.print("Unknown error,\t");
                    break;
      } */



  
  }
 
boolean readPMSdata(Stream *s) {
  if (! s->available()) {
    return false;
  }
  
  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }
 
  // Now read all 32 bytes
  if (s->available() < 32) {
    return false;
  }
    
  uint8_t buffer[32];    
  uint16_t sum = 0;
  s->readBytes(buffer, 32);
 
  // get checksum ready
  for (uint8_t i=0; i<30; i++) {
    sum += buffer[i];
  }
 
  /* debugging
  for (uint8_t i=2; i<32; i++) {
    Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
  }
  Serial.println();
  */
  
  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }
 
  // put it into a nice struct :)
  memcpy((void *)&data, (void *)buffer_u16, 30);
 
  if (sum != data.checksum) {
    Serial.println("Checksum failure");
    return false;
  }
  // success!
  return true;
}
























//////////TEMPLATE PER ESEGUIRE IL CODICE UNA VOLTA OGNI SECONDO ////////////

//unsigned long previousMillis = 0UL;
//unsigned long interval = 1000UL;

//void setup()
//{ /* do setup */ }

//void loop()
//{
  /* other code */

//  unsigned long currentMillis = millis();

//  if(currentMillis - previousMillis > interval)
//  {
	/* The Arduino executes this code once every second
 	*  (interval = 1000 (ms) = 1 second).
 	*/

 	// Don't forget to update the previousMillis value
// 	previousMillis = currentMillis;
//  }
