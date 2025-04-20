// outil de configuration pour l'imu BWK215S
// écrit par buched
// manu@couvercelle.eu
// dont claim its your job.

#include <EEPROM.h>
// if not in eeprom, overwrite
#define EEP_Ident 2444
//EEPROM
int16_t EEread = 0;
//Variables for settings
struct Storage {
  uint8_t Kb = 1;              // 1 125k 2 250 3 500
};  Storage Settings;      // 11 bytes

#include <FlexCAN_T4.h>
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_256> IMU_Bus;
CAN_message_t msgi;

uint16_t CanBauds = 125000;              //1 = 125k(default settings of canimu module), 2 = 250k, 3 = 500k, 4 = 1000k
uint8_t bauds = 0;
uint8_t Service = 0;

void setup(void) {
    Serial.begin(460800);
   EEPROM.get(0, EEread);              // read identifier

  if (EEread != EEP_Ident)            // check on first start and write EEPROM
  {
    EEPROM.put(0, EEP_Ident);
    EEPROM.put(10, Settings);  
  }
  else
  {
    EEPROM.get(10, Settings);
  }
  IMU_Bus.begin();
  Serial.println("config actuelle : ");
  bauds = Settings.Kb;
  if (bauds == 1)
  {
    IMU_Bus.setBaudRate(125000);
    Serial.println("125k bauds");
    Serial.println("");
  }
  else  if (bauds == 2) 
  {
    IMU_Bus.setBaudRate(250000);
    Serial.println("250k bauds");
    Serial.println("");
  }
  else  if (bauds == 3) 
  {
    IMU_Bus.setBaudRate(500000);
    Serial.println("500k bauds");
    Serial.println("");
  }
  else  if (bauds == 4) 
  {
    IMU_Bus.setBaudRate(1000000);
  }
  Serial.println("Outil de configuration pour Imu BWK215S");
  Serial.println("S pour démarrer");
}

void loop() {
  
        if (Serial.available())
          {        // Read Data From Serial Monitor 
            byte b = Serial.read();
        
            while (Serial.available()){
              Serial.read();              //Clear the serial buffer
            }
        
            if ( b == 'S') {
              Service = 1;
        
              //save in EEPROM and restart
              EEPROM.put(10, Settings);
              serv();
            }
          }
       if ( IMU_Bus.read(msgi) ) 
          {
            if (msgi.id == 0x585)
                {
                      if (msgi.len >= 8)
                      {
                        float pitch = decodeSignedDecimal(msgi.buf[0], msgi.buf[1], msgi.buf[2]);
                        float roll  = decodeSignedDecimal(msgi.buf[3], msgi.buf[4], msgi.buf[5]);
                        float yaw   = decodeYaw(msgi.buf[6], msgi.buf[7]);
        
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

void serv() {
  Serial.println("\r\nAgOpenGPS CANBUS Service Tool Mode:");
  Help();
   while (Service == 1) 
  { 
      if (Serial.available())   // Read Data From Serail Monitor 
      {    
        byte b = Serial.read();
        if ( b == '?') Help();          
        else if ( b == 'X') exi(); //Exit Service Mode
        else if ( b == '0') setoutput();
        else if ( b == '1') vgt();
        else if ( b == '2') cvcqk();
        else if ( b == '3') dcqtk();
        else if ( b == '4') cctk();
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
 void exi() {
    Serial.println("fin du mode service");
  Service  = 0;
  //save in EEPROM and restart
  EEPROM.put(10, Settings);
  delay(2000);
  //reboo();
  }
void Help() {
  Serial.println("X = service mode exit");
  Serial.println("0 = configurer en 3 angles output");
  Serial.println("1 = configurer en 20htz");
  Serial.println("2 = configurer en 125k bauds");
  Serial.println("3 = configurer en 250k bauds");
  Serial.println("4 = configurer en 500k bauds");
//  Serial.println("");
//  Serial.println("config actuelle : ");
//  Serial.println(CanBauds);
  }

void setoutput() {
    msgi.id = 0x605;
  msgi.flags.extended = false;
  msgi.len = 8;
  msgi.buf[0] = 0x40;
  msgi.buf[1] = 0x56;
  msgi.buf[2] = 0x10;
  msgi.buf[3] = 0x00;
  msgi.buf[4] = 0x00;
  msgi.buf[5] = 0x00;
  msgi.buf[6] = 0x00;
  msgi.buf[7] = 0x00;
  IMU_Bus.write(msgi);
  Serial.println("IMU réglé sur 3 angles output");
  delay(2000);
  save();
  }

void vgt() {
  msgi.id = 0x605;
  msgi.flags.extended = false;
  msgi.len = 8;
  msgi.buf[0] = 0x40;
  msgi.buf[1] = 0x0c;
  msgi.buf[2] = 0x10;
  msgi.buf[3] = 0x00;
  msgi.buf[4] = 0x03;
  msgi.buf[5] = 0x00;
  msgi.buf[6] = 0x00;
  msgi.buf[7] = 0x00;
  IMU_Bus.write(msgi);
  Serial.println("IMU réglé en 20 htz");
  delay(2000);
  save();
}

void cvcqk() {
    msgi.id = 0x605;
  msgi.flags.extended = false;
  msgi.len = 8;
  msgi.buf[0] = 0x40;
  msgi.buf[1] = 0x20;
  msgi.buf[2] = 0x10;
  msgi.buf[3] = 0x00;
  msgi.buf[4] = 0x03;
  msgi.buf[5] = 0x00;
  msgi.buf[6] = 0x00;
  msgi.buf[7] = 0x00;
  IMU_Bus.write(msgi);
  Serial.println("réglé sur 125k bauds");
  delay(2000);
  save();
  delay(500);
  Serial.println("Patientez, reboot en cours");
  Settings.Kb  = 1;
  //save in EEPROM and restart
  EEPROM.put(10, Settings);
  delay(2000);
  reboo();
  }
  
void dcqtk() {
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
  IMU_Bus.write(msgi);
  Serial.println("réglé sur 250k bauds");
  delay(2000);
  save();
  delay(500);
  Serial.println("Patientez, reboot en cours");
  Settings.Kb  = 2;
  //save in EEPROM and restart
  EEPROM.put(10, Settings);
  delay(2000);
  reboo();
  }

void cctk() {
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
  IMU_Bus.write(msgi);
  Serial.println("réglé sur 500k bauds");
  delay(2000);
  save();
  delay(500);
  Serial.println("Patientez, reboot en cours");
  Settings.Kb  = 3;
  //save in EEPROM and restart
  EEPROM.put(10, Settings);
  delay(2000);
  reboo();
  }
  
void save() {
  msgi.id = 0x605;
  msgi.flags.extended = false;
  msgi.len = 8;
  msgi.buf[0] = 0x40;
  msgi.buf[1] = 0x0A;
  msgi.buf[2] = 0x10;
  msgi.buf[3] = 0x00;
  msgi.buf[4] = 0x00;
  msgi.buf[5] = 0x00;
  msgi.buf[6] = 0x00;
  msgi.buf[7] = 0x00;
  IMU_Bus.write(msgi);
  
  Serial.println("saved");
}

void reboo() {
  SCB_AIRCR = 0x05FA0004; //Teensy Reset
             }
