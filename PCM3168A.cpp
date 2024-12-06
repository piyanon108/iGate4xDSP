#include "PCM3168A.h"

PCM3168A::PCM3168A(QString i2cdevice, int address)
{
    i2cDev = i2cdevice;
    i2cAddress = address;
    dataReg = new CodecDataReg;
    i2cCodecCtrl_CODEC = new I2CReadWrite(i2cDev.toStdString().c_str(),i2cAddress);
    active = writeBytes(0x40,0xc1);
    if (active) initCodec();
}

void PCM3168A::initCodec()
{
    writeReg40h(1,1,0,0,0,0,0,1); //DAC Single rate
    writeReg50h(0,0,0,0,0,0,0,1); //ADC Single rate
    writeReg41h(0,0,0,0,0,1,1,0); //FMTDA 0110 24-bit I2S mode TDM format
    writeReg51h(0,0,0,0,0,1,1,0); //FMTDA 0110 24-bit I2S mode TDM format

}
void PCM3168A::setInputGain(uint8_t channel, uint8_t AINX_VOL)
{
    if (AINX_VOL <= 8) AINX_VOL = 0;
    AINX_VOL = 255 - AINX_VOL;
    uint8_t reg = 0x58+channel;
    writeBytes(reg,AINX_VOL);
}
bool PCM3168A::writeBytes(uint8_t reg, uint8_t data)
{
    i2cCodecCtrl_CODEC->buffer[0] = reg;
    i2cCodecCtrl_CODEC->buffer[1] = data;
    return i2cCodecCtrl_CODEC->writeBytes();
}

void PCM3168A::writeReg40h(uint8_t MRST, uint8_t SRST, uint8_t B5, uint8_t B4, uint8_t B3, uint8_t B2, uint8_t SRDA1, uint8_t SRDA0)
{
    dataReg->Reg40h = char((MRST << 7) | (SRST << 6) | (B5 << 5) | (B4 << 4) | (B3 << 3) | (B2 << 2) | (SRDA1 << 1) | (SRDA0 << 0));
    writeBytes(0x40,dataReg->Reg40h);
}
//Table 15. Register: DAC Control 1
//FMTDA 0100 24-bit I2S mode DSP form

void PCM3168A::writeReg41h(uint8_t PSMDA, uint8_t MSDA2, uint8_t MSDA1, uint8_t MSDA0, uint8_t FMTDA3, uint8_t FMTDA2, uint8_t FMTDA1, uint8_t FMTDA0){
    dataReg->Reg41h = char((PSMDA << 7) | (MSDA2 << 6) | (MSDA1 << 5) | (MSDA0 << 4) | (FMTDA3 << 3) | (FMTDA2 << 2) | (FMTDA1 << 1) | (FMTDA0 << 0));
    writeBytes(0x41,dataReg->Reg41h);
}

void PCM3168A::writeReg42h(uint8_t OPEDA3, uint8_t OPEDA2, uint8_t OPEDA1, uint8_t OPEDA0, uint8_t FLT3, uint8_t FLT2, uint8_t FLT1, uint8_t FLT0){
    dataReg->Reg42h = char((OPEDA3 << 7) | (OPEDA2 << 6) | (OPEDA1 << 5) | (OPEDA0 << 4) | (FLT3 << 3) | (FLT2 << 2) | (FLT1 << 1) | (FLT0 << 0));
    writeBytes(0x42,dataReg->Reg42h);
}

void PCM3168A::writeReg43h(uint8_t REVDA8, uint8_t REVDA7, uint8_t REVDA6, uint8_t REVDA5, uint8_t REVDA4, uint8_t REVDA3, uint8_t REVDA2, uint8_t REVDA1){
    dataReg->Reg43h = char((REVDA8 << 7) | (REVDA7 << 6) | (REVDA6 << 5) | (REVDA5 << 4) | (REVDA4 << 3) | (REVDA3 << 2) | (REVDA2 << 1) | (REVDA1 << 0));
    writeBytes(0x43,dataReg->Reg43h);
}

void PCM3168A::writeReg44h(uint8_t MUTDA8, uint8_t MUTDA7, uint8_t MUTDA6, uint8_t MUTDA5, uint8_t MUTDA4, uint8_t MUTDA3, uint8_t MUTDA2, uint8_t MUTDA1){
    dataReg->Reg44h = char((MUTDA8 << 7) | (MUTDA7 << 6) | (MUTDA6 << 5) | (MUTDA5 << 4) | (MUTDA4 << 3) | (MUTDA3 << 2) | (MUTDA2 << 1) | (MUTDA1 << 0));
    writeBytes(0x44,dataReg->Reg44h);
}

void PCM3168A::writeReg45h(uint8_t ZERO8, uint8_t ZERO7, uint8_t ZERO6, uint8_t ZERO5, uint8_t ZERO4, uint8_t ZERO3, uint8_t ZERO2, uint8_t ZERO1){
    dataReg->Reg45h = char((ZERO8 << 7) | (ZERO7 << 6) | (ZERO6 << 5) | (ZERO5 << 4) | (ZERO4 << 3) | (ZERO3 << 2) | (ZERO2 << 1) | (ZERO1 << 0));
    writeBytes(0x45,dataReg->Reg45h);
}

void PCM3168A::writeReg46h(uint8_t ATMDDA, uint8_t ATSPDA, uint8_t DEMP1, uint8_t DEMP0, uint8_t AZRO2, uint8_t AZRO1, uint8_t AZRO0, uint8_t ZREV){
    dataReg->Reg46h = char((ATMDDA << 7) | (ATSPDA << 6) | (DEMP1 << 5) | (DEMP0 << 4) | (AZRO2 << 3) | (AZRO1 << 2) | (AZRO0 << 1) | (ZREV << 0));
    writeBytes(0x46,dataReg->Reg46h);
}

void PCM3168A::writeReg47h(uint8_t ATDA07, uint8_t ATDA06, uint8_t ATDA05, uint8_t ATDA04, uint8_t ATDA03, uint8_t ATDA02, uint8_t ATDA01, uint8_t ATDA00){
    dataReg->Reg47h = char((ATDA07 << 7) | (ATDA06 << 6) | (ATDA05 << 5) | (ATDA04 << 4) | (ATDA03 << 3) | (ATDA02 << 2) | (ATDA01 << 1) | (ATDA00 << 0));
    writeBytes(0x47,dataReg->Reg47h);
}

void PCM3168A::writeReg48h(uint8_t ATDA17, uint8_t ATDA16, uint8_t ATDA15, uint8_t ATDA14, uint8_t ATDA13, uint8_t ATDA12, uint8_t ATDA11, uint8_t ATDA10){
    dataReg->Reg48h = char((ATDA17 << 7) | (ATDA16 << 6) | (ATDA15 << 5) | (ATDA14 << 4) | (ATDA13 << 3) | (ATDA12 << 2) | (ATDA11 << 1) | (ATDA10 << 0));
    writeBytes(0x48,dataReg->Reg48h);
}

void PCM3168A::writeReg49h(uint8_t ATDA27, uint8_t ATDA26, uint8_t ATDA25, uint8_t ATDA24, uint8_t ATDA23, uint8_t ATDA22, uint8_t ATDA21, uint8_t ATDA20){
    dataReg->Reg49h = char((ATDA27 << 7) | (ATDA26 << 6) | (ATDA25 << 5) | (ATDA24 << 4) | (ATDA23 << 3) | (ATDA22 << 2) | (ATDA21 << 1) | (ATDA20 << 0));
    writeBytes(0x49,dataReg->Reg49h);
}

void PCM3168A::writeReg4Ah(uint8_t ATDA37, uint8_t ATDA36, uint8_t ATDA35, uint8_t ATDA34, uint8_t ATDA33, uint8_t ATDA32, uint8_t ATDA31, uint8_t ATDA30){
    dataReg->Reg4ah = char((ATDA37 << 7) | (ATDA36 << 6) | (ATDA35 << 5) | (ATDA34 << 4) | (ATDA33 << 3) | (ATDA32 << 2) | (ATDA31 << 1) | (ATDA30 << 0));
    writeBytes(0x4a,dataReg->Reg4ah);
}

void PCM3168A::writeReg4Bh(uint8_t ATDA47, uint8_t ATDA46, uint8_t ATDA45, uint8_t ATDA44, uint8_t ATDA43, uint8_t ATDA42, uint8_t ATDA41, uint8_t ATDA40){
    dataReg->Reg4bh = char((ATDA47 << 7) | (ATDA46 << 6) | (ATDA45 << 5) | (ATDA44 << 4) | (ATDA43 << 3) | (ATDA42 << 2) | (ATDA41 << 1) | (ATDA40 << 0));
    writeBytes(0x4b,dataReg->Reg4bh);
}

void PCM3168A::writeReg4Ch(uint8_t ATDA57, uint8_t ATDA56, uint8_t ATDA55, uint8_t ATDA54, uint8_t ATDA53, uint8_t ATDA52, uint8_t ATDA51, uint8_t ATDA50){
    dataReg->Reg4ch = char((ATDA57 << 7) | (ATDA56 << 6) | (ATDA55 << 5) | (ATDA54 << 4) | (ATDA53 << 3) | (ATDA52 << 2) | (ATDA51 << 1) | (ATDA50 << 0));
    writeBytes(0x4c,dataReg->Reg4ch);
}

void PCM3168A::writeReg4Dh(uint8_t ATDA67, uint8_t ATDA66, uint8_t ATDA65, uint8_t ATDA64, uint8_t ATDA63, uint8_t ATDA62, uint8_t ATDA61, uint8_t ATDA60){
    dataReg->Reg4dh = char((ATDA67 << 7) | (ATDA66 << 6) | (ATDA65 << 5) | (ATDA64 << 4) | (ATDA63 << 3) | (ATDA62 << 2) | (ATDA61 << 1) | (ATDA60 << 0));
    writeBytes(0x4d,dataReg->Reg4dh);
}

void PCM3168A::writeReg4Eh(uint8_t ATDA77, uint8_t ATDA76, uint8_t ATDA75, uint8_t ATDA74, uint8_t ATDA73, uint8_t ATDA72, uint8_t ATDA71, uint8_t ATDA70){
    dataReg->Reg4eh = char((ATDA77 << 7) | (ATDA76 << 6) | (ATDA75 << 5) | (ATDA74 << 4) | (ATDA73 << 3) | (ATDA72 << 2) | (ATDA71 << 1) | (ATDA70 << 0));
    writeBytes(0x4e,dataReg->Reg4eh);
}

void PCM3168A::writeReg4Fh(uint8_t ATDA87, uint8_t ATDA86, uint8_t ATDA85, uint8_t ATDA84, uint8_t ATDA83, uint8_t ATDA82, uint8_t ATDA81, uint8_t ATDA80){
    dataReg->Reg4fh = char((ATDA87 << 7) | (ATDA86 << 6) | (ATDA85 << 5) | (ATDA84 << 4) | (ATDA83 << 3) | (ATDA82 << 2) | (ATDA81 << 1) | (ATDA80 << 0));
    writeBytes(0x4f,dataReg->Reg4fh);
}

void PCM3168A::writeReg50h(uint8_t B7, uint8_t B6, uint8_t B5, uint8_t B4, uint8_t B3, uint8_t B2, uint8_t SRAD1, uint8_t SRAD0){
    dataReg->Reg50h = char((B7 << 7) | (B6 << 6) | (B5 << 5) | (B4 << 4) | (B3 << 3) | (B2 << 2) | (SRAD1 << 1) | (SRAD0 << 0));
    writeBytes(0x50,dataReg->Reg50h);
}

void PCM3168A::writeReg51h(uint8_t B7, uint8_t MSAD2, uint8_t MSAD1, uint8_t MSAD0, uint8_t B3, uint8_t FMTAD2, uint8_t FMTAD1, uint8_t FMTAD0){
    dataReg->Reg51h = char((B7 << 7) | (MSAD2 << 6) | (MSAD1 << 5) | (MSAD0 << 4) | (B3 << 3) | (FMTAD2 << 2) | (FMTAD1 << 1) | (FMTAD0 << 0));
    writeBytes(0x51,dataReg->Reg51h);
}

void PCM3168A::writeReg52h(uint8_t B7, uint8_t PSVAD2, uint8_t PSVAD1, uint8_t PSVAD0, uint8_t B3, uint8_t BYP2, uint8_t BYP1, uint8_t BYP0){
    dataReg->Reg52h = char((B7 << 7) | (PSVAD2 << 6) | (PSVAD1 << 5) | (PSVAD0 << 4) | (B3 << 3) | (BYP2 << 2) | (BYP1 << 1) | (BYP0 << 0));
    writeBytes(0x52,dataReg->Reg52h);
}

void PCM3168A::writeReg53h(uint8_t B7, uint8_t B6, uint8_t SEAD6, uint8_t SEAD5, uint8_t SEAD4, uint8_t SEAD3, uint8_t SEAD2, uint8_t SEAD1){
    dataReg->Reg53h = char((B7 << 7) | (B6 << 6) | (SEAD6 << 5) | (SEAD5 << 4) | (SEAD4 << 3) | (SEAD3 << 2) | (SEAD2 << 1) | (SEAD1 << 0));
    writeBytes(0x53,dataReg->Reg53h);
}

void PCM3168A::writeReg54h(uint8_t B7, uint8_t B6, uint8_t REVAD6, uint8_t REVAD5, uint8_t REVAD4, uint8_t REVAD3, uint8_t REVAD2, uint8_t REVAD1){
    dataReg->Reg54h = char((B7 << 7) | (B6 << 6) | (REVAD6 << 5) | (REVAD5 << 4) | (REVAD4 << 3) | (REVAD3 << 2) | (REVAD2 << 1) | (REVAD1 << 0));
    writeBytes(0x54,dataReg->Reg54h);
}

void PCM3168A::writeReg55h(uint8_t B7, uint8_t B6, uint8_t MUTAD6, uint8_t MUTAD5, uint8_t MUTAD4, uint8_t MUTAD3, uint8_t MUTAD2, uint8_t MUTAD1){
    dataReg->Reg55h = char((B7 << 7) | (B6 << 6) | (MUTAD6 << 5) | (MUTAD5 << 4) | (MUTAD4 << 3) | (MUTAD3 << 2) | (MUTAD2 << 1) | (MUTAD1 << 0));
    writeBytes(0x55,dataReg->Reg55h);
}

void PCM3168A::writeReg56h(uint8_t B7, uint8_t B6, uint8_t OVF6, uint8_t OVF5, uint8_t OVF4, uint8_t OVF3, uint8_t OVF2, uint8_t OVF1){
    dataReg->Reg56h = char((B7 << 7) | (B6 << 6) | (OVF6 << 5) | (OVF5 << 4) | (OVF4 << 3) | (OVF3 << 2) | (OVF2 << 1) | (OVF1 << 0));
    writeBytes(0x56,dataReg->Reg56h);
}

void PCM3168A::writeReg57h(uint8_t ATMDAD, uint8_t ATSPAD, uint8_t B5, uint8_t B4, uint8_t B3, uint8_t B2, uint8_t B1, uint8_t OVFP){
    dataReg->Reg57h = char((ATMDAD << 7) | (ATSPAD << 6) | (B5 << 5) | (B4 << 4) | (B3 << 3) | (B2 << 2) | (B1 << 1) | (OVFP << 0));
    writeBytes(0x57,dataReg->Reg57h);
}

void PCM3168A::writeReg58h(uint8_t ATAD07, uint8_t ATAD06, uint8_t ATAD05, uint8_t ATAD04, uint8_t ATAD03, uint8_t ATAD02, uint8_t ATAD01, uint8_t ATAD00){
    dataReg->Reg58h = char((ATAD07 << 7) | (ATAD06 << 6) | (ATAD05 << 5) | (ATAD04 << 4) | (ATAD03 << 3) | (ATAD02 << 2) | (ATAD01 << 1) | (ATAD00 << 0));
    writeBytes(0x58,dataReg->Reg58h);
}

void PCM3168A::writeReg59h(uint8_t ATAD17, uint8_t ATAD16, uint8_t ATAD15, uint8_t ATAD14, uint8_t ATAD13, uint8_t ATAD12, uint8_t ATAD11, uint8_t ATAD10){
    dataReg->Reg59h = char((ATAD17 << 7) | ( ATAD16<< 6) | (ATAD15 << 5) | (ATAD14 << 4) | (ATAD13 << 3) | (ATAD12 << 2) | (ATAD11 << 1) | (ATAD10 << 0));
    writeBytes(0x59,dataReg->Reg59h);
}

void PCM3168A::writeReg5Ah(uint8_t ATAD27, uint8_t ATAD26, uint8_t ATAD25, uint8_t ATAD24, uint8_t ATAD23, uint8_t ATAD22, uint8_t ATAD21, uint8_t ATAD20){
    dataReg->Reg5ah = char((ATAD27 << 7) | (ATAD26 << 6) | (ATAD25 << 5) | (ATAD24 << 4) | (ATAD23 << 3) | ( ATAD22<< 2) | (ATAD21 << 1) | (ATAD20 << 0));
    writeBytes(0x5a,dataReg->Reg5ah);
}

void PCM3168A::writeReg5Bh(uint8_t ATAD37, uint8_t ATAD36, uint8_t ATAD35, uint8_t ATAD34, uint8_t ATAD33, uint8_t ATAD32, uint8_t ATAD31, uint8_t ATAD30){
    dataReg->Reg5bh = char((ATAD37 << 7) | (ATAD36 << 6) | (ATAD35 << 5) | (ATAD34 << 4) | (ATAD33 << 3) | (ATAD32 << 2) | (ATAD31 << 1) | (ATAD30 << 0));
    writeBytes(0x5b,dataReg->Reg5bh);
}

void PCM3168A::writeReg5Ch(uint8_t ATAD47, uint8_t ATAD46, uint8_t ATAD45, uint8_t ATAD44, uint8_t ATAD43, uint8_t ATAD42, uint8_t ATAD41, uint8_t ATAD40){
    dataReg->Reg5ch = char((ATAD47 << 7) | (ATAD46 << 6) | (ATAD45 << 5) | (ATAD44 << 4) | (ATAD43 << 3) | (ATAD42 << 2) | (ATAD41 << 1) | (ATAD40 << 0));
    writeBytes(0x5c,dataReg->Reg5ch);
}

void PCM3168A::writeReg5Dh(uint8_t ATAD57, uint8_t ATAD56, uint8_t ATAD55, uint8_t ATAD54, uint8_t ATAD53, uint8_t ATAD52, uint8_t ATAD51, uint8_t ATAD50){
    dataReg->Reg5dh = char((ATAD57 << 7) | (ATAD56 << 6) | (ATAD55 << 5) | (ATAD54 << 4) | (ATAD53 << 3) | (ATAD52 << 2) | (ATAD51 << 1) | (ATAD50 << 0));
    writeBytes(0x5d,dataReg->Reg5dh);
}

void PCM3168A::writeReg5Eh(uint8_t ATAD67, uint8_t ATAD66, uint8_t ATAD65, uint8_t ATAD64, uint8_t ATAD63, uint8_t ATAD62, uint8_t ATAD61, uint8_t ATAD60){
    dataReg->Reg5eh = char((ATAD67 << 7) | (ATAD66 << 6) | (ATAD65 << 5) | (ATAD64 << 4) | (ATAD63 << 3) | (ATAD62 << 2) | (ATAD61 << 1) | (ATAD60 << 0));
    writeBytes(0x5e,dataReg->Reg5eh);
}
