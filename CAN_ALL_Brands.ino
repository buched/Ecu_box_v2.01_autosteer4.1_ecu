float decimal_input;
float unit_input;

int32_t vbauds;
void CAN_setup (void) {
if (canimu.CanBauds == 1)
  {
    vbauds = 125000;
  }
else if (canimu.CanBauds == 2)
  {
    vbauds = 250000;
  }
else if (canimu.CanBauds == 3)
  {
    vbauds = 500000;
  }
else if (canimu.CanBauds == 4)
  {
    vbauds = 1000000;
  }
//V_Bus is CAN-3 and is the Steering BUS
  V_Bus.begin();
  V_Bus.setBaudRate(vbauds);
  //V_Bus.enableFIFO();
  //V_Bus.setFIFOFilter(REJECT_ALL);
  //V_Bus.setFIFOFilter(0, 0x18FF1A03, EXT);  //Claas Curve Data & Valve State Message
  delay(500);
} //End CAN SETUP

//---Receive K_Bus message
void VBus_Receive()
{
CAN_message_t msgiv;
      if ( V_Bus.read(msgiv) ) 
          {
            if (msgiv.id == 0x18FF1A03)
              {        
                if ((msgiv.buf[2])== 0x15)
                  {
                    Time = millis();
                    engageCAN = 1;
                    relayTime = ((millis() + 1000));
                   }

               }
          }
  if (msgiv.id == 0x585)
    {
      decimal_input = (msgiv.buf[2]);
      decimal_input = decimal_input /152 ;
      unit_input = (msgiv.buf[1]);
      unit_input = unit_input *1000;
      unit_input = unit_input /152;
      roll = unit_input + decimal_input;
          if (msgiv.buf[0] >= 10)
          {
            roll *= -1;
          }
          //Serial.println(roll);
    }
}



void serv() {
  Serial.println("\r\nAgOpenGPS CANBUS Service Tool Mode:");
  Help();
   while (Service == 1) 
  { 
      if (Serial.available())   // Read Data From Serail Monitor 
      {    
        byte b = Serial.read();
        if ( b == '?') Help();          
        else if ( b == 'X') Service = 0; //Exit Service Mode
        else if ( b == '0') startsetup();
        else if ( b == '1') startsetupp();
        else if ( b == '2') activate();
        else if ( b == '3') deactivate();
        else
        {
          Serial.println("No command, send ? for help");
          Serial.println(" ");
          delay(50);
        }

        while (Serial.available())
        {
        Serial.read();                //Clear the serial buffer
        }
      }
  }
}

void Help() {
  Serial.println("X = service mode exit");
  Serial.println("0 = start setup with 250k baud");
  Serial.println("1 = start setup with 500k baud");
  Serial.println("2 = can imu activation in ino");
  Serial.println("3 = can imu desactivation in ino");
  }

void startsetup() {
  delay(2000);
  dixhtz();
  delay(2000);
  relativeangle();
  delay(2000);
  save();
  delay(2000);
  setcanbaudrate();
  }

void startsetupp() {
  delay(2000);
  dixhtz();
  delay(2000);
  relativeangle();
  delay(2000);
  save();
  delay(2000);
  setcanbaudratee();
  }

void dixhtz() {
  msgi.id = 0x605;
  msgi.flags.extended = false;
  msgi.len = 8;
  msgi.buf[0] = 0x40;
  msgi.buf[1] = 0x0c;
  msgi.buf[2] = 0x10;
  msgi.buf[3] = 0x00;
  msgi.buf[4] = 0x02;
  msgi.buf[5] = 0x00;
  msgi.buf[6] = 0x00;
  msgi.buf[7] = 0x00;
  V_Bus.write(msgi);
  
  Serial.println("IMU Can set to 10 htz");
}

void relativeangle() {
  msgi.id = 0x605;
  msgi.flags.extended = false;
  msgi.len = 8;
  msgi.buf[0] = 0x40;
  msgi.buf[1] = 0x05;
  msgi.buf[2] = 0x10;
  msgi.buf[3] = 0x00;
  msgi.buf[4] = 0x01;
  msgi.buf[5] = 0x00;
  msgi.buf[6] = 0x00;
  msgi.buf[7] = 0x00;
  V_Bus.write(msgi);
  
  Serial.println("IMU Can set in relative angle mode");
}

void save() {
  msgi.id = 0x605;
  msgi.flags.extended = false;
  msgi.len = 8;
  msgi.buf[0] = 0x40;
  msgi.buf[1] = 0x0a;
  msgi.buf[2] = 0x10;
  msgi.buf[3] = 0x00;
  msgi.buf[4] = 0x00;
  msgi.buf[5] = 0x00;
  msgi.buf[6] = 0x00;
  msgi.buf[7] = 0x00;
  V_Bus.write(msgi);
  
  Serial.println("IMU settings saved");
}

void setcanbaudrate() {
  msgi.id = 0x605;
  msgi.flags.extended = false;
  msgi.len = 8;
  msgi.buf[0] = 0x40;
  msgi.buf[1] = 0x20;
  msgi.buf[2] = 0x10;
  msgi.buf[3] = 0x00;
  msgi.buf[4] = 0x02;
  msgi.buf[5] = 0x00;
  msgi.buf[6] = 0x00;
  msgi.buf[7] = 0x00;
  V_Bus.write(msgi);
  
  canimu.CanBauds = 2;
   //store in EEPROM
  EEPROM.put(90, canimu);
  
  Serial.println("CAN bauds set to 250k");
}

void setcanbaudratee() {
  msgi.id = 0x605;
  msgi.flags.extended = false;
  msgi.len = 8;
  msgi.buf[0] = 0x40;
  msgi.buf[1] = 0x20;
  msgi.buf[2] = 0x10;
  msgi.buf[3] = 0x00;
  msgi.buf[4] = 0x01;
  msgi.buf[5] = 0x00;
  msgi.buf[6] = 0x00;
  msgi.buf[7] = 0x00;
  V_Bus.write(msgi);

    canimu.CanBauds = 3;
   //store in EEPROM
  EEPROM.put(90, canimu);
  
  Serial.println("CAN bauds set to 500k");
}

void activate() {
      canimu.UseImuCan = 1;
   //store in EEPROM
  EEPROM.put(90, canimu);
  }

void deactivate() {
      canimu.UseImuCan = 0;
   //store in EEPROM
  EEPROM.put(90, canimu);
  }
