// Conversion to Hexidecimal
const char* asciiHex = "0123456789ABCDEF";

// the new PANDA sentence buffer
char nmea[100];

// GGA
char fixTime[12];
char latitude[15];
char latNS[3];
char longitude[15];
char lonEW[3];
char fixQuality[2];
char numSats[4];
char HDOP[5];
char altitude[12];
char ageDGPS[10];

// VTG
char vtgHeading[12] = { };
char speedKnots[10] = { };

// IMU
char imuHeading[6];
char imuRoll[6];
char imuPitch[6];
char imuYawRate[6];

// If odd characters showed up.
void errorHandler()
{
  //nothing at the moment
}

void GGA_Handler() //Rec'd GGA
{
    // fix time
    parser.getArg(0, fixTime);

    // latitude
    parser.getArg(1, latitude);
    parser.getArg(2, latNS);

    // longitude
    parser.getArg(3, longitude);
    parser.getArg(4, lonEW);

    // fix quality
    parser.getArg(5, fixQuality);

    // satellite #
    parser.getArg(6, numSats);

    // HDOP
    parser.getArg(7, HDOP);

    // altitude
    parser.getArg(8, altitude);

    // time of last DGPS update
    parser.getArg(12, ageDGPS);

    if (blink)
    {
        digitalWrite(GGAReceivedLED, HIGH);
    }
    else
    {
        digitalWrite(GGAReceivedLED, LOW);
    }

    blink = !blink;
    bnoTrigger = true;
    bnoTimer = 0;

    if (useDual)
    {
       dualReadyGGA = true;
    }

    else if (useBNO08xRVC)
    {
        BuildNmea();           //Build & send data GPS data to AgIO (Both Dual & Single)
        dualReadyGGA = false;  //Force dual GGA ready false because we just sent it to AgIO based off the IMU data
        if (!useDual)
        {
            digitalWrite(GPSRED_LED, HIGH);    //Turn red GPS LED ON, we have GGA and must have a IMU     
            digitalWrite(GPSGREEN_LED, LOW);   //Make sure the Green LED is OFF     
        }
    }

    else if (!useDual && !useBNO08xRVC) 
    {
        digitalWrite(GPSRED_LED, blink);   //Flash red GPS LED, we have GGA but no IMU or dual
        digitalWrite(GPSGREEN_LED, LOW);   //Make sure the Green LED is OFF
        itoa(65535, imuHeading, 10);       //65535 is max value to stop AgOpen using IMU in Panda
        BuildNmea();
    }
    
    GGAReadyTime = 0;   //Used for GGA timeout (LED's ETC) 
}

void imuHandler()
{
    if (!useDual)
    {
        if (useBNO08xRVC)
        {
            float angVel;

            // Fill rest of Panda Sentence - Heading
            itoa(bnoData.yawX10, imuHeading, 10);

            if (steerConfig.IsUseY_Axis)
            {
                // the pitch x100
                itoa(bnoData.pitchX10, imuPitch, 10);

                // the roll x100
                itoa(bnoData.rollX10, imuRoll, 10);
            }
            else
            {
                // the pitch x100
                itoa(bnoData.rollX10, imuPitch, 10);

                // the roll x100
                itoa(bnoData.pitchX10, imuRoll, 10);
            }

            //Serial.print(rvc.angCounter);
            //Serial.print(", ");
            //Serial.print(bnoData.angVel);
            //Serial.print(", ");
            // YawRate
            if (rvc.angCounter > 0)
            {
                angVel = ((float)bnoData.angVel) / (float)rvc.angCounter;
                angVel *= 10.0;
                rvc.angCounter = 0;
                bnoData.angVel = (int16_t)angVel;
            }
            else
            {
                bnoData.angVel = 0;
            }

            itoa(bnoData.angVel, imuYawRate, 10);
            bnoData.angVel = 0;
        }
    }

    else
    {
        // the roll
        dtostrf(rollDual, 4, 2, imuRoll);

        // the Dual heading raw
        dtostrf(heading, 4, 2, imuHeading);
    }
}

void BuildNmea(void)
{
    strcpy(nmea, "");

    if (useDual) strcat(nmea, "$PAOGI,");
    else strcat(nmea, "$PANDA,");

    strcat(nmea, fixTime);
    strcat(nmea, ",");

    strcat(nmea, latitude);
    strcat(nmea, ",");

    strcat(nmea, latNS);
    strcat(nmea, ",");

    strcat(nmea, longitude);
    strcat(nmea, ",");

    strcat(nmea, lonEW);
    strcat(nmea, ",");

    // 6
    strcat(nmea, fixQuality);
    strcat(nmea, ",");

    strcat(nmea, numSats);
    strcat(nmea, ",");

    strcat(nmea, HDOP);
    strcat(nmea, ",");

    strcat(nmea, altitude);
    strcat(nmea, ",");

    //10
    strcat(nmea, ageDGPS);
    strcat(nmea, ",");

    //11
    strcat(nmea, speedKnots);
    strcat(nmea, ",");

    //12
    strcat(nmea, imuHeading);
    strcat(nmea, ",");

    //13
    strcat(nmea, imuRoll);
    strcat(nmea, ",");

    //14
    strcat(nmea, imuPitch);
    strcat(nmea, ",");

    //15
    strcat(nmea, imuYawRate);

    strcat(nmea, "*");

    CalculateChecksum();

    strcat(nmea, "\r\n");

    SerialAOG.write(nmea);  //Always send USB GPS data


    int len = strlen(nmea);
    Eth_udpPAOGI.beginPacket(Eth_ipDestination, portDestination);
    Eth_udpPAOGI.write(nmea, len);
    Eth_udpPAOGI.endPacket();
}

void CalculateChecksum(void)
{
  int16_t sum = 0;
  int16_t inx = 0;
  char tmp;

  // The checksum calc starts after '$' and ends before '*'
  for (inx = 1; inx < 200; inx++)
  {
    tmp = nmea[inx];

    // * Indicates end of data and start of checksum
    if (tmp == '*')
    {
      break;
    }

    sum ^= tmp;    // Build checksum
  }

  byte chk = (sum >> 4);
  char hex[2] = { asciiHex[chk], 0 };
  strcat(nmea, hex);

  chk = (sum % 16);
  char hex2[2] = { asciiHex[chk], 0 };
  strcat(nmea, hex2);
}

/*
  $PANDA
  (1) Time of fix

  position
  (2,3) 4807.038,N Latitude 48 deg 07.038' N
  (4,5) 01131.000,E Longitude 11 deg 31.000' E

  (6) 1 Fix quality:
    0 = invalid
    1 = GPS fix(SPS)
    2 = DGPS fix
    3 = PPS fix
    4 = Real Time Kinematic
    5 = Float RTK
    6 = estimated(dead reckoning)(2.3 feature)
    7 = Manual input mode
    8 = Simulation mode
  (7) Number of satellites being tracked
  (8) 0.9 Horizontal dilution of position
  (9) 545.4 Altitude (ALWAYS in Meters, above mean sea level)
  (10) 1.2 time in seconds since last DGPS update
  (11) Speed in knots

  FROM IMU:
  (12) Heading in degrees
  (13) Roll angle in degrees(positive roll = right leaning - right down, left up)

  (14) Pitch angle in degrees(Positive pitch = nose up)
  (15) Yaw Rate in Degrees / second

  CHKSUM
*/

/*
  $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M ,  ,*47
   0     1      2      3    4      5 6  7  8   9    10 11  12 13  14
        Time      Lat       Lon     FixSatsOP Alt
  Where:
     GGA          Global Positioning System Fix Data
     123519       Fix taken at 12:35:19 UTC
     4807.038,N   Latitude 48 deg 07.038' N
     01131.000,E  Longitude 11 deg 31.000' E
     1            Fix quality: 0 = invalid
                               1 = GPS fix (SPS)
                               2 = DGPS fix
                               3 = PPS fix
                               4 = Real Time Kinematic
                               5 = Float RTK
                               6 = estimated (dead reckoning) (2.3 feature)
                               7 = Manual input mode
                               8 = Simulation mode
     08           Number of satellites being tracked
     0.9          Horizontal dilution of position
     545.4,M      Altitude, Meters, above mean sea level
     46.9,M       Height of geoid (mean sea level) above WGS84
                      ellipsoid
     (empty field) time in seconds since last DGPS update
     (empty field) DGPS station ID number
      47          the checksum data, always begins with


  $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
  0      1    2   3      4    5      6   7     8     9     10   11
        Time      Lat        Lon       knots  Ang   Date  MagV

  Where:
     RMC          Recommended Minimum sentence C
     123519       Fix taken at 12:35:19 UTC
     A            Status A=active or V=Void.
     4807.038,N   Latitude 48 deg 07.038' N
     01131.000,E  Longitude 11 deg 31.000' E
     022.4        Speed over the ground in knots
     084.4        Track angle in degrees True
     230394       Date - 23rd of March 1994
     003.1,W      Magnetic Variation
      6A          The checksum data, always begins with

  $GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48

    VTG          Track made good and ground speed
    054.7,T      True track made good (degrees)
    034.4,M      Magnetic track made good
    005.5,N      Ground speed, knots
    010.2,K      Ground speed, Kilometers per hour
     48          Checksum
*/

void VTG_Handler()
{
  // vtg heading
  parser.getArg(0, vtgHeading);

  // vtg Speed knots
  parser.getArg(4, speedKnots);


}
