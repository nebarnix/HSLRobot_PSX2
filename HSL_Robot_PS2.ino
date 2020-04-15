//HeatSyncLabs Robot PS2X Controller
//SPECIAL THANKS TO BILL PORTER FOR BEING SO AWESOME

#include <PS2X_lib.h>  //for v1.6
#include <NeoSWSerial.h>

#define RF_TX_PIN 7
#define RF_RX_PIN 8
NeoSWSerial Serial1(RF_TX_PIN, RF_RX_PIN);


PS2X ps2x; // create PS2 Controller Class

#define JOY_DEADZONE 10
#define turnMultiplier 1.2

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you conect the controller,
//or call config_gamepad(pins) again after connecting the controller.
int error = 0;
byte type = 0;
byte vibrate = 0;

unsigned long int theTime = millis();
unsigned long int timeD1 = 0;
unsigned long int timeD2 = 0;
String cmd = "";

void setup() {
  Serial.begin(9600);  
  Serial1.begin(9600);  
  
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  //GamePad(clock, command, attention, data, Pressures?, Rumble?)
  //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(5, 2, 4, 3, true, true);
  /*
    if(error == 0){
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
    }

    else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

    else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

    else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

    //Serial.print(ps2x.Analog(1), HEX);

    type = ps2x.readType();
      switch(type) {
        case 0:
         Serial.println("Unknown Controller type");
        break;
        case 1:
         Serial.println("DualShock Controller Found");
        break;
        case 2:
          Serial.println("GuitarHero Controller Found");
        break;
      }
  */
}

void loop() {
  /* You must Read Gamepad to get new values
    Read GamePad and set vibration values
    ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
    if you don't enable the rumble, use ps2x.read_gamepad(); with no values

    you should call this at least once a second
  */

  int throttle = 100;


  if (error == 1) //skip loop if no controller found
    return;

/*
    //DualShock Controller

    //ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

    if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
         Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
         Serial.println("Select is being held");


     if(ps2x.Button(PSB_PAD_UP)) {         //will be TRUE as long as button is pressed
       Serial.print("Up held this hard: ");
       Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      }
      if(ps2x.Button(PSB_PAD_RIGHT)){
       Serial.print("Right held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
      }
      if(ps2x.Button(PSB_PAD_LEFT)){
       Serial.print("LEFT held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
      }
      if(ps2x.Button(PSB_PAD_DOWN)){
       Serial.print("DOWN held this hard: ");
     Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      }


      //vibrate = ps2x.Analog(PSS_LY);        //this will set the large motor vibrate speed based on
                                              //how hard you press the blue (X) button

    if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
    {



        if(ps2x.Button(PSB_L3))
         Serial.println("L3 pressed");
        if(ps2x.Button(PSB_R3))
         Serial.println("R3 pressed");
        if(ps2x.Button(PSB_L2))
         Serial.println("L2 pressed");
        if(ps2x.Button(PSB_R2))
         Serial.println("R2 pressed");
        if(ps2x.Button(PSB_GREEN))
         Serial.println("Triangle pressed");

    }


    if(ps2x.ButtonPressed(PSB_RED))             //will be TRUE if button was JUST pressed
         Serial.println("Circle just pressed");

    if(ps2x.ButtonReleased(PSB_PINK))             //will be TRUE if button was JUST released
         Serial.println("Square just released");
    if(ps2x.ButtonPressed(PSB_PINK))             //will be TRUE if button was JUST released
         Serial.println("Square just Pressed");

    if(ps2x.NewButtonState(PSB_BLUE))            //will be TRUE if button was JUST pressed OR released
         Serial.println("X just changed");


    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
    {
        //Serial.print("Stick Values:");
        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
        Serial.print(" ");
        Serial.print(ps2x.Analog(PSS_LX), DEC); //This one is at 132? The rest are 123 (strange not 127)
        Serial.print(" ");
        Serial.print(ps2x.Analog(PSS_RY), DEC);
        Serial.print(" ");
        Serial.println(ps2x.Analog(PSS_RX), DEC);
    }
  
  return; */
  
  ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

  if (ps2x.Button(PSB_L2)) //L2 speed boost
    throttle = 255;

  //if there no command waiting then look for buttons.
  //Else wait for the command to clear (or we might miss brief key presses!)

  if (cmd == "")
  {
    //This section is for movement commands. Anything that falls out at the end will result in the STOP command being issued to halt runaway motion.
    if (ps2x.Button(PSB_R2)) //mario kart style R2 accel button
    {
    cmd += "RAW=";
      cmd += throttle;
      cmd += ",";
      cmd += map(ps2x.Analog(PSS_LX),0,255,int(-throttle*0.65),int(throttle*0.65)); //limit side to side motion for controllability in mario kart mode
      cmd  += ';';  
    }
    else if (ps2x.Analog(PSS_LY) > (127+15) || ps2x.Analog(PSS_LY) < (127-15) || ps2x.Analog(PSS_LX) > (127+15) || ps2x.Analog(PSS_LX) < (127-15) )
    {
      cmd += "RAW=";
      cmd += map(ps2x.Analog(PSS_LY),255,0,-throttle,throttle);
      cmd += ",";
      cmd += map(ps2x.Analog(PSS_LX),0,255,-throttle,throttle);
      cmd  += ';';
    }
    else if (ps2x.Button(PSB_PAD_UP))        //will be TRUE as long as button is pressed
    {
      if (ps2x.Button(PSB_PAD_LEFT))
      {
        cmd  += "FWL=";
        cmd  += throttle;
        cmd  += ';';
      }
      else if (ps2x.Button(PSB_PAD_RIGHT))
      {
        cmd  += "FWR=";
        cmd  += throttle;
        cmd  += ';';
      }
      else
      {
        cmd  += "FWD=";
        cmd  += throttle;
        cmd  += ';';
      }
    }
    else if (ps2x.Button(PSB_PAD_DOWN))        //will be TRUE as long as button is pressed
    {
      if (ps2x.Button(PSB_PAD_LEFT))
      {
        cmd  += "RVL=";
        cmd  += throttle;
        cmd  += ';';
      }
      else if (ps2x.Button(PSB_PAD_RIGHT))
      {
        cmd  += "RVR=";
        cmd  += throttle;
        cmd  += ';';
      }
      else
      {
        cmd  += "REV=";
        cmd  += throttle;
        cmd  += ';';
      }
    }
    else if (ps2x.Button(PSB_PAD_LEFT))    //will be TRUE as long as button is pressed
    {
      cmd  += "LFT=";
      cmd  += int(constrain(throttle * turnMultiplier, 0, 255));
      cmd  += ';';
    }
    else if (ps2x.Button(PSB_PAD_RIGHT))   //will be TRUE as long as button is pressed
    {
      cmd  += "RGT=";
      cmd  += int(constrain(throttle * turnMultiplier, 0, 255));
      cmd  += ';';
    }
    else
      cmd += "STP;";

    //Other commands NOT pertaining to movement
    if (ps2x.Analog(PSS_RY) > (127+15) || ps2x.Analog(PSS_RY) < (127-15) || ps2x.Analog(PSS_RX) > (127+15) || ps2x.Analog(PSS_RX) < (127-15) )
    {
      cmd += "SRV=";
      cmd += map(ps2x.Analog(PSS_RY),127,0,0,90); //map center to 0
      cmd += ",";
      cmd += map(ps2x.Analog(PSS_RX),255,0,0,180); //map center to 90
      cmd  += ';';
    }
    else if(ps2x.Button(PSB_R3))
      cmd  += "SRV=0,90;";

    if (ps2x.Button(PSB_L1)) //L1 Headlight Toggle
      cmd  += "HLO;";

    if (ps2x.Button(PSB_BLUE)) //R1 "Not cool man!"
      cmd  += "NCM;";
    else if (ps2x.Button(PSB_PINK)) //R1 "Look at this!"
      cmd  += "LAT;";
    else if (ps2x.Button(PSB_RED)) //R1 Insult
      cmd  += "TNT;";
    else if (ps2x.Button(PSB_GREEN)) //R1 Insult
      cmd  += "CMP;";
    
  }

  if ((millis() - theTime) > 100) //loop constantly, but only send radio commands every so often
  {
    Serial.print(cmd);
    Serial1.print(cmd);
    cmd = "";
    theTime = millis();
  }
  else if ((millis() - theTime) > 100000) //don't let it overflow -- 100 seconds and reset the time
    theTime = millis();
}
