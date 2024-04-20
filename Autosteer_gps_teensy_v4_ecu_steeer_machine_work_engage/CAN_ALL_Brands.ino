void CAN_setup (void) {

//V_Bus is CAN-3 and is the Steering BUS
  V_Bus.begin();
  V_Bus.setBaudRate(250000);
  //V_Bus.enableFIFO();
  //V_Bus.setFIFOFilter(REJECT_ALL);
  //V_Bus.setFIFOFilter(0, 0x18FF1A03, EXT);  //Claas Curve Data & Valve State Message
  delay(500);
} //End CAN SETUP

//---Receive K_Bus message
void VBus_Receive()
{
CAN_message_t msgi;
      if ( V_Bus.read(msgi) ) 
          {
            if (msgi.id == 0x18FFB306)
              {        
                if ((msgi.buf[2])== 0x1F)
                  {
                    Time = millis();
                    engageCAN = 1;
                    relayTime = ((millis() + 1000));
                   }
               }
             if (msgi.id == 0x18FE4523)
               {
                  KBUSRearHitch = (msgi.buf[0]); 
                  pressureReading = KBUSRearHitch;
                  if (KBUSRearHitch < steerConfig.PulseCountMax) workCAN = 1; 
                  else workCAN = 0; 
               }
          }
}
