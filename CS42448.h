#ifndef CS42448_H
#define CS42448_H
#include "I2CReadWrite.h"
#include <QDebug>
#include <string>
#define CODEC1          1


#define CODEC1I2CDEV   "/dev/i2c-0" //I2S1


#define CODECI2CADDR_CS42448   0x48

#define VOIPCH1_I2S1 1
#define VOIPCH2_I2S1 2
#define VOIPCH3_I2S1 3
#define VOIPCH4_I2S1 4


class CS42448
{
public:
    CS42448(QString i2cdevice, int address);
    /*
     * Power Control (Address 02h)
     * 7 6 5 4 3 2 1 0
     * PDN_ADC3 | PDN_ADC2 | PDN_ADC1 | PDN_DAC4 | PDN_DAC3 | PDN_DAC2 | PDN_DAC1 | PDN
     */
    bool Reg02h_PowerDownCtrl(unsigned char PDN_ADC3, unsigned char PDN_ADC2, unsigned char PDN_ADC1, unsigned char PDN_DAC4, unsigned char PDN_DAC3, unsigned char PDN_DAC2, unsigned char PDN_DAC1, unsigned char PDN);

    /*
     * Functional Mode (Address 03h)
     * DAC_FM1 | DAC_FM0 | ADC_FM1 | ADC_FM0 | MFreq2 | MFreq1 | MFreq0 | Reserved
     */
    bool Reg03h_FunctionalMode(unsigned char DAC_FM, unsigned char ADC_FM, unsigned char MFREQ);

    /*
     * Interface Formats (Address 04h)
     * FREEZE | AUX_DIF | DAC_DIF2 | DAC_DIF1 | DAC_DIF0 | ADC_DIF2 | ADC_DIF1 | ADC_DIF0
     */

    bool Reg04h_InterfaceFormats(unsigned char FREEZE, unsigned char AUX_DIF, unsigned char DAC_DIF, unsigned char ADC_DIF);

    /*
     * ADC Control & DAC De-Emphasis (Address 05h)
     * ADC1-2_HPF FREEZE | ADC3_HPF FREEZE | DAC_DEM | ADC1 SINGLE | ADC2 SINGLE | ADC3 SINGLE | AIN5_MUX | AIN6_MUX
     */
    bool Reg05h_ADCCtrlDACDeEmphasis(unsigned char ADC1_2_HPF_FREEZE, unsigned char ADC3_HPF_FREEZE, unsigned char DAC_DEM, unsigned char ADC1_SINGLE, unsigned char ADC2_SINGLE, unsigned char ADC3_SINGLE, unsigned char AIN5_MUX, unsigned char AIN6_MUX);

    /*
     * Transition Control (Address 06h)
     * DAC_SNGVOL | DAC_SZC1 | DAC_SZC0 | AMUTE | MUTE_ADC_SP | ADC_SNGVOL | ADC_SZC1 | ADC_SZC0
     */
    bool Reg06h_TransitionControl(unsigned char DAC_SNGVOL, unsigned char DAC_SZC1, unsigned char DAC_SZC0, unsigned char AMUTE, unsigned char MUTE_ADC_SP, unsigned char ADC_SNGVOL, unsigned char ADC_SZC1, unsigned char ADC_SZC0);

    /*
     * DAC Channel Mute (Address 07h)
     * AOUT8_MUTE | AOUT7_MUTE | AOUT6_MUTE | AOUT5_MUTE | AOUT4_MUTE | AOUT3_MUTE | AOUT2_MUTE | AOUT1_MUTE
     */
    bool Reg07hDACChannelMute(unsigned char AOUT8_MUTE, unsigned char AOUT7_MUTE, unsigned char AOUT6_MUTE, unsigned char AOUT5_MUTE, unsigned char AOUT4_MUTE, unsigned char AOUT3_MUTE, unsigned char AOUT2_MUTE, unsigned char AOUT1_MUTE);

    void DACChannelMute(unsigned char channel, unsigned char AOUT_MUTE);

    /*
     * AOUTX Volume Control (Addresses 08h- 0Fh)
     * AOUTx_VOL7 | AOUTx_VOL6 | AOUTx_VOL5 | AOUTx_VOL4 | AOUTx_VOL3 | AOUTx_VOL2 | AOUTx_VOL1 | AOUTx_VOL0
     * The AOUTx Volume Control registers allow independent setting of the signal levels in 0.5 dB increments
     * from 0 dB to -127.5 dB. Volume settings are decoded as shown in Table 14. The volume changes are
     * implemented as dictated by the Soft and Zero Cross bits (DAC_SZC[1:0]). All volume settings less than
     * -127.5 dB are equivalent to enabling the AOUTx_MUTE bit for the given channel.
     */
    bool Reg08_0FhAOUTXVolumeCtrl(unsigned char channel, unsigned char AOUTX_VOL);

    /*
     * DAC Channel Invert (Address 10h)
     * INV_AOUT8 | INV_AOUT7 | INV_AOUT6 | INV_AOUT5 | INV_AOUT4 | INV_AOUT3 | INV_AOUT2 | INV_AOUT1
     */
    bool Reg10hDACChannelInvert(unsigned char INV_AOUT8, unsigned char INV_AOUT7, unsigned char INV_AOUT6, unsigned char INV_AOUT5, unsigned char INV_AOUT4, unsigned char INV_AOUT3, unsigned char INV_AOUT2, unsigned char INV_AOUT1);

    /*
     * AINX Volume Control (Address 11h-16h) 7 6 5 4 3 2 1 0
     * AINx_VOL7 AINx_VOL6 AINx_VOL5 AINx_VOL4 AINx_VOL3 AINx_VOL2 AINx_VOL1 AINx_VOL0
     */


    bool Reg11_16hAINVolumeCtrl(unsigned char channel, unsigned char AINX_VOL);

    bool Reg17hADCChannelInvert(unsigned char INV_AIN6, unsigned char INV_AIN5, unsigned char INV_AIN4, unsigned char INV_AIN3, unsigned char INV_AIN2, unsigned char INV_AIN1);

    bool MuteAudioOutput(bool mute, int channel);
    bool active = false;

private:
    void initCodec();
    struct CodecDataReg
    {
        unsigned char powerDown = 0;                  //2h
        unsigned char functionMode = 0xf0;            //3h
        unsigned char interfaceFormat = 0x36;         //4h
        unsigned char ADCControlDACDeEmphasis = 0x00; //5h
        unsigned char TransitionControl = 0x10;       //6h Auto mute had enable
        unsigned char DACChannelMute = 0x00;          //7h
        unsigned char aOutVolumeCtrl[8] = {0x00};     //8h - 0fh
        unsigned char DACChannelInvert = 0x00;        //10h
        unsigned char aInVolumeCtrl[6] = {0x00};      //11h - 16h
        unsigned char ADCChannelInvert = 0x00;        //17h
    };
    QString i2cDev;
    int i2cAddress = 0;
    CodecDataReg *dataReg;
    I2CReadWrite *i2cCodecCtrl_CODEC;

};

#endif // CS42448_H
