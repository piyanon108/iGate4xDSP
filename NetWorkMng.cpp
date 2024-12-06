#include "NetWorkMng.h"
#include "QDebug"
#include <QFile>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
NetWorkMng::NetWorkMng(QObject *parent) : QObject(parent)
{
    getAddressProcess = new QProcess(this);
    gstLaunchProcess = new QProcess(this);
}
NetWorkMng::~NetWorkMng(){
    delete getAddressProcess;
}
bool NetWorkMng::get_gst_launch_process()
{
    QString prog = "/bin/bash";//shell
    QStringList arguments = {"-c" , "pgrep gst-launch-1.0"};
    QString output;
//    arguments << "-c" << "pgrep gst-launch-1.0";
    gstLaunchProcess->start(prog , arguments);
    gstLaunchProcess->waitForFinished(100);
    output = QString(gstLaunchProcess->readAll()).trimmed();
    arguments.clear();
    if (output != "") return  true;
    return  false;
}
void NetWorkMng::getIPAddress(QString netWorkCard){
    QString prog = "/bin/bash";//shell
//    QStringList arguments = {"-c" , QString("ifconfig %1 | grep 'inet ' | awk '{print $2}' | sed 's/addr://'").arg(netWorkCard)};
//    arguments << "-c" << QString("ifconfig %1 | grep 'inet ' | awk '{print $2}' | sed 's/addr://'").arg(netWorkCard);
    getAddressProcess->start(prog , QStringList{"-c" , QString("ifconfig %1 | grep 'inet ' | awk '{print $2}' | sed 's/addr://'").arg(netWorkCard)});
    getAddressProcess->waitForFinished(100);
    netWorkCardAddr = QString(getAddressProcess->readAll()).trimmed();
//    arguments.clear();
}
bool NetWorkMng::getLinkDetected(QString networkCard)
{
    QString prog = "/bin/bash";//shell
    QStringList arguments = {"-c" , QString("ethtool %1 | grep \"Link detected\"").arg(networkCard)};
//    arguments << "-c" << QString("ethtool %1 | grep \"Link detected\"").arg(networkCard);
//    qDebug() << "getLinkDetected" << arguments;
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished(100);
    linkDetected = QString(getAddressProcess->readAll()).contains("Link detected: yes");
    arguments.clear();
    return  linkDetected;
}
QString NetWorkMng::getTimezone()
{
    QString prog = "/bin/bash";//shell
    QStringList arguments = {"-c" , QString("ls -la /etc/localtime | grep '/usr/share/zoneinfo/' | awk '{print $11}'")};
    QString Timezone = "";
//    arguments << "-c" << QString("ls -la /etc/localtime | grep '/usr/share/zoneinfo/' | awk '{print $11}'");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished(100);
    Timezone = QString(getAddressProcess->readAll()).trimmed();
    arguments.clear();
    qDebug() << "Timezone" << Timezone;
    return Timezone.replace("/usr/share/zoneinfo/","");
}

bool NetWorkMng::checkPhyEth(QString eth)
{
    QString prog = "/bin/bash";//shell
    QStringList arguments = {"-c" , QString("ifconfig %1 | grep '%1'").arg(eth)};
    QString output = "";
//    arguments << "-c" << QString("ifconfig %1 | grep '%1'").arg(eth);
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished(100);
    output = QString(getAddressProcess->readAll()).trimmed();
    arguments.clear();
    return output.contains(eth);
}

QString NetWorkMng::getAddress(QString netWorkCard){
    QString networkInfo;
    QString prog = "/bin/bash";//shell
    QStringList arguments = {"-c" , QString("ifconfig %1 | grep 'inet ' | awk '{print $2}' | sed 's/addr://'").arg(netWorkCard)};
//    arguments << "-c" << QString("ifconfig %1 | grep 'inet ' | awk '{print $2}' | sed 's/addr://'").arg(netWorkCard);
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished(100);
    netWorkCardAddr = QString(getAddressProcess->readAll()).trimmed();

    arguments.clear();
    arguments << "-c" << QString("ifconfig %1 | grep 'ether'  | awk '{print $2}'").arg(netWorkCard);
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished(100);
    netWorkCardMac = QString(getAddressProcess->readAll()).trimmed();

    arguments.clear();
    arguments << "-c" << QString("ifconfig %1 | grep 'inet ' | awk '{print $4}' | sed 's/Mask://'").arg(netWorkCard);
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished(100);
    netWorkCardMask = QString(getAddressProcess->readAll()).trimmed();

    arguments.clear();
    arguments << "-c" << QString("ip -4 route list 0/0  |  awk '/default/ { print $3 }'");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished(100);
    netWorkCardGW = QString(getAddressProcess->readAll()).trimmed();

    arguments.clear();
    arguments << "-c" << QString("cat /etc/resolv.conf  | grep nameserver | awk '{print $2}'");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished(100);
    netWorkCardDNS = QString(getAddressProcess->readAll()).trimmed();
    netWorkCardDNS = netWorkCardDNS.replace("\n",",");

    networkInfo = "IP Address," + netWorkCardAddr +"\tNetmask," +netWorkCardMask + "\tDefault Gateway," +netWorkCardGW +
            "\tDNS Server," +netWorkCardDNS + "\tMAC Address," +netWorkCardMac;

    arguments.clear();
    return networkInfo;
}

void NetWorkMng::resetNetwork()
{
    QString prog = "/bin/bash";//shell
    QStringList arguments = {"-c" , "ifconfig eth0 down"};
    qDebug() << "networking restarting";

//    arguments << "-c" << QString("ifconfig eth0 down");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();

    arguments.clear();
    arguments << "-c" << QString("ifconfig eth0 up");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();

    arguments << "-c" << QString("ifconfig eth1 down");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();

    arguments.clear();
    arguments << "-c" << QString("ifconfig eth1 up");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();

    arguments.clear();
    arguments << "-c" << QString("systemctl daemon-reload");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();

    arguments.clear();
    arguments << "-c" << QString("/etc/init.d/dhcpcd restart");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();
    arguments.clear();
    for(int j=0; j < 10000; j++)
    {
        for(int i=0; i < 10000000; i++){}
    }
    qDebug() << "networking restarted";
}
void NetWorkMng::resetNetwork2(){
    QString prog = "/bin/bash";//shell
    QStringList arguments = {"-c" , "ifconfig eth0 down"};
    qDebug() << "networking restarting";
//    arguments << "-c" << QString("ifconfig eth0 down");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();

    qDebug() << "networking restarting";
    arguments.clear();
    arguments << "-c" << QString("ifconfig eth1 down");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();

    arguments.clear();
    arguments << "-c" << QString("ifconfig eth0 up");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();

    arguments.clear();
    arguments << "-c" << QString("ifconfig eth1 up");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();

//    arguments.clear();
//    arguments << "-c" << QString("systemctl daemon-reload");
//    getAddressProcess->start(prog , arguments);
//    getAddressProcess->waitForFinished();

    arguments.clear();
    arguments << "-c" << QString("/etc/init.d/networking restart");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();
    arguments.clear();
    qDebug() << "networking restarted";

    getAddress("eth0");
    getAddress("eth1");
}
void NetWorkMng::resetNtp(){
    QString prog = "/bin/bash";//shell
    QStringList arguments = {"-c" , "systemctl daemon-reload"};
//    arguments << "-c" << QString("systemctl daemon-reload");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();

    arguments.clear();
    arguments << "-c" << QString("systemctl restart systemd-timesyncd");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();
    arguments.clear();

    for(int j=0; j < 10000; j++){
        for(int i=0; i < 10000; i++){}
    }

    arguments.clear();
    arguments << "-c" << QString("systemctl restart systemd-timesyncd");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();
    arguments.clear();
    qDebug() << "systemctl restart systemd-timesyncd";
}
//int NetWorkMng::setNTPServer(QString ntpServer)
//{
//    ifstream filein("/etc/dhcpcd.conf"); //File to read from
//    ofstream fileout("/tmp/dhcpcd.conf"); //Temporary file

//    if(!filein || !fileout)
//    {
//        cout << "Error opening files!" << endl;
//        return 1;
//    }
//    string strTemp;
//       //bool found = false;
//    while ( std::getline(filein, strTemp) )
//    {
//        if(QString::fromStdString(strTemp).contains("option ntp_servers")){
//            strTemp = "option ntp_servers "+ntpServer.toStdString();
//        }
//        fileout << strTemp << '\n';
//    }
//    QString prog = "/bin/bash";//shell
//    QStringList arguments;
//    arguments << "-c" << QString("cp /tmp/dhcpcd.conf /etc/dhcpcd.conf");
//    getAddressProcess->start(prog , arguments);
//    getAddressProcess->waitForFinished();
//    arguments.clear();
//    return 0;
//}
void NetWorkMng::setTimeOutNetwork(int timeout){
QString filename = "/usr/local/bin/ping.sh";
float NetworkTimeout = timeout/1000.0;
//qDebug()<<"network"<<NetworkTimeout;
QString data;
    if (timeout != 0.1)
        data =  QString("#!/bin/bash\n"
                        "timeout -k %1 %1 ping -c1 $1 > /dev/null\n"
                        "if [ $? -eq 0 ]\n"
                        " then\n"
                        "   echo 1\n"
                        "   exit 0\n"
                        " else\n"
                        "   echo 0\n"
                        "   exit 1\n"
                        "fi\n").arg(NetworkTimeout);
    else
        data =  QString("#!/bin/bash\n"
                        "timeout -k 0.1 0.1 ping -c1 $1 > /dev/null\n"
                        "if [ $? -eq 0 ]\n"
                        " then\n"
                        "   echo 1\n"
                        "   exit 0\n"
                        " else\n"
                        "   echo 0\n"
                        "   exit 1\n"
                        "fi\n");

    QByteArray dataAyyay(data.toLocal8Bit());
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << dataAyyay;
    file.close();
}
void NetWorkMng::setNTPServer(QString ntpServer){
    QString filename = "/etc/systemd/timesyncd.conf";
    QString data = "";
    if (ntpServer != "")
        data = QString("[Time]\n"
                   "NTP=%1\n"
                   "FallbackNTP=0.debian.pool.ntp.org 1.debian.pool.ntp.org 2.debian.pool.ntp.org 3.debian.pool.ntp.org\n").arg(ntpServer);
    else
        data = QString("[Time]\n"
                   "#NTP=%1\n"
                   "#FallbackNTP=0.debian.pool.ntp.org 1.debian.pool.ntp.org 2.debian.pool.ntp.org 3.debian.pool.ntp.org\n").arg(ntpServer);

    QByteArray dataAyyay(data.toLocal8Bit());
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << dataAyyay;
    file.close();
    system("systemctl restart systemd-timesyncd.service");
    system("hwclock -w > /dev/null 2>&1 &");
}
void NetWorkMng::initPhyNetworkConfig(QString ipaddr,uint8_t netmask,QString gateway,QString dns1,QString dns2, QString phyNetworkName)
{
    QString data = "";
    if (netmask <= 0) netmask = 24;
    if(((ipaddr!="0.0.0.0") && (netmask > 0) && (gateway!="") && (dns1!="") && (dns2!="")) ||
            ((ipaddr!="0.0.0.0") && (netmask > 0) && (gateway!="") && (dns1=="") && (dns2!="")) ||
            ((ipaddr!="0.0.0.0") && (netmask > 0) && (gateway!="") && (dns1!="") && (dns2==""))){
        data = QString("# Static IP configuration:\n"
                           "interface %6\n"
                           "    static ip_address=%1/%2\n"
                           "    static routers=%3\n"
                           "    static domain_name_servers=%4 %5\n").arg(ipaddr).arg(netmask).arg(gateway).arg(dns1).arg(dns2).arg(phyNetworkName);
    }
    else if((ipaddr!="0.0.0.0") && (netmask > 0) && (gateway!="") && (dns1=="") && (dns2=="")){
        data = QString("# Static IP configuration:\n"
                           "interface %6\n"
                           "    static ip_address=%1/%2\n"
                           "    static routers=%3\n"
                           "#   static domain_name_servers=%4 %5\n").arg(ipaddr).arg(netmask).arg(gateway).arg(dns1).arg(dns2).arg(phyNetworkName);
    }
    else if((ipaddr!="0.0.0.0") && (netmask > 0) && (gateway=="")){
        data = QString("# Static IP configuration:\n"
                           "interface %6\n"
                           "    static ip_address=%1/%2\n"
                           "#   static routers=%3\n"
                           "#   static domain_name_servers=%4 %5\n").arg(ipaddr).arg(netmask).arg(gateway).arg(dns1).arg(dns2).arg(phyNetworkName);
    }

    if (phyNetworkName == "eth0")
        eth0Config = data;
    else if (phyNetworkName == "eth1")
        eth1Config = data;
}

void NetWorkMng::setStaticIpAddr(QString ipaddr,uint8_t netmask,QString gateway,QString dns1,QString dns2, QString phyNetworkName)
{
    QString filename = "/etc/dhcpcd.conf";
    QString data = "";
    QString fileHeader = "hostname\n"
                         "clientid\n"
                         "persistent\n"
                         "option rapid_commit\n"
                         "option domain_name_servers, domain_name, domain_search, host_name\n"
                         "option classless_static_routes\n"
                         "option ntp_servers\n"
                         "option interface_mtu\n"
                         "require dhcp_server_identifier\n"
                         "slaac private\n"
                         "\n";


    if (netmask <= 0) netmask = 24;
    if(((ipaddr!="0.0.0.0") && (netmask > 0) && (gateway!="") && (dns1!="") && (dns2!="")) ||
            ((ipaddr!="0.0.0.0") && (netmask > 0) && (gateway!="") && (dns1=="") && (dns2!="")) ||
            ((ipaddr!="0.0.0.0") && (netmask > 0) && (gateway!="") && (dns1!="") && (dns2==""))){
        data = QString("# Static IP configuration:\n"
                           "interface %6\n"
                           "    static ip_address=%1/%2\n"
                           "    static routers=%3\n"
                           "    static domain_name_servers=%4 %5\n").arg(ipaddr).arg(netmask).arg(gateway).arg(dns1).arg(dns2).arg(phyNetworkName);
    }
    else if((ipaddr!="0.0.0.0") && (netmask > 0) && (gateway!="") && (dns1=="") && (dns2=="")){
        data = QString("# Static IP configuration:\n"
                           "interface %6\n"
                           "    static ip_address=%1/%2\n"
                           "    static routers=%3\n"
                           "#   static domain_name_servers=%4 %5\n").arg(ipaddr).arg(netmask).arg(gateway).arg(dns1).arg(dns2).arg(phyNetworkName);
    }
    else if((ipaddr!="0.0.0.0") && (netmask > 0) && (gateway=="")){
        data = QString("# Static IP configuration:\n"
                           "interface %6\n"
                           "    static ip_address=%1/%2\n"
                           "#   static routers=%3\n"
                           "#   static domain_name_servers=%4 %5\n").arg(ipaddr).arg(netmask).arg(gateway).arg(dns1).arg(dns2).arg(phyNetworkName);
    }

    if (phyNetworkName == "eth0"){
        eth0Config = data;
    }
    else if (phyNetworkName == "eth1"){
        eth1Config = data;
    }

    QString prog = "/bin/bash";//shell
    QStringList arguments = {"-c" , QString("ifconfig %1 %2 netmask %3 down").arg(phyNetworkName).arg(ipaddr).arg(netmask)};
//    arguments << "-c" << QString("ifconfig %1 %2 netmask %3 down").arg(phyNetworkName).arg(ipaddr).arg(netmask);
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();

    arguments.clear();
    arguments << "-c" << QString("ifconfig %1 %2 netmask %3 up").arg(phyNetworkName).arg(ipaddr).arg(netmask);
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();

    QString dhcpcdConf = fileHeader + eth0Config + eth1Config;

    QByteArray dataAyyay(dhcpcdConf.toLocal8Bit());
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << dataAyyay;
    file.close();
}
void NetWorkMng::setDHCPIpAddr(QString phyNetworkName){
    QString filename = "/etc/dhcpcd.conf";
    QString data = QString("hostname\n"
                           "clientid\n"
                           "persistent\n"
                           "option rapid_commit\n"
                           "option domain_name_servers, domain_name, domain_search, host_name\n"
                           "option classless_static_routes\n"
                           "option ntp_servers\n"
                           "option interface_mtu\n"
                           "require dhcp_server_identifier\n"
                           "slaac private\n"
                           "\n");

    if (phyNetworkName == "eth0")
        eth0Config = "";
    else if (phyNetworkName == "eth1")
        eth1Config = "";

    QString dhcpcdConf = data + eth0Config + eth1Config;

    QByteArray dataAyyay(dhcpcdConf.toLocal8Bit());
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << dataAyyay;
    file.close();
}

void NetWorkMng::restartNetwork(){
    QString prog = "/bin/bash";//shell
    QStringList arguments = {"-c" , QString("systemctl daemon-reload")};
//    arguments << "-c" << QString("systemctl daemon-reload");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();

    arguments.clear();
    arguments << "-c" << QString("systemctl restart networking.service  > /dev/null 2>&1 &");
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();
    arguments.clear();
}


void NetWorkMng::setDHCPIpAddr2(QString phyNetworkName)
{
    if (!((phyNetworkName == "eth0")||(phyNetworkName == "eth1"))) return;

    QString filename = QString("/etc/network/interfaces.d/%1").arg(phyNetworkName);
    QString data = QString("auto %1\n"
                           "iface %2 inet dhcp").arg(phyNetworkName).arg(phyNetworkName);
    QByteArray dataAyyay(data.toLocal8Bit());
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << dataAyyay;
    file.close();
    restartNetwork();
}

void NetWorkMng::setDHCPIpAddr3(QString phyNetworkName)
{
    QString connectionName = "WiredConnectionEth0";
    if (phyNetworkName == "eth0")
        connectionName = "bond0-port1";
    else if (phyNetworkName == "eth1")
        connectionName = "bond0-port2";
    else if (phyNetworkName == "bond0")
        connectionName = "bond0";
    else return;

    QString prog = "/bin/bash";//shell
    QStringList arguments = {"-c" , QString("nmcli connection modify %1 ipv4.method auto ipv4.addresses '' ipv4.gateway ''").arg(connectionName)};
//    arguments << "-c" << QString("nmcli connection modify %1 ipv4.method auto ipv4.addresses '' ipv4.gateway ''").arg(connectionName);
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();

    arguments.clear();
    arguments << "-c" << QString("nmcli dev connect %1").arg(phyNetworkName);
    getAddressProcess->start(prog , arguments);
    getAddressProcess->waitForFinished();
    arguments.clear();
}

bool NetWorkMng::checkAddress(QString address)
{
    QStringList _address = address.split(".");
    if (_address.size() != 4) return false;
    if ((_address.at(0).toInt() > 255) || (_address.at(1).toInt() > 255)  || (_address.at(2).toInt() > 255) || (_address.at(3).toInt() > 255))  return false;
    return true;
}
void NetWorkMng::setStaticIpAddr2(QString ipaddr,QString netmask,QString gateway,QString dns1,QString dns2, QString phyNetworkName){
    if (!((phyNetworkName == "eth0")||(phyNetworkName == "eth1"))) return;
    QString filename = QString("/etc/network/interfaces.d/%1").arg(phyNetworkName);
    QString data;

    if ((ipaddr == "0.0.0.0") || (ipaddr == ""))  return;
    if ((netmask == "0.0.0.0") || (netmask == ""))  netmask = "255.255.255.0";
    if (gateway == "") gateway = "0.0.0.0";
    if (dns1 == "") dns1 = "0.0.0.0";
    if (dns2 == "") dns2 = "0.0.0.0";

    if (checkAddress(ipaddr) == false) return;
    if (checkAddress(netmask) == false) return;
    if (checkAddress(gateway) == false) return;
    if (checkAddress(dns1) == false) return;
    if (checkAddress(dns2) == false) return;
    if(((ipaddr!="0.0.0.0") &&  (gateway!="0.0.0.0") && (dns1!="0.0.0.0") && (dns2!="0.0.0.0")))
    {
        data = QString("auto %1\n"
                       "iface %2 inet static\n"
                       "address %3\n"
                       "netmask %4\n"
                       "gateway %5\n"
                       "dns-nameservers %6\n"
                       "dns-nameservers %7\n"
                       ).arg(phyNetworkName).arg(phyNetworkName).arg(ipaddr).arg(netmask).arg(gateway).arg(dns1).arg(dns2);
        QString commanifconfig = QString("ifconfig %1 %2").arg(phyNetworkName).arg(ipaddr);
        system(commanifconfig.toStdString().c_str());
    }
    else if(((ipaddr!="0.0.0.0") &&  (gateway!="0.0.0.0") && (dns1!="0.0.0.0") && (dns2=="0.0.0.0")))
    {
        data = QString("auto %1\n"
                       "iface %2 inet static\n"
                       "address %3\n"
                       "netmask %4\n"
                       "gateway %5\n"
                       "dns-nameservers %6\n"
                       "# dns-nameservers %7\n"
                       ).arg(phyNetworkName).arg(phyNetworkName).arg(ipaddr).arg(netmask).arg(gateway).arg(dns1).arg(dns2);
        QString commanifconfig = QString("ifconfig %1 %2").arg(phyNetworkName).arg(ipaddr);
        system(commanifconfig.toStdString().c_str());
    }
    else if(((ipaddr!="0.0.0.0") &&  (gateway!="0.0.0.0") && (dns1=="0.0.0.0") && (dns2=="0.0.0.0")))
    {
        data = QString("auto %1\n"
                       "iface %2 inet static\n"
                       "address %3\n"
                       "netmask %4\n"
                       "gateway %5\n"
                       "# dns-nameservers %6\n"
                       "# dns-nameservers %7\n"
                       ).arg(phyNetworkName).arg(phyNetworkName).arg(ipaddr).arg(netmask).arg(gateway).arg(dns1).arg(dns2);
        QString commanifconfig = QString("ifconfig %1 %2").arg(phyNetworkName).arg(ipaddr);
        system(commanifconfig.toStdString().c_str());
    }
    else if(((ipaddr!="0.0.0.0") &&  (gateway!="0.0.0.0") && (dns1=="0.0.0.0") && (dns2!="0.0.0.0")))
    {
        data = QString("auto %1\n"
                       "iface %2 inet static\n"
                       "address %3\n"
                       "netmask %4\n"
                       "gateway %5\n"
                       "# dns-nameservers %6\n"
                       "dns-nameservers %7\n"
                       ).arg(phyNetworkName).arg(phyNetworkName).arg(ipaddr).arg(netmask).arg(gateway).arg(dns1).arg(dns2);
        QString commanifconfig = QString("ifconfig %1 %2").arg(phyNetworkName).arg(ipaddr);
        system(commanifconfig.toStdString().c_str());
    }
    else if((ipaddr!="0.0.0.0") && (gateway!="0.0.0.0") && (dns1=="0.0.0.0") && (dns2=="0.0.0.0")){
        data = QString("auto %1\n"
                       "iface %2 inet static\n"
                       "address %3\n"
                       "netmask %4\n"
                       "gateway %5\n"
                       "# dns-nameservers %6\n"
                       "# dns-nameservers %7\n"
                       ).arg(phyNetworkName).arg(phyNetworkName).arg(ipaddr).arg(netmask).arg(gateway).arg(dns1).arg(dns2);
        QString commanifconfig = QString("ifconfig %1 %2").arg(phyNetworkName).arg(ipaddr);
        system(commanifconfig.toStdString().c_str());
    }
    else if((ipaddr!="0.0.0.0") && (gateway=="0.0.0.0")){
        data = QString("auto %1\n"
                       "iface %2 inet static\n"
                       "address %3\n"
                       "netmask %4\n"
                       "# gateway %5\n"
                       "# dns-nameservers %6\n"
                       "# dns-nameservers %7\n"
                       ).arg(phyNetworkName).arg(phyNetworkName).arg(ipaddr).arg(netmask).arg(gateway).arg(dns1).arg(dns2);
        QString commanifconfig = QString("ifconfig %1 %2").arg(phyNetworkName).arg(ipaddr);
        system(commanifconfig.toStdString().c_str());
    }
    QByteArray dataAyyay(data.toLocal8Bit());
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << dataAyyay;
    file.close();

    restartNetwork();
}

int NetWorkMng::calPrefix(QString mask)
{
    uint32_t longMask = 0;
    int prefix = 24;
    QStringList sListMask = mask.split(".");
    uint32_t listMask[4] = {0,0,0,0};
    for (int i=0; i<sListMask.length();i++)
    {
        listMask[i] = sListMask.at(i).toInt();
    }
    longMask = listMask[0] << 24 | listMask[1] << 16 | listMask[2] << 8 | listMask[3] << 0;
    qDebug() << "sListMask" << sListMask << listMask[0] << listMask[1] << listMask[2] << listMask[3] << longMask;
    return bit_count(longMask);
}
int NetWorkMng::bit_count(uint32_t i)
{
    QString valueInHex= QString("%1").arg(i , 0, 16);

    int c = 0;
    unsigned int seen_one = 0;

    while (i > 0) {
        if (i & 1) {
            seen_one = 1;
            c++;
        } else {
            if (seen_one) {
                return -1;
            }
        }
        i >>= 1;
    }

    qDebug() << "value = " << valueInHex << c;
    return c;
}


void NetWorkMng::setStaticIpAddr3(QString ipaddr,QString netmask,QString gateway,QString dns1,QString dns2, QString phyNetworkName){
    QString connectionName = "WiredConnectionEth0";
    if (phyNetworkName == "eth0")
        connectionName = "bond0-port1";
    else if (phyNetworkName == "eth1")
        connectionName = "bond0-port2";
    else if (phyNetworkName == "bond0")
        connectionName = "bond0";
    else return;

    int netMaskPrefix = calPrefix(netmask);

    QString data;

    if ((ipaddr == "0.0.0.0") || (ipaddr == ""))  return;
    if (gateway == "") gateway = "0.0.0.0";
    if (dns1 == "") dns1 = "0.0.0.0";
    if (dns2 == "") dns2 = "0.0.0.0";

    if (checkAddress(ipaddr) == false) return;
    if (checkAddress(netmask) == false) return;
    if (checkAddress(gateway) == false) return;
    if (checkAddress(dns1) == false) return;
    if (checkAddress(dns2) == false) return;
    if(((ipaddr!="0.0.0.0") &&  (gateway!="0.0.0.0") && (dns1!="0.0.0.0") && (dns2!="0.0.0.0")))
    {
        QString nmcliCmd = QString("nmcli connection modify %1 ipv4.method manual ipv4.addresses %2/%3 ipv4.gateway %4 ipv4.dns %5,%6").arg(connectionName).arg(ipaddr).arg(netMaskPrefix).arg(gateway).arg(dns1).arg(dns2);
        system(nmcliCmd.toStdString().c_str());

        nmcliCmd = QString("nmcli dev connect %1").arg(phyNetworkName);
        system(nmcliCmd.toStdString().c_str());
    }
    else if(((ipaddr!="0.0.0.0") &&  (gateway!="0.0.0.0") && (dns1!="0.0.0.0") && (dns2=="0.0.0.0")))
    {
        QString nmcliCmd = QString("nmcli connection modify %1 ipv4.method manual ipv4.addresses %2/%3 ipv4.gateway %4 ipv4.dns %5").arg(connectionName).arg(ipaddr).arg(netMaskPrefix).arg(gateway).arg(dns1);
        system(nmcliCmd.toStdString().c_str());

        nmcliCmd = QString("nmcli dev connect %1").arg(phyNetworkName);
        system(nmcliCmd.toStdString().c_str());
    }
    else if(((ipaddr!="0.0.0.0") &&  (gateway!="0.0.0.0") && (dns1=="0.0.0.0") && (dns2!="0.0.0.0")))
    {
        QString nmcliCmd = QString("nmcli connection modify %1 ipv4.method manual ipv4.addresses %2/%3 ipv4.gateway %4 ipv4.dns %5").arg(connectionName).arg(ipaddr).arg(netMaskPrefix).arg(gateway).arg(dns2);
        system(nmcliCmd.toStdString().c_str());

        nmcliCmd = QString("nmcli dev connect %1").arg(phyNetworkName);
        system(nmcliCmd.toStdString().c_str());
    }
    else if((ipaddr!="0.0.0.0") && (gateway!="0.0.0.0") && (dns1=="0.0.0.0") && (dns2=="0.0.0.0")){
        QString nmcliCmd = QString("nmcli connection modify %1 ipv4.method manual ipv4.addresses %2/%3 ipv4.gateway %4").arg(connectionName).arg(ipaddr).arg(netMaskPrefix).arg(gateway);
        system(nmcliCmd.toStdString().c_str());

        nmcliCmd = QString("nmcli dev connect %1").arg(phyNetworkName);
        system(nmcliCmd.toStdString().c_str());
    }
    else if((ipaddr!="0.0.0.0") && (gateway=="0.0.0.0"))
    {
        QString nmcliCmd = QString("nmcli connection modify %1 ipv4.method manual ipv4.addresses %2/%3").arg(connectionName).arg(ipaddr).arg(netMaskPrefix);
        system(nmcliCmd.toStdString().c_str());

        nmcliCmd = QString("nmcli dev connect %1").arg(phyNetworkName);
        system(nmcliCmd.toStdString().c_str());
    }
    else if(ipaddr!="0.0.0.0")
    {
        QString nmcliCmd = QString("nmcli connection modify %1 ipv4.method manual ipv4.addresses %2/24").arg(connectionName).arg(ipaddr);
        system(nmcliCmd.toStdString().c_str());

        nmcliCmd = QString("nmcli dev connect %1").arg(phyNetworkName);
        system(nmcliCmd.toStdString().c_str());
    }
}

 float NetWorkMng::getCPUTemp()
{
    string val;
    string getval_str = "/sys/class/thermal/thermal_zone0/temp";
    ifstream getval(getval_str.c_str());
    if (getval.fail()){
        cout << " OPERATION FAILED: Unable to getCPUTemp."<< endl;
        return 0.0;
            }

    getval >> val ;  //read gpio value
    getval.close(); //close the value file
    return float((QString::fromStdString(val).toFloat())/1000.0);
}
