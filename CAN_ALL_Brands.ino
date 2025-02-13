void CAN_setup (void) {

//V_Bus is CAN-3 and is the Steering BUS
  V_Bus.begin();
  V_Bus.setBaudRate(250000);
    switch (Brand)
    {
      case 0:
          V_Bus.setFIFOFilter(0, 0x18EF1CD2, EXT);  //Claas Engage Message
          V_Bus.setFIFOFilter(1, 0x1CFFE6D2, EXT);  //Claas Work Message (CEBIS Screen MR Models)
          break;
      case 1:
          V_Bus.setFIFOFilter(0, 0x18EF1C32, EXT);  //Valtra Engage Message
          V_Bus.setFIFOFilter(1, 0x18EF1CFC, EXT);  //Mccormick Engage Message
          V_Bus.setFIFOFilter(2, 0x18EF1C00, EXT);  //MF Engage Message
        break;
      case 2:
          V_Bus.setFIFOFilter(0, 0x14FF7706, EXT);  //CaseIH Engage Message
          V_Bus.setFIFOFilter(1, 0x18FE4523, EXT);  //CaseIH Rear Hitch Infomation
          V_Bus.setFIFOFilter(2, 0x18FF1A03, EXT);  //CaseIH Engage Message
          V_Bus.setFIFOFilter(3, 0x18FFB031, EXT);  //CaseIH Engage Message
        break;
      case 3:
          V_Bus.setFIFOFilter(0, 0x613, STD);  //Fendt Engage
        break;
      case 4:
          V_Bus.setFIFOFilter(0, 0x18EFAB27, EXT);  //JCB engage message
        break;
      case 5:
          V_Bus.setFIFOFilter(0, 0xCFFD899, EXT);  //FendtOne Engage
        break;
      default:
        Serial.println("No brand selected");
        break;
    }
  //V_Bus.enableFIFO();
  //V_Bus.setFIFOFilter(REJECT_ALL);
  //V_Bus.setFIFOFilter(0, 0x18FF1A03, EXT);  //Claas Curve Data & Valve State Message
  delay(500);
} //End CAN SETUP

//---Receive V_Bus message
void VBus_Receive()
{
CAN_message_t msgi;
  if ( V_Bus.read(msgi)) 
    {
      if (Brand == 1)
        {
              if (msgi.id == 0x18EF1C32)
              {
                  if ((msgi.buf[0])== 15 && (msgi.buf[1])== 96 && (msgi.buf[2])== 1)
                  {
                    eng();
                  }
              } 
  
              if (msgi.id == 0x18EF1CFC)//Mccormick engage message
              {
                  if ((msgi.buf[0])== 15 && (msgi.buf[1])== 96 && (msgi.buf[3])== 255)
                  {   
                    eng();
                  }
              } 
              if (msgi.id == 0x18EF1C00)//MF engage message
              {
                  if ((msgi.buf[0])== 15 && (msgi.buf[1])== 96 && (msgi.buf[2])== 1)
                  {   
                    eng();
                  }
              }
          
        }
      if (Brand == 2)
        {
          if (msgi.id == 0x14FF7706)
            {
              if ((msgi.buf[0]) == 130 && (msgi.buf[1]) == 1)
                {
                  eng();
                }
              if ((msgi.buf[0])== 178 && (msgi.buf[1])== 4)
                {
                  eng();
                }
            }
           if (msgi.id == 0x18FE4523)
                {
                  KBUSRearHitch = (msgi.buf[0]);
                  if (KBUSRearHitch < aogConfig.user1) workCAN = 1;
                  else workCAN = 0;
                }
           if (msgi.id == 0x18FF1A03)
            {
              if (msgi.buf[2]==0x15)
                {
                      eng();
                }  
            }
          if (msgi.id == 0x18FFB031)
            {
              if ((msgi.buf[2]) == 0xD0)
                {
                  eng();
                }
            }
        }
      if (Brand == 3)
        {
          if (msgi.id == 0x613)
            {
              if (msgi.buf[0]==0x15 && msgi.buf[2]==0x06 && msgi.buf[3]==0xCA)
                {
                  if (msgi.buf[1]==0x88 && msgi.buf[4]==0x80) // Fendt Auto Steer Go   
                    {
                      eng();
                    }
                }  
            }
        }
     if (Brand == 4)
        {
            if (msgi.id == 0x18EFAB27)
            {
                if ((msgi.buf[0])== 15 && (msgi.buf[1])== 96 && (msgi.buf[2])== 1)
                {
                  eng();
                }
            }    
   
        }
      if (Brand == 5)
      {
          if (msgi.id == 0xCFFD899)   //**FendtOne Engage Message**  
          {
            if ((msgi.buf[3])== 0xF6)
            {   
                eng();
            }
          }
      }
    }
}


void eng() 
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
