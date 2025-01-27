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
            if (msgi.id == 0x18FF1A03)
              {
                if ((msgi.buf[2])== 0x15)
                  {
                    myTime = millis();
                    if(myTime - lastpush > 1500) 
                      {
                        if (lastIdActive == 0)
                          {
                            Time = millis();
                            engageCAN = true;
                            lastIdActive = 1;
                            relayTime = ((millis() + 1000));
                            lastpush = Time;
                          }
                       else
                          {
                            engageCAN = false;
                            lastIdActive = 0;
                          }
                    }
                }
              }
          }
}
