// outil de configuration pour l'imu BWK215S
// écrit par buched
// manu@couvercelle.eu
// dont claim its your job.
#include <FlexCAN_T4.h>

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_256> IMU_Bus;
CAN_message_t msgi;

uint8_t CanBauds = 1;              //1 = 125k(default settings of canimu module), 2 = 250k, 3 = 500k, 4 = 1000k
uint8_t UseImuCan = 0;
uint8_t Service = 0;
float decimal_input;
float unit_input;
float roll = 0;

void setup(void) {

  IMU_Bus.begin();
  IMU_Bus.setBaudRate(125000);
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
              serv();
            }
          }
                  if ( IMU_Bus.read(msgi) ) 
          {
            if (msgi.id == 0x585)
              {
                decimal_input = (msgi.buf[2]);
                decimal_input = decimal_input /152 ;
                unit_input = (msgi.buf[1]);
                unit_input = unit_input *1000;
                unit_input = unit_input /152;
                roll = unit_input + decimal_input;
                    if (msgi.buf[0] >= 10)
                    {
                      roll *= -1;
                    }
                    roll = roll / 10;
                    Serial.println(roll);
              }
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
        else if ( b == '0') dixrel();
        else if ( b == '1') vgtrel();
        else if ( b == '2') dixabs();
        else if ( b == '3') vgtabs();
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
  Serial.println("0 = configurer en 10htz et angle relatif");
  Serial.println("1 = configurer en 20htz et angle relatif");
  Serial.println("2 = configurer en 10htz et angle absolu");
  Serial.println("3 = configurer en 20htz et angle absolu");
  //Serial.println("2 = see can bus");
  }

void view(){
        if ( IMU_Bus.read(msgi) ) 
          {
            if (msgi.id == 0x585)
              {
                decimal_input = (msgi.buf[2]);
                decimal_input = decimal_input /152 ;
                unit_input = (msgi.buf[1]);
                unit_input = unit_input *1000;
                unit_input = unit_input /152;
                roll = unit_input + decimal_input;
                    if (msgi.buf[0] >= 10)
                    {
                      roll *= -1;
                    }
                    roll = roll / 10;
                    Serial.println(roll);
              }
         }
  }

void dixrel() {
  delay(2000);
  dixhtz();
  delay(2000);
  relativeangle();
  delay(2000);
  save();
  }

void vgtrel() {
  delay(2000);
  vgthtz();
  delay(2000);
  relativeangle();
  delay(2000);
  save();
  }

void dixabs() {
  delay(2000);
  dixhtz();
  delay(2000);
  absoluteangle();
  delay(2000);
  save();
  }

void vgtabs() {
  delay(2000);
  vgthtz();
  delay(2000);
  absoluteangle();
  delay(2000);
  save();
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
  IMU_Bus.write(msgi);
  
  Serial.println("IMU réglé en 10 htz");
}

void vgthtz() {
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
  IMU_Bus.write(msgi);
  
  Serial.println("IMU réglé en relative mode");
}

void absoluteangle() {
  msgi.id = 0x605;
  msgi.flags.extended = false;
  msgi.len = 8;
  msgi.buf[0] = 0x40;
  msgi.buf[1] = 0x05;
  msgi.buf[2] = 0x10;
  msgi.buf[3] = 0x00;
  msgi.buf[4] = 0x00;
  msgi.buf[5] = 0x00;
  msgi.buf[6] = 0x00;
  msgi.buf[7] = 0x00;
  IMU_Bus.write(msgi);
  
  Serial.println("IMU réglé en absolute mode");
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
  IMU_Bus.write(msgi);
  
  Serial.println("saved");
}
