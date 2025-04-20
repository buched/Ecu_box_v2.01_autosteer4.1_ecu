float decimal_input;
float unit_input;

int32_t vbauds;
void CAN_setup (void) {

vbauds = 250000;

//V_Bus is CAN-3 and is the Steering BUS
  V_Bus.begin();
  V_Bus.setBaudRate(vbauds);
  delay(500);
} //End CAN SETUP

//---Receive K_Bus message
void VBus_Receive()
{
CAN_message_t msgimu;
if (V_Bus.read(msgimu)) 
   {
    if (msgimu.len == 8 && msgimu.buf[0] == 0x55 && msgimu.buf[1] == 0x53) {
      decodeFrameCAN(msgimu.buf);
    }
   }
   
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
}

// Décodage de la trame : [0x55 | 0x53 | RollL | RollH | PitchL | PitchH | YawL | YawH]
void decodeFrameCAN(uint8_t *buf) {
      int16_t roll_raw;
      int16_t pitch_raw;
      float yaw_raw;
      
  // Extraction valeurs 16 bits (Little-endian)
  roll_raw  = (buf[3] << 8) | buf[2];
  pitch_raw = (buf[5] << 8) | buf[4];
  yaw_raw   = (buf[7] << 8) | buf[6];

  // Conversion en degrés
  roll  = roll_raw  / 32768.0 * 180.0;
  pitch = pitch_raw / 32768.0 * 180.0;
  yaw = yaw_raw   / 32768 * 180;

  roll  = roll * 10;
  pitch = pitch * 10;
  yaw = yaw * 10;
  
  if(steerConfig.InvertWAS)
    {
      roll *= -1;
    }
    
//  // Échange Roll et Pitch selon la variable swapRollPitch
  if (steerConfig.IsUseY_Axis) {
    float temp = roll;
    roll = pitch;
    pitch = temp;
  }

  // Normalisation correcte du Yaw à 0-360°
  if (yaw < 0) yaw = 360.0 + yaw;
}
