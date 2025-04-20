float decimal_input;
float unit_input;

int32_t vbauds;
void CAN_setup (void) {

vbauds = 250000;

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
if (V_Bus.read(msgiv)) 
   {
    if (msgiv.id == 0x585)
        {
              if (msgiv.len >= 8)
              {
                float pitch = decodeSignedDecimal(msgiv.buf[0], msgiv.buf[1], msgiv.buf[2]);
                float roll  = decodeSignedDecimal(msgiv.buf[3], msgiv.buf[4], msgiv.buf[5]);
                float yaw   = decodeYaw(msgiv.buf[6], msgiv.buf[7]);

            if(steerConfig.InvertWAS)
              {
                roll *= -1;
              }
              
            //  // Échange Roll et Pitch selon la variable swapRollPitch
            if (steerConfig.IsUseY_Axis)
              {
                float temp = roll;
                roll = pitch;
                pitch = temp;
              }
              
                // Affichage JSON vers port série
                Serial.print("{\"pitch\": ");
                Serial.print(pitch, 2);
                Serial.print(", \"roll\": ");
                Serial.print(roll, 2);
                Serial.print(", \"yaw\": ");
                Serial.print(yaw, 2);
                Serial.println("}");
            }
        }
    else if (msgiv.id == 0x18FF1A03)
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


// Format SXXX.XX (ex: 10 13 06 → -13.06)
float decodeSignedDecimal(uint8_t SX, uint8_t XX, uint8_t YY) {
  uint8_t sign_digit = SX / 10;
  uint8_t hundreds_digit = SX % 10;

  bool isNegative = (sign_digit == 1);

  float value = (hundreds_digit * 100) + XX + (YY / 100.0);
  if (isNegative) value *= -1;

  return value;
}

// Format YYY.YY sur 2 octets (ex: 123 45 → 123.45)
float decodeYaw(uint8_t d7, uint8_t d8) {
  return d7 + (d8 / 100.0);
}

//// Décodage de la trame : [0x55 | 0x53 | RollL | RollH | PitchL | PitchH | YawL | YawH]
//void decodeFrameCAN(uint8_t *buf) {
//      int16_t roll_raw;
//      int16_t pitch_raw;
//      float yaw_raw;
//      
//  // Extraction valeurs 16 bits (Little-endian)
//  roll_raw  = (buf[3] << 8) | buf[2];
//  pitch_raw = (buf[5] << 8) | buf[4];
//  yaw_raw   = (buf[7] << 8) | buf[6];
//
//  // Conversion en degrés
//  roll  = roll_raw  / 32768.0 * 180.0;
//  pitch = pitch_raw / 32768.0 * 180.0;
//  yaw = yaw_raw   / 32768 * 180;
//
//  roll  = roll * 10;
//  pitch = pitch * 10;
//  yaw = yaw * 10;
//  
//  if(steerConfig.InvertWAS)
//    {
//      roll *= -1;
//    }
//    
////  // Échange Roll et Pitch selon la variable swapRollPitch
//  if (steerConfig.IsUseY_Axis) {
//    float temp = roll;
//    roll = pitch;
//    pitch = temp;
//  }
//
//  // Normalisation correcte du Yaw à 0-360°
//  if (yaw < 0) yaw = 360.0 + yaw;
//}
