#ifndef ADAU1467_H
#define ADAU1467_H
#include <string>
#include "linux/spi/spidev.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <QString>
//#include <iGate4CH_DSP/selfbootdata.h>
#include <iGate4CH_DSP/DesignDSP_REC_V1/DesignDSP_REC_V1_IC_1_PARAM.h>
#include <SPI.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define WRITE 0
#define READ 1
#define DSPDATABUF 4096 // 4096

typedef unsigned short ADI_DATA_U16;
typedef unsigned char ADI_REG_TYPE;
typedef unsigned char byte;
/*
 * Parameter data format
 */
#define SIGMASTUDIOTYPE_FIXPOINT 0
#define SIGMASTUDIOTYPE_INTEGER  1

#define DSP_TYPE_SIGMA300_350 3    // Sigma300/Sigma350: ADAU145x, ADAU146x

// Which type of DSP is connected?
#define DSP_TYPE DSP_TYPE_SIGMA300_350


// For compatibility with certain export files, redirect SIGMASTUDIOTYPE_8_24_CONVERT to
// SIGMASTUDIOTYPE_FIXPOINT_CONVERT
#define SIGMASTUDIOTYPE_8_24_CONVERT(x) ADAU1467::SIGMASTUDIOTYPE_FIXPOINT_CONVERT(x)
// Separate a 32-bit floating point value into four bytes

class ADAU1467
{
public:
    ADAU1467(char *spidev);

    /*
     * Convert a floating-point value to SigmaDSP (5.23 or 8.24) fixed point format
     */
    #if DSP_TYPE == DSP_TYPE_SIGMA300_350
    int32_t SIGMASTUDIOTYPE_FIXPOINT_CONVERT(double value) { return int32_t(value * (0x01 << 24)); }
    #else
    int32_t SIGMASTUDIOTYPE_FIXPOINT_CONVERT(double value) { return int32_t(value * (0x01 << 23)) & 0xFFFFFFF; }
    #endif

    // Separate a 32-bit floating point value into four bytes
    void SIGMASTUDIOTYPE_REGISTER_CONVERT(int32_t fixpt_val, byte dest[4]);
    void SIGMASTUDIOTYPE_INTEGER_CONVERT(int32_t fixpt_val, byte dest[4]);
    void SIGMA_WRITE_REGISTER_BLOCK(byte devAddress, int address, int length, byte pData[]);
    void SIGMA_WRITE_REGISTER_BLOCK(int address, int length, byte pData[]);
    void SIGMA_WRITE_REGISTER_INTEGER(int address, int32_t pData);
    void SIGMA_WRITE_REGISTER_FLOAT(int address, double pData);
    void SIGMA_WRITE_DELAY(byte devAddress, int length, byte pData[]);
    void SIGMA_READ_REGISTER_BYTES(uint16_t address, int length, byte *pData);
    uint32_t SIGMA_READ_REGISTER_INTEGER(uint16_t address, int length);
    double SIGMA_READ_REGISTER_FLOAT(int address);
    void SIGMA_PRINT_REGISTER(int address, int dataLength);
    void default_download_IC_1();
    void WriteSelfBootMemoryToDSP();

    void setMixerVolume(uint16_t address, uint16_t modeAddress, uint32_t modeValue, double value);
    void setModuleSingleVolume(uint16_t targetAddress, uint16_t slewModeAddress, uint32_t modeValue, double value);
    //TONE
    void setToneVolume(uint16_t address, double value);

    void setFIRfilter(int fir_start_addr, int fir_filter_length, double* coefficients);

    void setDSPSplitVolume(uint16_t address, double value);
private:
    void saveLoadWrite0x6000(double dValue);
    void blokcWrite(uint16_t address);
    void blokcWrite(uint16_t modeAddress, uint32_t modeValue);


//    void spi_init();
    bool send_byte_data(uint8_t *byteData, uint32_t len);
    void transfer(int fd, uint8_t const *tx, uint8_t const *rx, size_t len);
//    int mode_spi=0;
//    int ret_spi=0;
//    int fd_spi;
//    uint8_t bits = 8;
//    uint32_t speed = 500000;
//    uint16_t delay = 100;
    uint8_t tx_tmp[DSPDATABUF];
    uint8_t tx[DSPDATABUF];
    uint8_t rx[ARRAY_SIZE(tx)] = {0, };

    SPIClass *DSPSPI;
    char *spiDev;

};

#endif // ADAU1467_H
