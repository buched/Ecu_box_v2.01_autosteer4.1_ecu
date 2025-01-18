
void relPosDecode()
{
    int carrSoln;
    bool gnssFixOk, diffSoln, relPosValid;

    heading = (int32_t)ackPacket[30] + ((int32_t)ackPacket[31] << 8)
        + ((int32_t)ackPacket[32] << 16) + ((int32_t)ackPacket[33] << 24);
    heading *= 0.0001;

    heading += headingcorr;
    if (heading >= 3600) heading -= 3600;
    if (heading < 0) heading += 3600;
    heading *= 0.1;

    baseline = (int32_t)ackPacket[26] + ((int32_t)ackPacket[27] << 8)
        + ((int32_t)ackPacket[28] << 16) + ((int32_t)ackPacket[29] << 24);
    double baseHP = (signed char)ackPacket[41];
    baseHP *= 0.01;
    baseline += baseHP;

    relPosD = (int32_t)ackPacket[22] + ((int32_t)ackPacket[23] << 8)
        + ((int32_t)ackPacket[24] << 16) + ((int32_t)ackPacket[25] << 24);
    double relPosHPD = (signed char)ackPacket[40];
    relPosHPD *= 0.01;
    relPosD += relPosHPD;

    uint32_t flags = ackPacket[66];

    gnssFixOk = flags & 1;
    diffSoln = flags & 2;
    relPosValid = flags & 4;
    carrSoln = (flags & 24) >> 3;

    //must be all ok
    if (!gnssFixOk || !diffSoln || !relPosValid) return;

    if (carrSoln == 2)
    {
        if (baseline == 0) baseline += 0.01;
        rollDual = (asin(relPosD / baseline)) * -RAD_TO_DEG;
        digitalWrite(GPSGREEN_LED, HIGH);   //Turn green GPS LED ON
        imuHandler();             //No IMU so use dual data direct
        dualReadyRelPos = true;   //RelPos ready is true so PAOGI will send when the GGA is also ready
    }
    else
    {
        rollDual *= 0.9;
        digitalWrite(GPSGREEN_LED, blink);  //Flash the green GPS LED
        dualReadyRelPos = false;   //RelPos ready is true so PAOGI will send when the GGA is also ready
    }
}
