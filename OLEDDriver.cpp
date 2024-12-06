#include "OLEDDriver.h"
#include <QtDebug>

OLEDDriver::OLEDDriver(char *spidev)
{
    spiDev = spidev;
    spi_init();
    for (int i=0;i<BUFFERSIZE;i++) {
        tx[i] = 0;
    }
}

void OLEDDriver::writeOLEDRun()
{
    writeOLED();
}
void OLEDDriver::updateIPAddress(QString networkID, QString ipaddress, bool connected)
{
    QStringList listIP =  ipaddress.split(".");
    if (listIP.size() == 4)
    {
        if (networkID == "eth0")
        {
            tx[IPADDRESS1_0] = static_cast<uint8_t>(listIP.at(0).toInt());
            tx[IPADDRESS1_1] = static_cast<uint8_t>(listIP.at(1).toInt());
            tx[IPADDRESS1_2] = static_cast<uint8_t>(listIP.at(2).toInt());
            tx[IPADDRESS1_3] = static_cast<uint8_t>(listIP.at(3).toInt());
            tx[ETH1_CONN] = connected;
            qDebug() << tx[IPADDRESS1_0];
        }
        else if (networkID == "eth1")
        {
            tx[IPADDRESS2_0] = static_cast<uint8_t>(listIP.at(0).toInt());
            tx[IPADDRESS2_1] = static_cast<uint8_t>(listIP.at(1).toInt());
            tx[IPADDRESS2_2] = static_cast<uint8_t>(listIP.at(2).toInt());
            tx[IPADDRESS2_3] = static_cast<uint8_t>(listIP.at(3).toInt());
            tx[ETH2_CONN] = connected;
        }
    }
    //writeOLED();
}

void OLEDDriver::update_iGateInfo(uint8_t id, uint8_t txStatus, uint8_t rxStatus, uint8_t connNum,uint8_t numTxRx)
{
    switch (id)
    {
    case 1:
        tx[TRX1_INFO] = static_cast<uint8_t>(rxStatus | (txStatus << 1));
        tx[TRX1_NUMCONN] = static_cast<uint8_t>(connNum);
        tx[IGATE1_NUMTRX] = static_cast<uint8_t>(numTxRx);
        break;
    case 2:
        tx[TRX2_INFO] = static_cast<uint8_t>(rxStatus | (txStatus << 1));
        tx[TRX2_NUMCONN] = static_cast<uint8_t>(connNum);
        tx[IGATE2_NUMTRX] = static_cast<uint8_t>(numTxRx);
        break;
    case 3:
        tx[TRX3_INFO] = static_cast<uint8_t>(rxStatus | (txStatus << 1));
        tx[TRX3_NUMCONN] = static_cast<uint8_t>(connNum);
        tx[IGATE3_NUMTRX] = static_cast<uint8_t>(numTxRx);
        break;
    case 4:
        tx[TRX4_INFO] = static_cast<uint8_t>(rxStatus | (txStatus << 1));
        tx[TRX4_NUMCONN] = static_cast<uint8_t>(connNum);
        tx[IGATE4_NUMTRX] = static_cast<uint8_t>(numTxRx);
        break;
    }
    //writeOLED();
}

void OLEDDriver::update_igateRun(uint8_t id, uint8_t val)
{
    switch (id) {
    case 1:
        tx[IGATE1_RUN] = static_cast<uint8_t>(val);
        break;
    case 2:
        tx[IGATE2_RUN] = static_cast<uint8_t>(val);
        break;
    case 3:
        tx[IGATE3_RUN] = static_cast<uint8_t>(val);
        break;
    case 4:
        tx[IGATE4_RUN] = static_cast<uint8_t>(val);
        break;
    }
    //writeOLED();
}
void OLEDDriver::checkSum()
{
    uint8_t chk = 0;
    for (int i=0;i<BUFFERSIZE-1;i++)
    {
        chk ^= tx[i];
    }
    tx[CHECKSUM] = chk;
}
bool OLEDDriver::checkNewData()
{
    for (int i=0;i<BUFFERSIZE;i++)
    {
        if (tx[i] != tx_tmp[i]){
//            qDebug() << "send new oled data" << tx[IGATE1_RUN] << tx[IGATE2_RUN] << tx[IGATE3_RUN] << tx[IGATE4_RUN];
            return true;
        }
    }
    return false;
}
void OLEDDriver::writeOLED()
{
    if (checkNewData() == false) return;
    messageID++;
    tx[MESSAGEID] = messageID;
    tx[0] = 0xAA;
    tx[1] = 0xBB;
    checkSum();
    //send_byte_data(tx,BUFFERSIZE);
    for (int i=0;i<BUFFERSIZE;i++)
    {
        tx_tmp[i] = tx[i];
        send_byte_data(tx+i,1);
    }
}
void OLEDDriver::spi_init()
{
    fd_spi = open(spiDev, O_RDWR);
    if (fd_spi < 0)
        qDebug() << "can't open device";
    ret_spi = ioctl(fd_spi, SPI_IOC_WR_MODE, &mode_spi);
    if (ret_spi == -1)
        qDebug() << ("can't set spi mode");
    ret_spi = ioctl(fd_spi, SPI_IOC_RD_MODE, &mode_spi);
    if (ret_spi == -1)
        qDebug() << ("can't get spi mode");
    ret_spi = ioctl(fd_spi, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret_spi == -1)
        qDebug() << ("can't set bits per word");
    ret_spi = ioctl(fd_spi, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret_spi == -1)
        qDebug() << ("can't get bits per word");
    ret_spi = ioctl(fd_spi, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret_spi == -1)
        qDebug() << ("can't set max speed hz");
    ret_spi = ioctl(fd_spi, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret_spi == -1)
        qDebug() << ("can't get max speed hz");

    printf("spi mode: 0x%x\n", mode_spi);
    printf("bits per word: %d\n", bits);
    printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
}

bool OLEDDriver::send_byte_data(uint8_t *byteData, uint8_t len)
{
    struct spi_ioc_transfer tr = {
        tr.tx_buf = (unsigned long)byteData,
        tr.rx_buf = (unsigned long)rx,
        tr.len = len,
        tr.delay_usecs = delay,
        static_cast<__u16>(tr.speed_hz = speed),
        tr.bits_per_word = bits,
    };
    if (mode_spi & SPI_TX_QUAD)
        tr.tx_nbits = 4;
    else if (mode_spi & SPI_TX_DUAL)
        tr.tx_nbits = 2;
    if (mode_spi & SPI_RX_QUAD)
        tr.rx_nbits = 4;
    else if (mode_spi & SPI_RX_DUAL)
        tr.rx_nbits = 2;
    if (!(mode_spi & SPI_LOOP)) {
        if (mode_spi & (SPI_TX_QUAD | SPI_TX_DUAL))
            tr.rx_buf = 0;
        else if (mode_spi & (SPI_RX_QUAD | SPI_RX_DUAL))
            tr.tx_buf = 0;
    }
    ret_spi = ioctl(fd_spi, SPI_IOC_MESSAGE(1), &tr);
    if (ret_spi < 0){
        qDebug() << "can't send_byte_data" << ret_spi;
        return  false;
    }
    return  true;
}


