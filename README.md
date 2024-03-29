
# INTRODUCION

OPEN FARMER KIT
  Repository for developing the OFK (Open Farmer Kit) Environment. 
  The project has been developed only using Arduino UNO and WEMOS D1 ESP8266 boards. 


# REQUIREMENTS
  The entire environment is made up by 3 wireless & autonomous modules communicating with a main one that ghaters information about the sorrounding green conditions. Another wired module that sends air and temperature measures is wired-connected with the op server making the whole environment made up by 5 elements.
  The setup is made by:
    
  ## OPMODULE
  The op module requires:
    - Lora module (in this project we will use an Adafruit RFM900x Feather board)
    - SD module
    - an ESP8266 Board to upload files in cloud when wifi is detected

  ## AIR & TEMPERATURE MODULE
  The air Module module requires:
    - DHT22 board
    - PLANTOWER PMS5003 Laser pm sensor
    - Arduino Uno Board

  ## MOISTURE MODULE
    The Moisture Module requires:
      - Arduino uno Board
      - Capacitive Moist soil sensor 1.2
      - UBLOX Neo-6 module


  ## SOIL DISPLAY MODULE
      The Soil module requires:
      - An LCD 16X02 I2C module
      - Lora module (Adafruit RFM900x Feather board)
      - Rotary encoder
      - Arduino UNO Board
      - UBLOX Neo-6 module



  ## BIODIVERSITY MODULE
    The Bio Module requires:
      - A generic arduCam (In this project we will use an OV2642 board)
      - PIR module
      - SD module
      - Arduino UNO board

  ## LIBRARIES
All the different sketches require specific libraries that are explained in the sketch itself.
You can manage libraries through the library manager inside the arduino ide

# SETUP & WIRING
  In this section will be discussing about wiring and connections to get all the different modules working and communicating one with each other.

  ## OP MODULE
![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](/assets/getting_started/wiring/img/op_wiring.png)

  ## AIR & TEMP MODULE
![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](/assets/getting_started/wiring/img/air_temp_wiring.png)
  ## MOISTURE MODULE
![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](/assets/getting_started/wiring/img/soil_moisture_wiring.png)
  ## SOIL DISPLAY MODULE
![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](/assets/getting_started/wiring/img/soil_ph_display.png)
  ## BIODIVERSITY MODULE
![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](/assets/getting_started/wiring/img/biodiversity_wiring.png)

# CODING


  ## OP MODULE
      The Operative Module is the main base of the project. it develops

  ## AIR & TEMP MODULE
      The Operative Module is the main base of the project. it develops 

  ## MOISTURE MODULE
      The Operative Module is the main base of the project. it develops 

  ## SOIL DISPLAY MODULE
      The Operative Module is the main base of the project. it develops 

  ## BIODIVERSITY MODULE





# PRINTING FILES




