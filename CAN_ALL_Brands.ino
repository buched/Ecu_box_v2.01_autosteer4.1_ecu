void CAN_setup (void) {

//V_Bus is CAN-3 and is the Steering BUS
  V_Bus.begin();
  V_Bus.setBaudRate(250000);
  R_Bus.begin();
  R_Bus.setBaudRate(250000);
  V_Bus.enableFIFO();
  V_Bus.setFIFOFilter(REJECT_ALL);
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
          V_Bus.setFIFOFilter(0, 0x14FF7706, EXT);
          V_Bus.setFIFOFilter(1, 0x18FE4523, EXT);
        break;
      case 3:
          V_Bus.setFIFOFilter(0, 0x0CEF2CF0, EXT);
        break;
      case 4:
          V_Bus.setFIFOFilter(0, 0x18EFAB27, EXT);
        break;
      default:
        Serial.println("No brand selected");
        break;
    }  
//  R_Bus.enableFIFO();
//  R_Bus.setFIFOFilter(REJECT_ALL);
//  R_Bus.setFIFOFilter(0, 0x18FE4523, EXT);
  delay(500);
} //End CAN SETUP

//---Receive V_Bus message
void VBus_Receive()
{
  CAN_message_t msgi;
      if ( V_Bus.read(msgi) )
        {
        if (Brand == 0)
        {
          //**Engage Message**
          if (msgi.id == 0x18EF1CD2)
          {
            if ((msgi.buf[1])== 0 && (msgi.buf[2])== 0)   //Ryan Stage5 Models?
            {
                      myTime = millis();
                      if(myTime - lastpush > 1500)
                        {
                          if (lastIdActive == 0)
                            {
                              Time = millis();
                              engageCAN = bitRead(msgi.buf[0],2);
                              //engageCAN = true;
                              lastIdActive = 1;
                              relayTime = ((millis() + 1000));
                              lastpush = Time;
                            }
                      }      
            }

            else if ((msgi.buf[0]) == 39 && (msgi.buf[2]) == 241)   //Ryan MR Models?
              {
                      myTime = millis();
                      if(myTime - lastpush > 1500)
                        {
                          if (lastIdActive == 0)
                            {
                              Time = millis();
                              engageCAN = bitRead(msgi.buf[1],0);
                              //engageCAN = true;
                              lastIdActive = 1;
                              relayTime = ((millis() + 1000));
                              lastpush = Time;
                            }
                      }      
              }
  
            else if ((msgi.buf[1])== 0 && (msgi.buf[2])== 125) //Tony Non MR Models? Ryan Mod to bit read engage bit
              {
                      myTime = millis();
                      if(myTime - lastpush > 1500)
                        {
                          if (lastIdActive == 0)
                            {
                              Time = millis();
                              engageCAN = bitRead(msgi.buf[0],2);
                              //engageCAN = true;
                              lastIdActive = 1;
                              relayTime = ((millis() + 1000));
                              lastpush = Time;
                            }
                      }         
              }
          } 

          //**Work Message**
          if (msgi.id == 0x1CFFE6D2)
          {
            if ((msgi.buf[0])== 144)
            {
             workCAN = bitRead(msgi.buf[6],0);
            }
          }
        }


        if (Brand == 1)
        {
            if (msgi.id == 0x18EF1C32)
            {
                if ((msgi.buf[0])== 15 && (msgi.buf[1])== 96 && (msgi.buf[2])== 1)
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
                      }
                }
            } 

            if (msgi.id == 0x18EF1CFC)//Mccormick engage message
            {
                if ((msgi.buf[0])== 15 && (msgi.buf[1])== 96 && (msgi.buf[3])== 255)
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
                      }
                }
            } 
            if (msgi.id == 0x18EF1C00)//MF engage message
            {
                if ((msgi.buf[0])== 15 && (msgi.buf[1])== 96 && (msgi.buf[2])== 1)
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
                      }
                }
            } 

            
        }
        

          
          if (Brand == 2)
            {
              if (msgi.id == 0x14FF7706)
                {
                  if ((msgi.buf[0])== 130 && (msgi.buf[1])== 1)
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
                  if ((msgi.buf[0])== 178 && (msgi.buf[1])== 4)
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
                      }
                  }
                }
             if (msgi.id == 0x18FE4523)
                {
                  RBUSRearHitch = (msgi.buf[0]);
                  if (RBUSRearHitch < aogConfig.user1) workCAN = 1;
                  else workCAN = 0;
                }
            }


        if (Brand == 4)
          {
              if (msgi.id == 0x18EFAB27)
              {
                  if ((msgi.buf[0])== 15 && (msgi.buf[1])== 96 && (msgi.buf[2])== 1)
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
                      }
                  }
              }    
     
          }
        }
}

//---Receive R_Bus message
//void RBus_Receive()
//{
//  CAN_message_t msgr;
//  if ( R_Bus.read(msgr) )
//      {
//        if (msgr.id == 0x18FE4523)
//          {
//            RBUSRearHitch = (msgr.buf[0]);
//            if (RBUSRearHitch < aogConfig.user1) workCAN = 1;
//            else workCAN = 0;
//          }
//      }
//}
