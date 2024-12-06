#include "CS42448.h"

CS42448::CS42448(QString i2cdevice, int address)
{
    i2cDev = i2cdevice;
    i2cAddress = address;
    dataReg = new CodecDataReg;
    i2cCodecCtrl_CODEC = new I2CReadWrite(i2cDev.toStdString().c_str(),i2cAddress);
    active = MuteAudioOutput(true,5);
//    initCodec();
}

void CS42448::initCodec()
{
    Reg02h_PowerDownCtrl(1,0,0,1,1,1,0,0);
    Reg03h_FunctionalMode(0x03,0x03,0x00);
    Reg04h_InterfaceFormats(0,0,0x06,0x06);
    Reg05h_ADCCtrlDACDeEmphasis(0,0,0,0,0,0,0,0);
    Reg06h_TransitionControl(0,0,0,1,0,0,0,0);
    Reg07hDACChannelMute(1,1,1,1,1,1,1,1);
    Reg08_0FhAOUTXVolumeCtrl(1,0);
    Reg08_0FhAOUTXVolumeCtrl(2,0);
    Reg08_0FhAOUTXVolumeCtrl(3,0);
    Reg08_0FhAOUTXVolumeCtrl(4,0);
    Reg08_0FhAOUTXVolumeCtrl(5,0);
    Reg08_0FhAOUTXVolumeCtrl(6,0);
    Reg08_0FhAOUTXVolumeCtrl(7,0);
    Reg08_0FhAOUTXVolumeCtrl(8,0);
    Reg10hDACChannelInvert(0,0,0,0,0,0,0,0);
    Reg11_16hAINVolumeCtrl(1,0);
    Reg11_16hAINVolumeCtrl(2,0);
    Reg11_16hAINVolumeCtrl(3,0);
    Reg11_16hAINVolumeCtrl(4,0);
    Reg11_16hAINVolumeCtrl(5,0);
    Reg11_16hAINVolumeCtrl(6,0);
    Reg17hADCChannelInvert(0,0,0,0,0,0);
}

bool CS42448::Reg02h_PowerDownCtrl(uint8_t PDN_ADC3, uint8_t PDN_ADC2, uint8_t PDN_ADC1, uint8_t PDN_DAC4, uint8_t PDN_DAC3, uint8_t PDN_DAC2, uint8_t PDN_DAC1, uint8_t PDN)
{
    dataReg->powerDown = char((PDN_ADC3 << 7) | (PDN_ADC2 << 6) | (PDN_ADC1 << 5) | (PDN_DAC4 << 4) | (PDN_DAC3 << 3) | (PDN_DAC2 << 2) | (PDN_DAC1 << 1) | (PDN << 0));
    i2cCodecCtrl_CODEC->buffer[0] = 0x02;
    i2cCodecCtrl_CODEC->buffer[1] = dataReg->powerDown;
    return i2cCodecCtrl_CODEC->writeBytes();
}

bool CS42448::Reg03h_FunctionalMode(unsigned char DAC_FM, unsigned char ADC_FM, unsigned char MFREQ)
{
    dataReg->functionMode = char((DAC_FM << 6) | (ADC_FM << 4) | (MFREQ << 0));
    i2cCodecCtrl_CODEC->buffer[0] = 0x03;
    i2cCodecCtrl_CODEC->buffer[1] = dataReg->functionMode;
    return i2cCodecCtrl_CODEC->writeBytes();
}

bool CS42448::Reg04h_InterfaceFormats(unsigned char FREEZE, unsigned char AUX_DIF, unsigned char DAC_DIF, unsigned char ADC_DIF)
{
    dataReg->interfaceFormat = char((FREEZE << 7) | (AUX_DIF << 6) | (DAC_DIF << 5) | (ADC_DIF << 2));
    i2cCodecCtrl_CODEC->buffer[0] = 0x04;
    i2cCodecCtrl_CODEC->buffer[1] = dataReg->interfaceFormat;
    return i2cCodecCtrl_CODEC->writeBytes();
}

bool CS42448::Reg05h_ADCCtrlDACDeEmphasis(unsigned char ADC1_2_HPF_FREEZE, unsigned char ADC3_HPF_FREEZE, unsigned char DAC_DEM, unsigned char ADC1_SINGLE, unsigned char ADC2_SINGLE, unsigned char ADC3_SINGLE, unsigned char AIN5_MUX, unsigned char AIN6_MUX)
{
    dataReg->ADCControlDACDeEmphasis = char((ADC1_2_HPF_FREEZE << 7) | (ADC3_HPF_FREEZE << 6) | (DAC_DEM << 5) | (ADC1_SINGLE << 4) | (ADC2_SINGLE << 3) | (ADC3_SINGLE << 2) | (AIN5_MUX << 1) | (AIN6_MUX << 0));
    i2cCodecCtrl_CODEC->buffer[0] = 0x05;
    i2cCodecCtrl_CODEC->buffer[1] = dataReg->ADCControlDACDeEmphasis;
    return i2cCodecCtrl_CODEC->writeBytes();
}

bool CS42448::Reg06h_TransitionControl(unsigned char DAC_SNGVOL, unsigned char DAC_SZC1, unsigned char DAC_SZC0, unsigned char AMUTE, unsigned char MUTE_ADC_SP, unsigned char ADC_SNGVOL, unsigned char ADC_SZC1, unsigned char ADC_SZC0)
{
    dataReg->TransitionControl = char((DAC_SNGVOL << 7) | (DAC_SZC1 << 6) | (DAC_SZC0 << 5) | (AMUTE << 4) | (MUTE_ADC_SP << 3) | (ADC_SNGVOL << 2) | (ADC_SZC1 << 1) | (ADC_SZC0 << 0));
    i2cCodecCtrl_CODEC->buffer[0] = 0x06;
    i2cCodecCtrl_CODEC->buffer[1] = dataReg->TransitionControl;
    return i2cCodecCtrl_CODEC->writeBytes();
}

bool CS42448::Reg07hDACChannelMute(unsigned char AOUT8_MUTE, unsigned char AOUT7_MUTE, unsigned char AOUT6_MUTE, unsigned char AOUT5_MUTE, unsigned char AOUT4_MUTE, unsigned char AOUT3_MUTE, unsigned char AOUT2_MUTE, unsigned char AOUT1_MUTE)
{
    dataReg->DACChannelMute = char((AOUT8_MUTE << 7) | (AOUT7_MUTE << 6) | (AOUT6_MUTE << 5) | (AOUT5_MUTE << 4) | (AOUT4_MUTE << 3) | (AOUT3_MUTE << 2) | (AOUT2_MUTE << 1) | (AOUT1_MUTE << 0));
    i2cCodecCtrl_CODEC->buffer[0] = 0x07;
    i2cCodecCtrl_CODEC->buffer[1] = dataReg->DACChannelMute;
    return i2cCodecCtrl_CODEC->writeBytes();
}

//The AOUTx Volume Control registers allow independent setting of the signal levels in 0.5 dB increments
//from 0 dB to -127.5 dB. Volume settings are decoded as shown in Table 14. The volume changes are
//implemented as dictated by the Soft and Zero Cross bits (DAC_SZC[1:0]). All volume settings less than
//-127.5 dB are equivalent to enabling the AOUTx_MUTE bit for the given channel

bool CS42448::Reg08_0FhAOUTXVolumeCtrl(unsigned char channel, unsigned char AOUTX_VOL)
{
    dataReg->aOutVolumeCtrl[channel-1] = AOUTX_VOL;
    i2cCodecCtrl_CODEC->buffer[0] = 0x08+(channel - 1);
    i2cCodecCtrl_CODEC->buffer[1] = AOUTX_VOL;
    return i2cCodecCtrl_CODEC->writeBytes();
}

bool CS42448::Reg10hDACChannelInvert(unsigned char INV_AOUT8, unsigned char INV_AOUT7, unsigned char INV_AOUT6, unsigned char INV_AOUT5, unsigned char INV_AOUT4, unsigned char INV_AOUT3, unsigned char INV_AOUT2, unsigned char INV_AOUT1)
{
    dataReg->DACChannelInvert = char((INV_AOUT8 << 7) | (INV_AOUT7 << 6) | (INV_AOUT6 << 5) | (INV_AOUT5 << 4) | (INV_AOUT4 << 3) | (INV_AOUT3 << 2) | (INV_AOUT2 << 1) | (INV_AOUT1 << 0));
    i2cCodecCtrl_CODEC->buffer[0] = 0x10;
    i2cCodecCtrl_CODEC->buffer[1] = dataReg->DACChannelInvert;
    return i2cCodecCtrl_CODEC->writeBytes();
}

//The level of AIN1 - AIN6 can be adjusted in 0.5 dB increments as dictated by the ADC Soft and Zero Cross
//bits (ADC_SZC[1:0]) from +24 to -64 dB. Levels are decoded in two’s complement, as shown in Table 15.
bool CS42448::Reg11_16hAINVolumeCtrl(unsigned char channel, unsigned char AINX_VOL)
{
    if (AINX_VOL>=128) AINX_VOL -= 128;
    else AINX_VOL += 128;
    AINX_VOL = 255 - AINX_VOL;
    dataReg->aOutVolumeCtrl[channel-1] = AINX_VOL;
    i2cCodecCtrl_CODEC->buffer[0] = 0x11+(channel - 1);
    i2cCodecCtrl_CODEC->buffer[1] = AINX_VOL;
    return i2cCodecCtrl_CODEC->writeBytes();
}



bool CS42448::Reg17hADCChannelInvert(unsigned char INV_AIN6, unsigned char INV_AIN5, unsigned char INV_AIN4, unsigned char INV_AIN3, unsigned char INV_AIN2, unsigned char INV_AIN1)
{
    dataReg->ADCChannelInvert = char((0 << 7) | (0 << 6) | (INV_AIN6 << 5) | (INV_AIN5 << 4) | (INV_AIN4 << 3) | (INV_AIN3 << 2) | (INV_AIN2 << 1) | (INV_AIN1 << 0));
    i2cCodecCtrl_CODEC->buffer[0] = 0x17;
    i2cCodecCtrl_CODEC->buffer[1] = dataReg->ADCChannelInvert;
    return i2cCodecCtrl_CODEC->writeBytes();
}


bool CS42448::MuteAudioOutput(bool mute, int channel)
{
    if (mute){
        dataReg->DACChannelMute |= (1 << (channel-1));
    }else{
        dataReg->DACChannelMute &= (~(1 << (channel-1)));
    }
    i2cCodecCtrl_CODEC->buffer[0] = 0x07;
    i2cCodecCtrl_CODEC->buffer[1] = dataReg->DACChannelMute;
    return i2cCodecCtrl_CODEC->writeBytes();
}

