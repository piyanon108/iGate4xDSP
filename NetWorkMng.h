#ifndef NETWORKMNG_H
#define NETWORKMNG_H
#include <QString>
#include <QObject>
#include "QProcess"

class NetWorkMng : public QObject
{
    Q_OBJECT
public:
    explicit NetWorkMng(QObject *parent = 0);
    virtual ~NetWorkMng();
    bool get_gst_launch_process();
    QString getAddress(QString netWorkCard);
    void getIPAddress(QString netWorkCard);
    bool getLinkDetected(QString networkCard);
    QString getTimezone();
    bool checkPhyEth(QString eth);
    void resetNtp();
    void initPhyNetworkConfig(QString ipaddr,uint8_t netmask,QString gateway,QString dns1,QString dns2, QString phyNetworkName);
    void setStaticIpAddr(QString ipaddr, uint8_t netmask, QString gateway, QString dns1, QString dns2, QString phyNetworkName);
    void setDHCPIpAddr(QString phyNetworkName);
    void setNTPServer(QString ntpServer);
    void setDHCPIpAddr2(QString phyNetworkName);
    void setDHCPIpAddr3(QString phyNetworkName);
    void setStaticIpAddr2(QString ipaddr, QString netmask, QString gateway, QString dns1, QString dns2, QString phyNetworkName);
    void setStaticIpAddr3(QString ipaddr, QString netmask, QString gateway, QString dns1, QString dns2, QString phyNetworkName);
    void setTimeOutNetwork(int timeout);
    bool checkAddress(QString address);
    int calPrefix(QString mask);

    QString netWorkCardMac;
    QString netWorkCardAddr;
    QString netWorkCardMask;
    QString netWorkCardGW;
    QString netWorkCardDNS;
    bool linkDetected;

    bool eth0 = false;
    bool eth1 = false;

    QString eth0Config;
    QString eth1Config;
    void restartNetwork();
    float getCPUTemp();
signals:
    void gstNotRun();

    void newAddress();
public slots:
    void resetNetwork();
    void resetNetwork2();
private:
    QProcess* getAddressProcess;
    QProcess* gstLaunchProcess;

    static int bit_count(uint32_t i);
};

#endif // NETWORKMNG_H
