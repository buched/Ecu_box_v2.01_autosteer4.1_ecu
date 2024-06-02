void Service_Tool (void) 
{
  Serial.println("\r\nAgOpenGPS CANBUS Service Tool Mode:");
  Help();
  
  while (Service == 1) 
  {
      if (Serial.available())   // Read Data From Serail Monitor 
      {
        byte b = Serial.read();
        if ( b == 'c') conf();
        else if ( b == 'X') Service = 0; //Exit Service Mode
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

//**************************************************************************************
void conf(){
  if (SerialWit->available())
  Serial.write(SerialWit->read());

  if (Serial.available())
  SerialWit->write(Serial.read());
}

//**************************************************************************************
void Help(){
  Serial.println("? = Help");
  Serial.println("X = Exit Service Mode");
  Serial.println("c = config wit");

}
