#ifndef OLEDDRIVER_H
#define OLEDDRIVER_H

#include <string>
#include "linux/spi/spidev.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <QString>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define WRITE 0
#define READ 1

#define BUFFERSIZE           30
#define IPADDRESS1_0 		3
#define IPADDRESS1_1 		4
#define IPADDRESS1_2 		5
#define IPADDRESS1_3 		6
#define IPADDRESS2_0 		7
#define IPADDRESS2_1 		8
#define IPADDRESS2_2 		9
#define IPADDRESS2_3 		10

#define	ETH1_CONN			11
#define	ETH2_CONN			12

#define TRX1_INFO			13
#define TRX2_INFO			14
#define TRX3_INFO			15

#define TRX1_NUMCONN		16
#define TRX2_NUMCONN		17
#define TRX3_NUMCONN		18

#define IGATE1_NUMTRX       19
#define IGATE2_NUMTRX       20
#define IGATE3_NUMTRX       21

#define IGATE1_RUN			22
#define IGATE2_RUN			23
#define IGATE3_RUN			24

#define CHECKSUM           (BUFFERSIZE-1)

#define MESSAGEID			2

// ADD Igate4
#define TRX4_INFO			25
#define TRX4_NUMCONN		26
#define IGATE4_NUMTRX       27
#define IGATE4_RUN			28

#define SPI_MODE        SPI_MODE_0
#define SPI_FREQUENCY   (5000000)
class OLEDDriver
{
public:

    OLEDDriver(char *spidev);
    void writeOLEDRun();
    void updateIPAddress(QString networkID, QString ipaddress, bool connected);
    void update_iGateInfo(uint8_t id, uint8_t txStatus, uint8_t rxStatus, uint8_t connNum, uint8_t numTxRx);
    void update_igateRun(uint8_t id, uint8_t val);
private:
    bool checkNewData();
    void checkSum();
    void writeOLED();
    void spi_init();
    bool send_byte_data(uint8_t *byteData, uint8_t len);
    void transfer(int fd, uint8_t const *tx, uint8_t const *rx, size_t len);
    int mode_spi=SPI_MODE;
    int ret_spi=0;
    int fd_spi = 0;
    uint8_t bits = 8;
    uint32_t speed = SPI_FREQUENCY;
    uint16_t delay = 10;
    uint8_t tx_tmp[BUFFERSIZE] = {0, };
    uint8_t tx[BUFFERSIZE] = {0, };
    uint8_t rx[ARRAY_SIZE(tx)] = {0, };

    char *spiDev = "";
    uint8_t messageID = 0;



};

#endif // OLEDDRIVER_H
