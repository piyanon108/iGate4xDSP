#include "iGate4x.h"
using namespace std;
iGate4x::iGate4x(QObject *parent) : QObject(parent)
{    
    myConfigurate();

    dataLoggerServer = new ChatClient(8088,"127.0.0.1","",1,enableDataLogger);
    dataLoggerServer->setDevice("Server","localhost");
    if (enableDataLogger)
        connect(dataLoggerServer,SIGNAL(connected(int)),this,SLOT(initServer(int)));

    else
    {
        initServer(1);
    }
}

void iGate4x::initServer(int index)
{
    if (serverInit) return;
    serverInit = index;
//    system("modprobe smsc75xx");
    system("modprobe lan743x");
//    system("truncate -s 0 /var/log/syslog");
    system("echo LC_TIME=\"POSIX\" > /etc/default/locale");
    system("sudo update-locale LC_TIME=POSIX");
//    system("hwclock --hctosys");
//    system("systemctl restart hwclock.service");
//    system("systemctl restart systemd-timesyncd.service");
    system("systemctl stop iGateGroupCH1");
    system("systemctl stop iGateGroupCH2");
    system("systemctl stop iGateGroupCH3");
    system("systemctl stop iGateGroupCH4");
    system("systemctl stop iGatePlusCH1");
    system("systemctl stop iGatePlusCH2");
    system("systemctl stop iGatePlusCH3");
    system("systemctl stop iGatePlusCH4");
    system("systemctl stop nvfancontrol");
    system("speaker-test -Dhw:APE,1 -r8000 -c8 -S0 --nloops 3 -s 1 -tsine -f1000");

    SigmaFirmWareDownLoad = new ADAU1467("/dev/spidev2.0");
    ppt_sql_output1 = new GPIOClass(PTT_SQL_OUTPUT1);
    ppt_sql_output2 = new GPIOClass(PTT_SQL_OUTPUT2);
    ppt_sql_output3 = new GPIOClass(PTT_SQL_OUTPUT3);
    ppt_sql_output4 = new GPIOClass(PTT_SQL_OUTPUT4);
    ppt_sql_input1 = new GPIOClass(PTT_SQL_INPUT1);
    ppt_sql_input2 = new GPIOClass(PTT_SQL_INPUT2);
    ppt_sql_input3 = new GPIOClass(PTT_SQL_INPUT3);
    ppt_sql_input4 = new GPIOClass(PTT_SQL_INPUT4);

    led_rx1 = new GPIOClass(LED_RX1);
    led_rx2 = new GPIOClass(LED_RX2);
    led_rx3 = new GPIOClass(LED_RX3);
    led_rx4 = new GPIOClass(LED_RX4);
    led_tx1 = new GPIOClass(LED_TX1);
    led_tx2 = new GPIOClass(LED_TX2);
    led_tx3 = new GPIOClass(LED_TX3);
    led_tx4 = new GPIOClass(LED_TX4);
    codecReset = new GPIOClass(GPIO_CODEC_RESET);
    lcdReset = new GPIOClass(GPIO_LCD_RST);
    hubusbReset = new GPIOClass(GPIO_USB_RESET);
    dspReset = new GPIOClass(GPIO_DSP_RESET);
    dspBootSelect = new GPIOClass(GPIO_DSP_SBOOT);
    sleep(1);
    codecReset->setdir_gpio("out");
    lcdReset->setdir_gpio("out");
    hubusbReset->setdir_gpio("out");
    dspReset->setdir_gpio("out");
    dspBootSelect->setdir_gpio("out");
    ppt_sql_output1->setdir_gpio("out");
    ppt_sql_output2->setdir_gpio("out");
    ppt_sql_output3->setdir_gpio("out");
    ppt_sql_output4->setdir_gpio("out");
    ppt_sql_input1->setdir_gpio("in");
    ppt_sql_input2->setdir_gpio("in");
    ppt_sql_input3->setdir_gpio("in");
    ppt_sql_input4->setdir_gpio("in");
    ppt_sql_input1->set_edge("both");
    ppt_sql_input2->set_edge("both");
    ppt_sql_input3->set_edge("both");
    ppt_sql_input4->set_edge("both");

    led_rx1->setdir_gpio("out");
    led_rx2->setdir_gpio("out");
    led_rx3->setdir_gpio("out");
    led_rx4->setdir_gpio("out");
    led_tx1->setdir_gpio("out");
    led_tx2->setdir_gpio("out");
    led_tx3->setdir_gpio("out");
    led_tx4->setdir_gpio("out");

//    DSPBootSelect(JETSONSPI);
    DSPBootSelect(QSPIFLASH);
    codecReset->resetGpio();
    dspReset->resetGpio();
    QThread::msleep(100);
    codecReset->setGpio();
    hubusbReset->setGpio();
    lcdReset->setGpio();
    dspReset->setGpio();


    QThread::msleep(1000);
    SigmaFirmWareDownLoad->default_download_IC_1();
    SigmaFirmWareDownLoad->setToneVolume(TONE_CH1_ADDRESS,0);
    SigmaFirmWareDownLoad->setToneVolume(TONE_CH2_ADDRESS,0);
    SigmaFirmWareDownLoad->setToneVolume(TONE_CH3_ADDRESS,0);
    SigmaFirmWareDownLoad->setToneVolume(TONE_CH4_ADDRESS,0);

    SigmaFirmWareDownLoad->setToneVolume(TONE_SERVER_CH1_ADDRESS,0);
    SigmaFirmWareDownLoad->setToneVolume(TONE_SERVER_CH2_ADDRESS,0);
    SigmaFirmWareDownLoad->setToneVolume(TONE_SERVER_CH3_ADDRESS,0);
    SigmaFirmWareDownLoad->setToneVolume(TONE_SERVER_CH4_ADDRESS,0);
//    SigmaFirmWareDownLoad->setFIRfilter(MOD_FIR1_ALG0_FIRSIGMA300ALG1FIRCOEFF0_ADDR, MOD_FIR1_COUNT,FIRfilter_stateON_INPUT);

    QThread::msleep(500);
    m_IsEventThreadRunning2 = true;
    m_EventThreadInstance2 = new boost::shared_ptr<boost::thread>;
    m_EventThreadInstance2->reset(new boost::thread(boost::bind(&iGate4x::displayThread, this)));
//    SigmaFirmWareDownLoad->WriteSelfBootMemoryToDSP();
//    usleep(100000);
//    codecReset->resetGpio();
//    dspReset->resetGpio();
//    usleep(1000000);
//    codecReset->setGpio();
//    dspReset->setGpio();
    SocketServer = new ChatServer(1234);

    CODEC_CS42448 = new CS42448(CODEC1I2CDEV,CODECI2CADDR_CS42448);
    CODEC_PCM3168A = new PCM3168A(CODEC1I2CDEV,CODECI2CADDR_PCM3168A);
    networking = new NetWorkMng(this);

    qDebug() << "CODEC_CS42448" << CODEC_CS42448->active;
    qDebug() << "CODEC_PCM3168A" << CODEC_PCM3168A->active;

    dataLoggerServer->createDataLog(INFO,QString("CODEC_CS42448 Init: %1").arg(CODEC_CS42448->active));
    dataLoggerServer->createDataLog(INFO,QString("CODEC_PCM3168A Init: %1").arg(CODEC_PCM3168A->active));

    if ((CODEC_CS42448->active == false) & (CODEC_PCM3168A->active == false)){
        dataLoggerServer->createDataLog(ERROR,QString("CODEC Init: Error"));
        QThread::sleep(60);
        exit(1);
    }
//    SigmaFirmWareDownLoad->default_download_IC_1();
    if (CODEC_CS42448->active)
    {
        bool codec_init = true;
        codec_init &= CODEC_CS42448->Reg03h_FunctionalMode(0x03,0x03,0x03);
        codec_init &= CODEC_CS42448->Reg11_16hAINVolumeCtrl(VOIPCH1_I2S1,VoIPVolumeInCH1);
        codec_init &= CODEC_CS42448->Reg11_16hAINVolumeCtrl(VOIPCH2_I2S1,VoIPVolumeInCH2);
        codec_init &= CODEC_CS42448->Reg11_16hAINVolumeCtrl(VOIPCH3_I2S1,VoIPVolumeInCH3);
        codec_init &= CODEC_CS42448->Reg11_16hAINVolumeCtrl(VOIPCH4_I2S1,VoIPVolumeInCH4);
        codec_init &= CODEC_CS42448->Reg11_16hAINVolumeCtrl(5,0x80);
        codec_init &= CODEC_CS42448->Reg11_16hAINVolumeCtrl(6,0x80);
        codec_init &= CODEC_CS42448->Reg11_16hAINVolumeCtrl(7,0x80);
        codec_init &= CODEC_CS42448->Reg08_0FhAOUTXVolumeCtrl(VOIPCH1_I2S1,VoIPVolumeOutCH1);
        codec_init &= CODEC_CS42448->Reg08_0FhAOUTXVolumeCtrl(VOIPCH2_I2S1,VoIPVolumeOutCH2);
        codec_init &= CODEC_CS42448->Reg08_0FhAOUTXVolumeCtrl(VOIPCH3_I2S1,VoIPVolumeOutCH3);
        codec_init &= CODEC_CS42448->Reg08_0FhAOUTXVolumeCtrl(VOIPCH4_I2S1,VoIPVolumeOutCH4);
        codec_init &= CODEC_CS42448->Reg08_0FhAOUTXVolumeCtrl(5,VoIPVolumeOutCH1);
        codec_init &= CODEC_CS42448->Reg08_0FhAOUTXVolumeCtrl(6,VoIPVolumeOutCH1);
        codec_init &= CODEC_CS42448->Reg08_0FhAOUTXVolumeCtrl(7,VoIPVolumeOutCH1);
        codec_init &= CODEC_CS42448->Reg08_0FhAOUTXVolumeCtrl(8,VoIPVolumeOutCH1);

        dataLoggerServer->createDataLog(codec_init,QString("CODEC_CS42448 Config: %1").arg(CODEC_CS42448->active));
    }


//    system("amixer -c APE sset 'ADMAIF1 Capture Audio Channels' 8");
//    system("amixer -c APE sset 'ADMAIF2 Capture Audio Channels' 8");
//    system("amixer -c APE sset 'I2S2 Channels' 8");
//    system("amixer -c APE sset 'I2S4 Channels' 8");
//    system("amixer -c APE cset name='I2S3 input bit format' 2");
//    system("amixer -c APE cset name='I2S3 codec bit format' 2");
//    system("amixer -c APE cset name='I2S4 input bit format' 2");
//    system("amixer -c APE cset name='I2S4 codec bit format' 2");
//    system("amixer -c APE sset 'I2S2 codec frame mode' dsp-a");
//    system("amixer -c APE sset 'I2S4 codec frame mode' dsp-a");
//    system("amixer -c APE sset 'I2S2 fsync width' 0");
//    system("amixer -c APE sset 'I2S4 fsync width' 0");

//    system("speaker-test -r8000 -c8 -tsine -f1000 -Dhw:1,1 -l1");

    system("systemctl stop aloop.service");
    loopTimer = new QTimer();
    loopTimerGetAudioLevel = new QTimer();
    loopTruncatesyslog = new QTimer();
//    timerCout = new QTimer();
//    connect(timerCout, &QTimer::timeout, this, &iGate4x::processTimeout);
    connect(loopTimer,SIGNAL(timeout()),this,SLOT(loopGetInfo()));
    connect(loopTruncatesyslog,SIGNAL(timeout()),this,SLOT(Truncatesyslog()));
    connect(loopTimerGetAudioLevel,SIGNAL(timeout()),this,SLOT(loopGetAudioLevel()));
    connect(this,SIGNAL(onPTT_Sql_Changed(uint8_t, bool)),this,SLOT(setLED_txrx_value(uint8_t, bool)));
//    loopTruncatesyslog->start(300000);
    loopTimer->start(200);
//    timerCout->start(1000);

    myDatabase = new Database("iGatePlus","userData","Ifz8zean6868**","127.0.0.1",this);
    myDatabase->roleIDSelect = -1;
    bool DatabaseConnected = myDatabase->database_createConnection();
    dataLoggerServer->createDataLog(DatabaseConnected,QString("Database Connected: %1").arg(DatabaseConnected));
    if (myDatabase->checkContollerTable() != 4)
    {
        myDatabase->fixDatabase();
        printf("*******myDatabase->fixDatabase()*******");
    }
    else {
        printf("*******myDatabase->Ok*******");
    }
    myDatabase->addColumnRxBestSignalEnable();
    myDatabase->addColumnOutputDSPLevel();
    myDatabase->addColumnTone();
    myDatabase->addColumnToneServer();
    myDatabase->ALTERTABLEcontrolerUri64Char();

    connect(myDatabase,SIGNAL(onNewControler(int , int , int , QString , int , int , uint8_t , bool , bool , uint8_t ,quint8  , quint8  , float ,float , bool , bool , QString , int , int, bool, int, QString, bool, uint8_t, uint8_t, uint8_t, bool, float, int,float))
                   ,this,SLOT(onNewControler(int , int , int , QString , int , int , uint8_t , bool , bool , uint8_t ,quint8  , quint8  , float ,float , bool , bool , QString , int , int, bool, int, QString, bool, uint8_t, uint8_t, uint8_t, bool, float, int,float)));

    connect(myDatabase,SIGNAL(newRediodata(uint8_t, bool, bool, int, float, int, float)),
            this,SLOT(newRediodata(uint8_t, bool, bool, int, float, int, float)));

    connect(myDatabase,SIGNAL(newradio(int ,QString , QString , QString , QString , int ,int, QString )),
            this,SLOT(newradio(int ,QString , QString , QString , QString , int, int , QString )));

    connect(myDatabase,SIGNAL(newCallChannel(int, int, int, QString, QString, int, int, int, QString, int, int, QString, QString, int, bool , bool, bool , bool)),
            this,SLOT(newCallChannel(int, int, int, QString, QString, int, int, int, QString, int, int, QString, QString, int, bool , bool, bool , bool)));

    connect(myDatabase,SIGNAL(newIGateGroupMng(int , int , int , QString , QString )),
                      this,SLOT(newIGateGroupMng(int , int , int , QString , QString )));

    connect(this,SIGNAL(onDCInChanged()),
                      this,SLOT(dcInChanged()));

    connect(SocketServer,SIGNAL(adcValue(int,double, QString)),
                      this,SLOT(adcValue(int,double, QString)));

    connect(SocketServer,SIGNAL(getCtrlInfoPage(int)),
                      this,SLOT(getCtrlInfoPage(int)));

    connect(SocketServer,SIGNAL(newSoftphone(int, QWebSocket*)),
                      this,SLOT(startSoftphone(int, QWebSocket*)));

    connect(SocketServer,SIGNAL(removeSoftphone( QWebSocket*)),
                      this,SLOT(removeSoftphone( QWebSocket*)));

    connect(SocketServer,SIGNAL(addNewRadio(QString , QString , QString , QString , int ,int , QString )),
                      this,SLOT(addNewRadio(QString , QString , QString , QString , int ,int , QString )));
    connect(SocketServer,SIGNAL(updateRadio(QString , QString , QString , QString , int ,int , QString ,int )),
                      this,SLOT(updateRadio(QString , QString , QString , QString , int ,int , QString ,int )));
    connect(SocketServer,SIGNAL(removeRadio(int )),
                      this,SLOT(removeRadio(int )));

    connect(SocketServer,SIGNAL(addNewChannel(QString , QString , int , int , bool , QString , int , int , QString , QString )),
                      this,SLOT(addNewChannel(QString , QString , int , int , bool , QString , int , int , QString , QString )));
    connect(SocketServer,SIGNAL(updateChannel(QString , QString , int , int , bool , QString , int , int , QString , QString , int)),
                      this,SLOT(updateChannel(QString , QString , int , int , bool , QString , int , int , QString , QString , int)));
    connect(SocketServer,SIGNAL(removeChannel(int )),
                      this,SLOT(removeChannel(int )));

    connect(SocketServer,SIGNAL(updateControler(QString , int , int , int , int , int )),
                      this,SLOT(updateControler(QString , int , int , int , int , int )));
    connect(SocketServer,SIGNAL(updateChannelID(int , int )),
                      this,SLOT(updateChannelID(int , int )));

    connect(SocketServer,SIGNAL(setRadioController(int , int )),
                      this,SLOT(setRadioController(int , int )));

    connect(SocketServer,SIGNAL(updateRadioAutoInactive(bool , int )),
                      this,SLOT(updateRadioAutoInactive(bool , int )));

    connect(SocketServer,SIGNAL(updateMainRadioTransmitterUsed(bool , int )),
                      this,SLOT(updateMainRadioTransmitterUsed(bool , int )));

    connect(SocketServer,SIGNAL(updateRadioMainStandby(int , int )),
                      this,SLOT(updateRadioMainStandby(int , int )));

    connect(SocketServer,SIGNAL(updateMainRadioReceiverUsed(bool , int )),
                      this,SLOT(updateMainRadioReceiverUsed(bool , int )));


    connect(SocketServer,SIGNAL(updateRadioActive(bool ,bool ,bool , int , int )),
                      this,SLOT(updateRadioActive(bool ,bool ,bool , int , int )));

    connect(SocketServer,SIGNAL(appendChannelData(uint8_t , int , bool , bool , int , int , QString , QString , QString , QString , QString)),
                      this,SLOT(appendChannelData(uint8_t , int , bool , bool , int , int , QString , QString , QString , QString , QString)));

    connect(SocketServer,SIGNAL(appendClientConnStatus(int , QString , QString , QString , bool , bool , int )),
                      this,SLOT(appendClientConnStatus(int , QString , QString , QString , bool , bool , int )));

    connect(SocketServer,SIGNAL(appendClientCallInStatus(int , QString , QString , bool , bool , bool )),
                      this,SLOT(appendClientCallInStatus(int , QString , QString , bool , bool , bool )));

    connect(SocketServer,SIGNAL(getVuMeter()),
                      this,SLOT(getVuMeter()));

    connect(SocketServer,SIGNAL(getHomePage(uint8_t, QWebSocket*)),
                      this,SLOT(getHomePage(uint8_t, QWebSocket*)));

    connect(SocketServer,SIGNAL(getNetworkPage(QWebSocket*)),
                      this,SLOT(getNetworkPage(QWebSocket*)));

    connect(SocketServer,SIGNAL(getServerHomePage(uint8_t, QWebSocket*)),
                      this,SLOT(getServerHomePage(uint8_t, QWebSocket*)));

    connect(SocketServer,SIGNAL(getIGateGroupConfig(uint8_t, QWebSocket*)),
                      this,SLOT(getIGateGroupConfig(uint8_t, QWebSocket*)));

    connect(SocketServer,SIGNAL(updateInputGain(uint8_t , uint8_t )),
                      this,SLOT(updateInputGain(uint8_t , uint8_t )));
    connect(SocketServer,SIGNAL(updateOutputGain(uint8_t , uint8_t )),
                      this,SLOT(updateOutputGain(uint8_t , uint8_t )));
    connect(SocketServer,SIGNAL(updateDSPOutputGain(uint8_t , uint8_t )),
                      this,SLOT(updateDSPOutputGain(uint8_t , uint8_t )));
    connect(SocketServer,SIGNAL(updateURILits(QStringList , int )),
                      this,SLOT(updateURILits(QStringList , int )));
    connect(SocketServer,SIGNAL(updateRxBestSignalEnable(bool , int )),
                      this,SLOT(updateRxBestSignalEnable(bool , int )));
    connect(SocketServer,SIGNAL(updateNumconn(int , int )),
                      this,SLOT(updateNumconn(int , int )));
    connect(SocketServer,SIGNAL(updateSQLDefeat(bool , int )),
                      this,SLOT(updateSQLDefeat(bool , int )));
    connect(SocketServer,SIGNAL(updateSQLActiveHigh(bool , int )),
                      this,SLOT(updateSQLActiveHigh(bool , int )));
    connect(SocketServer,SIGNAL(updateHostConfig(QString , int , int , QString , int )),
                      this,SLOT(updateHostConfig(QString , int , int , QString , int )));
    connect(SocketServer,SIGNAL(updateDeviceName(int , QString )),
                      this,SLOT(updateDeviceName(int , QString )));

    connect(SocketServer,SIGNAL(updateInputTone(bool , float , int , float, uint8_t)),
                      this,SLOT(updateInputTone(bool, float, int , float, uint8_t)));

    connect(SocketServer,SIGNAL(updateServerTone(bool, bool,int, float, int, float, uint8_t)),
                      this,SLOT(updateServerTone(bool, bool,int, float, int, float, uint8_t)));

    connect(SocketServer, SIGNAL(updateFIRfilter(bool, bool)),
                       this,SLOT(updateFIRfilter(bool, bool)));

    connect(SocketServer,SIGNAL(getTestRadioPage(QWebSocket*)),
                      this,SLOT(getTestRadioPage(QWebSocket*)));

    connect(SocketServer,SIGNAL(getRedioTestHomePage(uint8_t, QWebSocket*)),
                      this,SLOT(getRedioTestHomePage(uint8_t, QWebSocket*)));
//    connect(SocketServer,SIGNAL(toggleRxEnable(int )),
//                      this,SLOT(toggleRxEnable(int )));
//    connect(SocketServer,SIGNAL(toggleTxEnable(int )),
//                      this,SLOT(toggleTxEnable(int )));

    connect(SocketServer,SIGNAL(restartSoftphone(int )),
                      this,SLOT(restartSoftphone(int )));

    connect(SocketServer,SIGNAL(updateSiteTone(float, int )),
                      this,SLOT(updateSidetone(float, int )));
    connect(SocketServer,SIGNAL(updateGroupMute(uint8_t, int )),
                      this,SLOT(updateGroupMute(uint8_t, int )));
    connect(SocketServer,SIGNAL(updatePttDelay(uint8_t, int )),
                      this,SLOT(updatePttDelay(uint8_t, int )));
    connect(SocketServer,SIGNAL(updateLocalSiteTone(float, int )),
                      this,SLOT(updateLocalSidetone(float, int )));

    connect(SocketServer,SIGNAL(onLocalSidetoneLoopbackChanged(bool, int )),
                      this,SLOT(onLocalSidetoneLoopbackChanged(bool, int )));

    connect(SocketServer,SIGNAL(getSystemPage(QWebSocket* )),
                      this,SLOT(getSystemPage(QWebSocket* )));

    connect(SocketServer,SIGNAL(rebootSystem()),
                      this,SLOT(rebootSystem()));

    connect(SocketServer,SIGNAL(updateNTPServer(QString)),this,SLOT(updateNTPServer(QString)));
    connect(SocketServer,SIGNAL(setcurrentDatetime(QString)),this,SLOT(setcurrentDatetime(QString)));
    connect(SocketServer,SIGNAL(setLocation(QString)),this,SLOT(setLocation(QString)));

    connect(SocketServer,SIGNAL(updateNetworkTimeout(int)),this,SLOT(updateNetworkTimeout(int)));


    connect(SocketServer,SIGNAL(updateNetwork(uint8_t,QString,QString,QString,QString,QString,QString)),
            this,SLOT(slotupdateNetwork(uint8_t,QString,QString,QString,QString,QString,QString)));

    connect(SocketServer,SIGNAL(updateswitchInviteMode(int)),this,SLOT(updateswitchInviteMode(int)));

    connect(SocketServer,SIGNAL(updateFirmware()),this,SLOT(updateFirmware()));
    connect(SocketServer,SIGNAL(systembackup()),this,SLOT(systembackup()));
    connect(SocketServer,SIGNAL(systemrestore()),this,SLOT(systemrestore()));
    connect(SocketServer,SIGNAL(addIGateGroupMng(int ,int ,QString ,QString )),this,SLOT(addIGateGroupMng(int ,int ,QString ,QString )));
    connect(SocketServer,SIGNAL(syncIGateGroup(QString ,int ,int ,QString ,QString )),this,SLOT(syncIGateGroup(QString ,int ,int ,QString ,QString )));
    connect(SocketServer,SIGNAL(deleteIGateGroup(QString ,int)),this,SLOT(deleteIGateGroup(QString ,int)));
    connect(SocketServer,SIGNAL(deleteIGate(int , int , int , QString )),this,SLOT(deleteIGate(int , int , int , QString )));
    connect(SocketServer,SIGNAL(syncDeleteIGateAnotherGroup(int , int , QString , QString )),this,SLOT(syncDeleteIGateAnotherGroup(int , int , QString , QString )));

    updateNetwork(eth0.dhcpmethod == "on",eth0.ipaddress, eth0.subnet, eth0.gateway, eth0.pridns, eth0.secdns, "bond0");

    myDatabase->getNewControlerData();
    myDatabase->getCallChannel(1);
    myDatabase->getCallChannel(2);
    myDatabase->getCallChannel(3);
    myDatabase->getCallChannel(4);
    myDatabase->getRadioData();
    myDatabase->getIGateGroupMng();
    myDatabase ->getNewRadioTestData();

    myConfigurate();
    m_IsEventThreadRunning = false;
    m_EventThreadInstance = new boost::shared_ptr<boost::thread>;
    m_EventThreadInstance->reset(new boost::thread(boost::bind(&iGate4x::i2cGpioExtReadWriteThread, this)));

    if(ntp)
    {
        system("timedatectl set-ntp 1");
        system("hwclock -w > /dev/null 2>&1 &");
    }
    else
    {
        system("timedatectl set-ntp 0");
    }
    qDebug() << "inviteMode" << inviteMode;
    if (inviteMode == SERVER)
    {
        system("tar -xf /opt/iGate4x/servermode.tar -C /");
        system("chown -R orinnx:www-data /var/www/html/*.php");

        RadioControllerCH1 = 0;
        RadioControllerCH2 = 0;
        RadioControllerCH3 = 0;
        RadioControllerCH4 = 0;
    }
    else if (inviteMode == CLIENT)
    {
        system("tar -xf /opt/iGate4x/clientmode.tar -C /");
        system("chown -R orinnx:www-data /var/www/html/*.php");
    }
    system("systemctl restart adc_mcp3208.service");

    dataLoggerServer->createDataLog(INFO,QString("Start iGate Mode: %1").arg(inviteMode));


    SigmaFirmWareDownLoad->setMixerVolume(AUDIOIN_VOLUME_CH1_ADDRESS,SIDETONE_VOLUME_CH1_MODE_ADDRESS,SIDETONE_VOLUME_CH1_MODE_VALUE,1);
    SigmaFirmWareDownLoad->setMixerVolume(AUDIOIN_VOLUME_CH2_ADDRESS,SIDETONE_VOLUME_CH2_MODE_ADDRESS,SIDETONE_VOLUME_CH2_MODE_VALUE,1);
    SigmaFirmWareDownLoad->setMixerVolume(AUDIOIN_VOLUME_CH3_ADDRESS,SIDETONE_VOLUME_CH3_MODE_ADDRESS,SIDETONE_VOLUME_CH3_MODE_VALUE,1);
    SigmaFirmWareDownLoad->setMixerVolume(AUDIOIN_VOLUME_CH4_ADDRESS,SIDETONE_VOLUME_CH4_MODE_ADDRESS,SIDETONE_VOLUME_CH4_MODE_VALUE,1);
    SigmaFirmWareDownLoad->setFIRfilter(MOD_FIR1_ALG0_FIRSIGMA300ALG1FIRCOEFF0_ADDR,MOD_FIR1_COUNT,FIRfilter_stateOFF_INPUT);
    SigmaFirmWareDownLoad->setFIRfilter(MOD_FIR2_ALG0_FIRSIGMA300ALG5FIRCOEFF0_ADDR, MOD_FIR2_COUNT,FIRfilter_stateOFF_OUTPUT);

    setRadioController(RadioControllerCH1,1);
    setRadioController(RadioControllerCH2,2);
    setRadioController(RadioControllerCH3,3);
    setRadioController(RadioControllerCH4,4);

    loopTimerGetAudioLevel->start(200);

}
void iGate4x::loopGetAudioLevel()
{    
    level_in1 = getAudioLevel(MOD_LEVEL1_ALG0_SINGLEBANDLEVELDETS3001X_ADDR,MOD_LEVEL1_ALG0_SINGLEBANDLEVELDETS3001XLO_ADDR);
    level_in2 = getAudioLevel(MOD_LEVEL2_ALG0_SINGLEBANDLEVELDETS3002X_ADDR,MOD_LEVEL2_ALG0_SINGLEBANDLEVELDETS3002XLO_ADDR);
    level_in3 = getAudioLevel(MOD_LEVEL1_2_ALG0_SINGLEBANDLEVELDETS3003X_ADDR,MOD_LEVEL1_2_ALG0_SINGLEBANDLEVELDETS3003XLO_ADDR);
    level_in4 = getAudioLevel(MOD_LEVEL1_3_ALG0_SINGLEBANDLEVELDETS3004X_ADDR,MOD_LEVEL1_3_ALG0_SINGLEBANDLEVELDETS3004XLO_ADDR);
    level_out1 = getAudioLevel(MOD_LEVEL1_4_ALG0_SINGLEBANDLEVELDETS3006X_ADDR,MOD_LEVEL1_4_ALG0_SINGLEBANDLEVELDETS3006XLO_ADDR);
    level_out2 = getAudioLevel(MOD_LEVEL1_5_ALG0_SINGLEBANDLEVELDETS3005X_ADDR,MOD_LEVEL1_5_ALG0_SINGLEBANDLEVELDETS3005XLO_ADDR);
    level_out3 = getAudioLevel(MOD_LEVEL1_6_ALG0_SINGLEBANDLEVELDETS3007X_ADDR,MOD_LEVEL1_6_ALG0_SINGLEBANDLEVELDETS3007XLO_ADDR);
    level_out4 = getAudioLevel(MOD_LEVEL1_7_ALG0_SINGLEBANDLEVELDETS3008X_ADDR,MOD_LEVEL1_7_ALG0_SINGLEBANDLEVELDETS3008XLO_ADDR);
    log_level_in1 = 10*log10(level_in1);
    log_level_in2 = 10*log10(level_in2);
    log_level_in3 = 10*log10(level_in3);
    log_level_in4 = 10*log10(level_in4);
    log_level_out1 = 10*log10(level_out1);
    log_level_out2 = 10*log10(level_out2);
    log_level_out3 = 10*log10(level_out3);
    log_level_out4 = 10*log10(level_out4);

    SocketServer->broadcastVUMeter(level_in1,level_in2,level_in3,level_in4,
                                   level_out1,level_out2,level_out3,level_out4,
                                   log_level_in1,log_level_in2,log_level_in3,log_level_in4,
                                   log_level_out1,log_level_out2,log_level_out3,log_level_out4);
    if (SocketServer->m_WebSocketVUClients.size() == 0)
    {
//        qDebug() << "loopTimerGetAudioLevel->stop()";
        loopTimerGetAudioLevel->start(200);
    }
}


void iGate4x::getVuMeter()
{
    loopTimerGetAudioLevel->start(100);
}

double iGate4x::getAudioLevel(uint16_t x_address, uint16_t xlo_address)
{
    double level = 0.0000000002;
    uint32_t level_high_word, level_low_word; int64_t level_64bit;
    spi2Busy = true;
    level_high_word = SigmaFirmWareDownLoad->SIGMA_READ_REGISTER_INTEGER(x_address,4);
    level_low_word= SigmaFirmWareDownLoad->SIGMA_READ_REGISTER_INTEGER(xlo_address,4);
    spi2Busy = false;
    level_64bit = ((int64_t)level_high_word<<32) + (level_low_word); /*56bit */
    if (level_64bit == 0) level_64bit = 1;
    level= (double) level_64bit/0x00FFFFFFFFFFFFFF;
    if (level < 0.0000000002) level = 0.0000000002;
    return level;
}
void iGate4x::onLocalSidetoneLoopbackChanged(bool localSidetoneLoopbackOn, int softPhoneID)
{
    double sidetone = 0;
    Q_FOREACH(SoftPhone *softPhone ,m_softPhone_list)
    {
        if (softPhone->softPhoneID == softPhoneID){
            sidetone = softPhone->localSidetone;
            softPhone->localSidetoneOn = localSidetoneLoopbackOn;
            break;
        }
    }

    if (localSidetoneLoopbackOn == false) sidetone = 0;

    switch (softPhoneID)
    {
    case 1:
        SigmaFirmWareDownLoad->setMixerVolume(SIDETONE_VOLUME_CH1_ADDRESS,SIDETONE_VOLUME_CH1_MODE_ADDRESS,SIDETONE_VOLUME_CH1_MODE_VALUE,sidetone);
        break;
    case 2:
        SigmaFirmWareDownLoad->setMixerVolume(SIDETONE_VOLUME_CH2_ADDRESS,SIDETONE_VOLUME_CH2_MODE_ADDRESS,SIDETONE_VOLUME_CH2_MODE_VALUE,sidetone);
        break;
    case 3:
        SigmaFirmWareDownLoad->setMixerVolume(SIDETONE_VOLUME_CH3_ADDRESS,SIDETONE_VOLUME_CH3_MODE_ADDRESS,SIDETONE_VOLUME_CH3_MODE_VALUE,sidetone);
        break;
    case 4:
        SigmaFirmWareDownLoad->setMixerVolume(SIDETONE_VOLUME_CH4_ADDRESS,SIDETONE_VOLUME_CH4_MODE_ADDRESS,SIDETONE_VOLUME_CH4_MODE_VALUE,sidetone);
        break;
    }
}
int iGate4x::softPhoneIDCheckCHInfo(int softPhoneID)
{
    int i = 0;
    Q_FOREACH (ChannelInfo *softPhone_list, m_ChannelInfo_list){
        if (softPhone_list->softPhoneID == softPhoneID){
            return i;
        }
        i++;
    }
    qDebug() << "append(new ChannelInfo)";
    m_ChannelInfo_list.append(new ChannelInfo); //Add
    return i;
}
void iGate4x::set_Tx_LED(int softPhoneID,bool mainTxPTTOn)
{
    bool led = LEDOFF;
    if (mainTxPTTOn) led = LEDON;
    switch (softPhoneID) {
        case 1:
            led_tx1->setval_gpio(led);
        break;
        case 2:
            led_tx2->setval_gpio(led);
        break;
        case 3:
            led_tx3->setval_gpio(led);
        break;
        case 4:
            led_tx4->setval_gpio(led);
        break;
    }
}
void iGate4x::set_Rx_LED(int softPhoneID,bool mainRxPTTOn)
{
    bool led = LEDOFF;
    if (mainRxPTTOn) led = LEDON;
    switch (softPhoneID) {
        case 1:
            led_rx1->setval_gpio(led);
        break;
        case 2:
            led_rx2->setval_gpio(led);
        break;
        case 3:
            led_rx3->setval_gpio(led);
        break;
        case 4:
            led_rx4->setval_gpio(led);
        break;
    }
}

int iGate4x::checkCallInList(int ChannelInfoListIndex, QString uri)
{
    int i = 0;
    Q_FOREACH (trx_callin *callIn, m_ChannelInfo_list.at(ChannelInfoListIndex)->list_trxCallIn)
    {
        if (callIn->uri == uri)
        {
            return i;
        }
        i++;
    }
    qDebug() << "append(new trx_callin)" << uri;
    m_ChannelInfo_list.at(ChannelInfoListIndex)->list_trxCallIn.append(new trx_callin); //Add
    return i;
}

void iGate4x::getIGateGroupConfig(uint8_t softPhoneID,QWebSocket *pSender)
{
    qDebug() << "getIGateGroupConfig" << softPhoneID;
    int i = (softPhoneIDCheck(softPhoneID));
    QString strIpAddress = "127.0.0.1";
    QString uriName = m_softPhone_list.at(i)->sipUser;
    if (m_softPhone_list.at(i)->defaultEthernet == "eth1")
    {
        strIpAddress = eth1.ipaddress;
    }
    else {
        strIpAddress = eth0.ipaddress;
    }

    QString message = QString("{\"menuID\":\"updateIGateInfo\", \"softPhoneID\":%1, \"ipaddress\":\"%2\", \"uriName\":\"%3\"}").arg(softPhoneID).arg(strIpAddress).arg(uriName);

    pSender->sendTextMessage(message);

    Q_FOREACH(igateGroupInfo *igateInfo, igateGroupInfoList)
    {
        if (igateInfo->softPhoneID == softPhoneID)
        {
            message = QString("{\"menuID\":\"newIgateGroupInfoList\", \"softPhoneID\":%1, \"ipaddress\":\"%2\", \"uriName\":\"%3\", \"visible\":%4}")
                    .arg(igateInfo->iGateID).arg(igateInfo->ipaddress).arg(igateInfo->uriName).arg(igateInfo->visible);
            pSender->sendTextMessage(message);
        }
    }
}
void iGate4x::getRedioTestHomePage(uint8_t softPhoneID,QWebSocket *pSender){
    if (m_softPhone_list.size() == 0) return;
    int i = (softPhoneIDCheck(softPhoneID));
    int tonestate = m_softPhone_list.at(i) -> tone_server_state;
    int pttstate = m_softPhone_list.at(i) -> tone_server_ptt;
    float ServerFrequency = m_softPhone_list.at(i) -> tone_server_frequency;
    float ServerLevel  = m_softPhone_list.at(i) -> tone_server_level;
    int ServerPhase = m_softPhone_list.at(i) -> tone_server_phase;
    int timeoutptt = m_softPhone_list.at(i) -> tone_server_ptt_timeout;

    SocketServer->updateRadioTestDataClient(tonestate, pttstate,timeoutptt,ServerFrequency,ServerPhase,ServerLevel,softPhoneID,pSender);
}
void iGate4x::getServerHomePage(uint8_t softPhoneID,QWebSocket *pSender)
{
    if (m_softPhone_list.size() == 0) return;
    int i = (softPhoneIDCheck(softPhoneID));
    QString deviceName = m_softPhone_list.at(i)->deviceName;
    QString sipUser = m_softPhone_list.at(i)->sipUser;
    uint8_t inputLevel = m_softPhone_list.at(i)->inputLevel;
    uint8_t outputLevel = m_softPhone_list.at(i)->outputLevel;
    uint8_t outputDSPgain = m_softPhone_list.at(i)->outputDSPLevel;
    uint16_t keepAlivePeroid = m_softPhone_list.at(i)->keepAlivePeroid;
    uint16_t sipPort = m_softPhone_list.at(i)->sipPort;
    uint8_t portInterface = 0;
    uint8_t txScheduler = m_softPhone_list.at(i)->txScheduler;
    uint8_t invitemode = m_softPhone_list.at(i)->ServerClientMode;
    float sidetone = m_softPhone_list.at(i)->sidetone;
    float localSidetone = m_softPhone_list.at(i)->localSidetone;
    int defaultEthernet = 0;
    int testModeEnable = 0;
    bool mainRadioTxUsed = m_softPhone_list.at(i)->mainRadioTransmitterUsed;
    bool mainRadioRxUsed = m_softPhone_list.at(i)->mainRadioReceiverUsed;
    bool radioAutoInactive = m_softPhone_list.at(i)->radioAutoInactive;
    int radioMainStandby = m_softPhone_list.at(i)->radioMainStandby;
    int pttDelay = m_softPhone_list.at(i)->pttDelay;
    bool tone_state = m_softPhone_list.at(i)-> tone_state;
    float tone_frequency =  m_softPhone_list.at(i)-> frequency;
    int tone_phase =  m_softPhone_list.at(i)-> phase;
    float tone_level = m_softPhone_list.at(i)-> level;

    SocketServer->updateHomeInput(deviceName,sipUser, inputLevel, outputLevel, pSender, keepAlivePeroid, sipPort, portInterface ,txScheduler,invitemode,
                                  sidetone, defaultEthernet,testModeEnable,softPhoneID,mainRadioTxUsed,mainRadioRxUsed,radioAutoInactive,radioMainStandby,
                                  localSidetone,pttDelay,outputDSPgain,tone_state , tone_frequency, tone_phase, tone_level);
   SocketServer->updateFIRfiltersendClient(FIRInput,FIROutput,pSender);

}
void iGate4x::getHomePage(uint8_t softPhoneID,QWebSocket *pSender)
{
    qDebug() << "getHomePage" << softPhoneID  << "m_softPhone_list.size()" << m_softPhone_list.size();
    if (m_softPhone_list.size() == 0) return;
    int i = (softPhoneIDCheck(softPhoneID));
    QStringList uriAllowList  = m_softPhone_list.at(i)->uriAllowList;
    uint8_t numConnection = m_softPhone_list.at(i)->numConnection;
    QString uri1 = uriAllowList.at(0);
    QString uri2 = uriAllowList.at(1);
    QString uri3 = uriAllowList.at(2);
    QString uri4 = uriAllowList.at(3);
    QString uri5 = uriAllowList.at(4);
    QString uri6 = uriAllowList.at(5);
    QString uri7 = uriAllowList.at(6);
    QString uri8 = uriAllowList.at(7);
    QWebSocket *newClient = pSender;

    int j = (softPhoneIDCheckCHInfo(softPhoneID));

    QString lastPtt = m_ChannelInfo_list.at(j)->lastPttOn;
    QString TxRxStatus;
    uint8_t connNum = m_ChannelInfo_list.at(j)->numConn;



    if (m_ChannelInfo_list.at(j)->PTTOn & m_ChannelInfo_list.at(j)->SQLOn) TxRxStatus = PTTRX;
    else if (m_ChannelInfo_list.at(j)->PTTOn) TxRxStatus = PTTON;
    else if (m_ChannelInfo_list.at(j)->SQLOn) TxRxStatus = RXON;
    else TxRxStatus = STANDBY;

    QString deviceName = m_softPhone_list.at(i)->deviceName;
    QString sipUser = m_softPhone_list.at(i)->sipUser;
    uint8_t inputLevel = m_softPhone_list.at(i)->inputLevel;
    uint8_t outputLevel = m_softPhone_list.at(i)->outputLevel;
    uint8_t outputDSPgain = m_softPhone_list.at(i)->outputDSPLevel;
    uint16_t keepAlivePeroid = m_softPhone_list.at(i)->keepAlivePeroid;
    uint16_t sipPort = m_softPhone_list.at(i)->sipPort;
    uint8_t portInterface = 0;
    uint8_t txScheduler = m_softPhone_list.at(i)->txScheduler;
    uint8_t invitemode = m_softPhone_list.at(i)->ServerClientMode;
    bool mainRadioTxUsed = m_softPhone_list.at(i)->mainRadioTransmitterUsed;
    bool mainRadioRxUsed = m_softPhone_list.at(i)->mainRadioReceiverUsed;
    bool radioAutoInactive = m_softPhone_list.at(i)->radioAutoInactive;
    int radioMainStandby = m_softPhone_list.at(i)->radioMainStandby;
    float sidetone = m_softPhone_list.at(i)->sidetone;
    float localSidetone = m_softPhone_list.at(i)->localSidetone;
    float pttDelay = m_softPhone_list.at(i)->pttDelay;
    int defaultEthernet = 0;
    int testModeEnable = 0;
    bool tone_state = m_softPhone_list.at(i)-> tone_state;
    float tone_frequency =  m_softPhone_list.at(i)-> frequency;
    int tone_phase =  m_softPhone_list.at(i)-> phase;
    float tone_level = m_softPhone_list.at(i)-> level;

    if (m_softPhone_list.at(i)->defaultEthernet == "eth1") defaultEthernet = 1;

    QString uriConnList;
    int trxCallInID = 0;
    Q_FOREACH (trx_callin *callIn, m_ChannelInfo_list.at(j)->list_trxCallIn)
    {
        qDebug() << trxCallInID << "list_trxCallIn" << callIn->uri << callIn->callState;
        if (callIn->callState)
        {
            if (trxCallInID == 0)
                uriConnList += (callIn->uri);
            else
                uriConnList += ","+(callIn->uri);
            trxCallInID++;
        }

    }
    int RadioController = 0;
    switch (softPhoneID) {
    case 1:
        RadioController = RadioControllerCH1;
        break;
    case 2:
        RadioController = RadioControllerCH2;
        break;
    case 3:
        RadioController = RadioControllerCH3;
        break;
    case 4:
        RadioController = RadioControllerCH4;
        break;

    }

    if (m_softPhone_list.at(i)->defaultEthernet == "eth1") defaultEthernet = 1;
    SocketServer->updateAllowedUri(softPhoneID, numConnection, uri1, uri2, uri3, uri4, uri5, uri6, uri7, uri8, newClient);
    SocketServer->updateClientConnStatus(connNum, TxRxStatus, lastPtt, newClient,softPhoneID);
    SocketServer->updateHomeInput(deviceName,sipUser, inputLevel, outputLevel, newClient, keepAlivePeroid, sipPort, portInterface ,txScheduler,
                                  invitemode, sidetone, defaultEthernet,testModeEnable,softPhoneID,mainRadioTxUsed,mainRadioRxUsed,radioAutoInactive,
                                  radioMainStandby,localSidetone,pttDelay,outputDSPgain,tone_state, tone_frequency, tone_phase, tone_level);
    SocketServer->updateURIConnList(uriConnList, newClient);
    SocketServer->updateSqlDefeat(m_softPhone_list.at(i)->sqlAlwayOn,newClient);
    SocketServer->updateSqlActiveHigh(m_softPhone_list.at(i)->sqlActiveHigh,newClient);
    SocketServer->updateRadioController(softPhoneID, RadioController,newClient);
    QString message = QString("{"
                                  "\"menuID\"               :\"dcInStatus\","
                                  "\"dcIn1\"                :%1,"
                                  "\"dcIn2\"                :%2 }"
                                  ).arg(psuDCInStatus).arg(psuBattStatus);
    newClient->sendTextMessage(message);
}
void iGate4x::getNetworkPage(QWebSocket *pSender)
{
        SocketServer->updateNetworktime(Networktimeout,pSender);
        SocketServer->updateNetworkInfo(eth0.dhcpmethod, eth0.ipaddress, eth0.subnet, eth0.gateway, eth0.pridns, eth0.secdns, ntpServer, pSender, "eth0");
        SocketServer->updateNetworkInfo(eth1.dhcpmethod, eth1.ipaddress, eth1.subnet, eth1.gateway, eth1.pridns, eth1.secdns, ntpServer, pSender, "eth1");
        SocketServer->updateNetworkInfo(bond0.dhcpmethod, bond0.ipaddress, bond0.subnet, bond0.gateway, bond0.pridns, bond0.secdns, ntpServer, pSender, "bond0");
}
void iGate4x::getTestRadioPage(QWebSocket *pSender)
{

}
void iGate4x::appendClientCallInStatus(int softPhoneID, QString uri, QString duration, bool pttOn, bool sqlOn, bool callState)
{
    int i = (softPhoneIDCheckCHInfo(softPhoneID));
    m_ChannelInfo_list.at(i)->softPhoneID = softPhoneID;
    int j = checkCallInList(i,uri);
    m_ChannelInfo_list.at(i)->list_trxCallIn.at(j)->uri = uri;
    m_ChannelInfo_list.at(i)->list_trxCallIn.at(j)->duration = duration;
    m_ChannelInfo_list.at(i)->list_trxCallIn.at(j)->pttOn = pttOn;
    m_ChannelInfo_list.at(i)->list_trxCallIn.at(j)->sqlOn = sqlOn;
    m_ChannelInfo_list.at(i)->list_trxCallIn.at(j)->callState = callState;

    SocketServer->broadcastConnDuration(uri,duration,softPhoneID);
}

void iGate4x::appendClientConnStatus(int softPhoneID, QString lastPttOn, QString trxStatus, QString pttURI, bool mainTxPTTOn, bool mainRxSQLOn, int numConn)
{
    int i = (softPhoneIDCheckCHInfo(softPhoneID));
    m_ChannelInfo_list.at(i)->softPhoneID = softPhoneID;
    m_ChannelInfo_list.at(i)->PTTOn = mainTxPTTOn;
    m_ChannelInfo_list.at(i)->SQLOn = mainRxSQLOn;
    m_ChannelInfo_list.at(i)->numConn = numConn;
    m_ChannelInfo_list.at(i)->numTxRx = 0;
    m_ChannelInfo_list.at(i)->lastPttOn = lastPttOn;
    m_ChannelInfo_list.at(i)->pttURI = pttURI;

    set_Tx_LED(softPhoneID,mainTxPTTOn);
    set_Rx_LED(softPhoneID,mainRxSQLOn);

   oled->update_iGateInfo(softPhoneID,mainTxPTTOn,mainRxSQLOn,numConn,0);

   SocketServer->broadcastMessageStatus(numConn,trxStatus,pttURI,softPhoneID);

}
void iGate4x::appendChannelData(uint8_t softPhoneID, int trxMode, bool mainTxPTTOn, bool mainRxSQLOn, int numConn, int numTxRx, QString trxType, QString main01IPAddr, QString main02IPAddr, QString standby01IPAddr, QString standby02IPAddr)
{
    int i = (softPhoneIDCheckCHInfo(softPhoneID));
    int SumNumConn = 0;
    int SumNumTxRx = 0;
    if (i <= (m_softPhone_list.size()+1))
    {
        m_ChannelInfo_list.at(i)->softPhoneID = softPhoneID;
        m_ChannelInfo_list.at(i)->trxMode = trxMode;
        m_ChannelInfo_list.at(i)->PTTOn = mainTxPTTOn;
        m_ChannelInfo_list.at(i)->SQLOn = mainRxSQLOn;
        m_ChannelInfo_list.at(i)->numConn = numConn;
        m_ChannelInfo_list.at(i)->numTxRx = numTxRx;
        m_ChannelInfo_list.at(i)->trxType = trxType;
        m_ChannelInfo_list.at(i)->mainRadio01.ipAddress = main01IPAddr;
        m_ChannelInfo_list.at(i)->mainRadio02.ipAddress = main02IPAddr;
        m_ChannelInfo_list.at(i)->standbyRadio01.ipAddress = standby01IPAddr;
        m_ChannelInfo_list.at(i)->standbyRadio02.ipAddress = standby02IPAddr;

        set_Tx_LED(softPhoneID,mainTxPTTOn);
        set_Rx_LED(softPhoneID,mainRxSQLOn);

       oled->update_iGateInfo(softPhoneID,mainTxPTTOn,mainRxSQLOn,numConn,numTxRx);

       //qDebug() << "softPhoneID,mainTxPTTOn,mainRxSQLOn,numConn,numTxRx" << softPhoneID << mainTxPTTOn << mainRxSQLOn << numConn << numTxRx;
//        set_Output_SQL_or_PTT(softPhoneID,mainRxSQLOn);
    }
    Q_FOREACH (ChannelInfo *softPhone_list, m_ChannelInfo_list)
    {
        SumNumConn += softPhone_list->numConn;
        SumNumTxRx += softPhone_list->numTxRx;
    }
    if (SumNumConn == SumNumTxRx)
    {
        //setLED2_VoIP(LEDGREEN);
    }
    else if (SumNumConn < SumNumTxRx)
    {
        //setLED2_VoIP(LEDRED);
    }
//    qDebug() << SumNumConn << SumNumTxRx;
}
void iGate4x::startSoftphone(int softPhoneID, QWebSocket *socket)
{
    int i = (softPhoneIDCheck(softPhoneID));
    int channelID = 0;
    int sipPort = 0;
    QString sipUser = "";
    int keepAlivePeroid = 0;
    int rtpStartPort = 0;
    uint8_t ServerClientMode = 0;
    uint8_t WireConnectMode = 0;
    quint8  txScheduler;
    quint8  numConnection;
    double   sidetone;
    double   localSidetone;
    bool    sqlAlwayOn;
    bool    sqlActiveHigh;
    bool mainRadioTxUsed;
    bool mainRadioRxUsed;
    bool radioAutoInactive;
    int radioMainStandby;
    QString defaultEthernet = "eth0";
    int clock_rate             = 8000;
    bool rxBestSignalEnable;
    uint8_t groupMute;
    uint8_t pttDelay;
    QStringList uriAllowList;

    if (i <= (m_softPhone_list.size()+1))
    {
        softPhoneID = m_softPhone_list.at(i)->softPhoneID;
        sipUser = m_softPhone_list.at(i)->sipUser;
        sipPort = m_softPhone_list.at(i)->sipPort;
        channelID = m_softPhone_list.at(i)->channelID;
        rtpStartPort = m_softPhone_list.at(i)->rtpStartPort;
        keepAlivePeroid = m_softPhone_list.at(i)->keepAlivePeroid;
        ServerClientMode = m_softPhone_list.at(i)->ServerClientMode;
        WireConnectMode = m_softPhone_list.at(i)->WireConnectMode;
        txScheduler = m_softPhone_list.at(i)->txScheduler;
        numConnection = m_softPhone_list.at(i)->numConnection;
        sidetone = m_softPhone_list.at(i)->sidetone;
        localSidetone = m_softPhone_list.at(i)->localSidetone;
        sqlAlwayOn = m_softPhone_list.at(i)->sqlAlwayOn;
        sqlActiveHigh = m_softPhone_list.at(i)->sqlActiveHigh;
        uriAllowList  = m_softPhone_list.at(i)->uriAllowList;
        mainRadioTxUsed = m_softPhone_list.at(i)->mainRadioTransmitterUsed;
        mainRadioRxUsed = m_softPhone_list.at(i)->mainRadioReceiverUsed;
        radioAutoInactive = m_softPhone_list.at(i)->radioAutoInactive;
        radioMainStandby = m_softPhone_list.at(i)->radioMainStandby;
        m_softPhone_list.at(i)->socketClients = socket;
        m_softPhone_list.at(i)->softphoneRunning = true;
        oled->update_iGateInfo(softPhoneID,0,0,0,0);
        defaultEthernet = m_softPhone_list.at(i)->defaultEthernet;
        rxBestSignalEnable = m_softPhone_list.at(i)->rxBestSignalEnable;
        groupMute = m_softPhone_list.at(i)->groupMute;
        pttDelay = m_softPhone_list.at(i)->pttDelay;
        qDebug() << "startSoftphone" << softPhoneID << sipUser << channelID;

        dataLoggerServer->createDataLog(INFO,QString("Start Softphone softPhoneID %1, sipUser %2, Mode: %3").arg(softPhoneID).arg(sipUser).arg(inviteMode));

        switch (softPhoneID)
        {
        case 1:
            SigmaFirmWareDownLoad->setMixerVolume(SIDETONE_VOLUME_CH1_ADDRESS,SIDETONE_VOLUME_CH1_MODE_ADDRESS,SIDETONE_VOLUME_CH1_MODE_VALUE,0);
            break;
        case 2:
            SigmaFirmWareDownLoad->setMixerVolume(SIDETONE_VOLUME_CH2_ADDRESS,SIDETONE_VOLUME_CH2_MODE_ADDRESS,SIDETONE_VOLUME_CH2_MODE_VALUE,0);
            break;
        case 3:
            SigmaFirmWareDownLoad->setMixerVolume(SIDETONE_VOLUME_CH3_ADDRESS,SIDETONE_VOLUME_CH3_MODE_ADDRESS,SIDETONE_VOLUME_CH3_MODE_VALUE,0);
            break;
        case 4:
            SigmaFirmWareDownLoad->setMixerVolume(SIDETONE_VOLUME_CH4_ADDRESS,SIDETONE_VOLUME_CH4_MODE_ADDRESS,SIDETONE_VOLUME_CH4_MODE_VALUE,0);
            break;
        }
        QString message = QString("{"
                          "\"menuID\"               :\"controlerData\", "
                          "\"softPhoneID\"          :%1, "
                          "\"sipUser\"              :\"%2\", "
                          "\"sipPort\"              :%3, "
                          "\"channelID\"            :%4, "
                          "\"rtpStartPort\"         :%5, "
                          "\"keepAlivePeroid\"      :%6, "
                          "\"defaultEthernet\"      :\"%7\", "
                          "\"inviteMode\"           :%8, "
                          "\"txScheduler\"          :%9, "
                          "\"WireConnectMode\"      :%10, "
                          "\"numConnection\"        :%11, "
                          "\"sidetone\"             :%12, "
                          "\"sqlAlwayOn\"           :%13, "
                          "\"sqlActiveHigh\"        :%14, "
                          "\"clock_rate\"           :%15, "
                          "\"mainRadioTransmitterUsed\" :%16, "
                          "\"mainRadioReceiverUsed\"    :%17, "
                          "\"radioAutoInactive\"        :%18, "
                          "\"radioMainStandby\"         :%19, "
                          "\"rxBestSignalEnable\"       :%20, "
                          "\"localSidetone\"            :%21, "
                          "\"groupMute\"                :%22, "
                          "\"pttDelay\"                 :%23 "
                          "}"
                          )
                .arg(softPhoneID).arg(sipUser).arg(sipPort).arg(channelID).arg(rtpStartPort).arg(keepAlivePeroid).arg(defaultEthernet).arg(ServerClientMode)
                .arg(txScheduler).arg(WireConnectMode).arg(numConnection).arg(sidetone).arg(sqlAlwayOn).arg(sqlActiveHigh).arg(clock_rate).arg(mainRadioTxUsed)
                .arg(mainRadioRxUsed).arg(radioAutoInactive).arg(radioMainStandby).arg(rxBestSignalEnable).arg(localSidetone).arg(groupMute).arg(pttDelay);
        if (socket != nullptr)
            socket->sendTextMessage(message);

        if (ServerClientMode == CLIENT)
        {
            message = QString("{"
                              "\"menuID\"               :\"uriAllowList\", "
                              "\"softPhoneID\"          :%1, "
                              "\"allowUri1\"            :\"%2\", "
                              "\"allowUri2\"            :\"%3\", "
                              "\"allowUri3\"            :\"%4\", "
                              "\"allowUri4\"            :\"%5\", "
                              "\"allowUri5\"            :\"%6\", "
                              "\"allowUri6\"            :\"%7\", "
                              "\"allowUri7\"            :\"%8\", "
                              "\"allowUri8\"            :\"%9\"  "
                              "}").arg(softPhoneID).arg(uriAllowList.at(0)).arg(uriAllowList.at(1)).arg(uriAllowList.at(2)).arg(uriAllowList.at(3)).arg(uriAllowList.at(4)).arg(uriAllowList.at(5)).arg(uriAllowList.at(6)).arg(uriAllowList.at(7));
            if (socket != nullptr)
                socket->sendTextMessage(message);
        }
        else if (ServerClientMode == SERVER)
        {
            int i = (callChannelIDCheck(softPhoneID));
            quint8 trxMainSTB = 1; //1 Main, 2 Standby
            QString channelName = channel_call_list.at(i)->mainRadioName;
            QString trxmode = channel_call_list.at(i)->mainRadioType;
            bool enable = channel_call_list.at(i)->mainRadioEnable;
            bool alwaysConnect = channel_call_list.at(i)->mainAlwaysConnect;
            QString radio1callName = channel_call_list.at(i)->mainRadio01.uri;
            QString radio1ipAddress = channel_call_list.at(i)->mainRadio01.ipAddress;
            int radio1portAddr = channel_call_list.at(i)->mainRadio01.portAddr;
            QString radio2callName = channel_call_list.at(i)->mainRadio02.uri;
            QString radio2ipAddress = channel_call_list.at(i)->mainRadio02.ipAddress;
            int radio2portAddr = channel_call_list.at(i)->mainRadio02.portAddr;
            message = QString("{"
                              "\"menuID\"                   :\"initRadio\", "
                              "\"trxMainSTB\"               :%1, "
                              "\"channelName\"              :\"%2\", "
                              "\"trxmode\"                  :\"%3\", "
                              "\"enable\"                   :%4, "
                              "\"alwaysConnect\"            :%5, "
                              "\"radio1callName\"           :\"%6\", "
                              "\"radio1ipAddress\"          :\"%7\", "
                              "\"radio1portAddr\"           :\"%8\", "
                              "\"radio2callName\"           :\"%9\", "
                              "\"radio2ipAddress\"          :\"%10\", "
                              "\"radio2portAddr\"           :\"%11\", "
                              "\"softPhoneID\"              :%12 "
                              "}").arg(trxMainSTB).arg(channelName).arg(trxmode).arg(enable).arg(alwaysConnect).arg(radio1callName).arg(radio1ipAddress).arg(radio1portAddr)
                    .arg(radio2callName).arg(radio2ipAddress).arg(radio2portAddr).arg(softPhoneID);
            if (socket != nullptr)
                socket->sendTextMessage(message);

            trxMainSTB = 2; //1 Main, 2 Standby
            channelName = channel_call_list.at(i)->standbyRadioName;
            trxmode = channel_call_list.at(i)->standbyRadioType;
            enable = channel_call_list.at(i)->standbyEnable;
            alwaysConnect = channel_call_list.at(i)->standbyAlwaysConnect;
            radio1callName = channel_call_list.at(i)->standbyRadio01.uri;
            radio1ipAddress = channel_call_list.at(i)->standbyRadio01.ipAddress;
            radio1portAddr = channel_call_list.at(i)->standbyRadio01.portAddr;
            radio2callName = channel_call_list.at(i)->standbyRadio02.uri;
            radio2ipAddress = channel_call_list.at(i)->standbyRadio02.ipAddress;
            radio2portAddr = channel_call_list.at(i)->standbyRadio02.portAddr;

            message = QString("{"
                              "\"menuID\"                   :\"initRadio\", "
                              "\"trxMainSTB\"               :%1, "
                              "\"channelName\"              :\"%2\", "
                              "\"trxmode\"                  :\"%3\", "
                              "\"enable\"                   :%4, "
                              "\"alwaysConnect\"            :%5, "
                              "\"radio1callName\"           :\"%6\", "
                              "\"radio1ipAddress\"          :\"%7\", "
                              "\"radio1portAddr\"           :\"%8\", "
                              "\"radio2callName\"           :\"%9\", "
                              "\"radio2ipAddress\"          :\"%10\", "
                              "\"radio2portAddr\"           :\"%11\", "
                              "\"softPhoneID\"              :%12 "
                              "}").arg(trxMainSTB).arg(channelName).arg(trxmode).arg(enable).arg(alwaysConnect).arg(radio1callName).arg(radio1ipAddress).arg(radio1portAddr)
                    .arg(radio2callName).arg(radio2ipAddress).arg(radio2portAddr).arg(softPhoneID);
            if (socket != nullptr)
                socket->sendTextMessage(message);
        }
        message = QString("{"
                          "\"menuID\"                   :\"startSoftphone\", "
                          "\"softPhoneID\"              :%1 "
                          "}").arg(softPhoneID);
        if (socket != nullptr)
            socket->sendTextMessage(message);


    }
}
void iGate4x::removeSoftphone(QWebSocket *socket)
{
    Q_FOREACH(SoftPhone *softPhone ,m_softPhone_list)
    {
        if (softPhone->socketClients == socket){
            qDebug() << "Disconnected form softPhoneID" << softPhone->softPhoneID << "sipUser" << softPhone->sipUser << "channelID" << softPhone->channelID;
            onComponentCompleted(softPhone->softPhoneID,softPhone->sipUser,softPhone->channelID);
            softPhone->softphoneRunning = false;
            softPhone->socketClients = nullptr;
            oled->update_iGateInfo(softPhone->softPhoneID,0,0,0,0);
            if (softPhone->softPhoneID == 1)
                ppt_sql_output1->resetGpio();
            else if (softPhone->softPhoneID == 2)
                ppt_sql_output2->resetGpio();
            else if (softPhone->softPhoneID == 3)
                ppt_sql_output3->resetGpio();
            else if (softPhone->softPhoneID == 4)
                ppt_sql_output4->resetGpio();
        }
    }
}

void iGate4x::getCtrlInfoPage(int softPhoneID)
{
    Q_FOREACH(SoftPhone *softPhone, m_softPhone_list)
    {

        if ((softPhone->softPhoneID == softPhoneID) & (softPhone->socketClients != nullptr))
        {
            QString  message = QString("{"
                                       "\"menuID\"          :\"getChannelMessage\"}");
            SocketServer->sendMessage(softPhone->socketClients,message);

        }
    }
}

int iGate4x::softPhoneIDCheck(int softPhoneID)
{
    int i = 0;
    Q_FOREACH (SoftPhone *softPhone_list, m_softPhone_list){
        if (softPhone_list->softPhoneID == softPhoneID){
            return i;
        }
        i++;
    }
    return i;
}
void iGate4x::newRediodata(uint8_t softPhoneID, bool tonestate, bool pttstate, int timeoutptt, float ServerFrequency, int ServerPhase, float ServerLevel){
    int i = (softPhoneIDCheck(softPhoneID));
    if (i == m_softPhone_list.size())
        m_softPhone_list.append(new SoftPhone); //Add
    else if(i > m_softPhone_list.size()) return;
    m_softPhone_list.at(i) -> tone_server_state = tonestate;
    m_softPhone_list.at(i) -> tone_server_ptt = pttstate;
    m_softPhone_list.at(i) -> tone_server_frequency = ServerFrequency;
    m_softPhone_list.at(i) -> tone_server_level = ServerLevel;
    m_softPhone_list.at(i) -> tone_server_phase =  ServerPhase;
    m_softPhone_list.at(i) -> tone_server_ptt_timeout = timeoutptt;

    updateServerTone(tonestate,pttstate,timeoutptt,ServerFrequency,ServerPhase,ServerLevel,softPhoneID);
}
void iGate4x::onNewControler(int softPhoneID, int channelID, int sipPort, QString sipUser, int keepAlivePeroid, int rtpStartPort, uint8_t wireConnectMode,
                             bool mainRadioTransmitterUsed, bool mainRadioReceiverUsed, uint8_t ServerClientMode, quint8  txScheduler, quint8  numConnection, float sidetone,
                             float localSidetone, bool sqlAlwayOn, bool sqlActiveHigh, QString deviceName, int inputLevel, int outputLevel,
                             bool radioAutoInactive, int radioMainStandby, QString defaultEthernet, bool rxBestSignalEnable, uint8_t groupMute, uint8_t pttDelay, uint8_t outputDSPLevel
                             ,bool tone_state,float frequency,int phase,float level)
{
    int i = (softPhoneIDCheck(softPhoneID));
    qDebug() << "getNewControler" << softPhoneID << sipUser << channelID << "wireConnectMode" << wireConnectMode << "radioMainStandby" << radioMainStandby << "pttDelay" << pttDelay;
    if (i == m_softPhone_list.size())
        m_softPhone_list.append(new SoftPhone); //Add
    else if(i > m_softPhone_list.size()) return;

    //Update    
    m_softPhone_list.at(i)->softPhoneID = softPhoneID;
    m_softPhone_list.at(i)->sipUser = sipUser;
    m_softPhone_list.at(i)->sipPort = sipPort;
    m_softPhone_list.at(i)->channelID = channelID;
    m_softPhone_list.at(i)->rtpStartPort = rtpStartPort;
    m_softPhone_list.at(i)->keepAlivePeroid = keepAlivePeroid;
    m_softPhone_list.at(i)->WireConnectMode = wireConnectMode;
    m_softPhone_list.at(i)->mainRadioTransmitterUsed = mainRadioTransmitterUsed;
    m_softPhone_list.at(i)->mainRadioReceiverUsed = mainRadioReceiverUsed;
    m_softPhone_list.at(i)->ServerClientMode = ServerClientMode;
    m_softPhone_list.at(i)->txScheduler = txScheduler;
    m_softPhone_list.at(i)->numConnection = numConnection;
    m_softPhone_list.at(i)->sidetone = sidetone;
    m_softPhone_list.at(i)->localSidetone = localSidetone;
    m_softPhone_list.at(i)->sqlAlwayOn = sqlAlwayOn;
    m_softPhone_list.at(i)->sqlActiveHigh = sqlActiveHigh;
    m_softPhone_list.at(i)->deviceName = deviceName;
    m_softPhone_list.at(i)->inputLevel = inputLevel;
    m_softPhone_list.at(i)->outputLevel = outputLevel;    
    m_softPhone_list.at(i)->outputDSPLevel = outputDSPLevel;
    m_softPhone_list.at(i)->radioAutoInactive = radioAutoInactive;
    m_softPhone_list.at(i)->radioMainStandby = radioMainStandby;
    m_softPhone_list.at(i)->uriAllowList = myDatabase->getUriAllowList(softPhoneID);
    m_softPhone_list.at(i)->defaultEthernet = defaultEthernet;
    m_softPhone_list.at(i)->rxBestSignalEnable = rxBestSignalEnable;
    m_softPhone_list.at(i)->groupMute = groupMute;
    m_softPhone_list.at(i)->pttDelay = pttDelay;
    m_softPhone_list.at(i)-> tone_state = tone_state;
    m_softPhone_list.at(i)-> frequency = frequency;
    m_softPhone_list.at(i)-> phase = phase;
    m_softPhone_list.at(i)-> level = level;

    stopAllSoftphone(softPhoneID);
    startAllSoftphone(softPhoneID);
    onComponentCompleted(softPhoneID,sipUser,channelID);
//    qDebug() << "debugTone " << m_softPhone_list.at(i)-> phase << "frequency" <<  m_softPhone_list.at(i)-> frequency;
    updateInputGain(inputLevel,softPhoneID);
    updateOutputGain(outputLevel,softPhoneID);
    updateDSPOutputGain(outputDSPLevel,softPhoneID);
    updateInputTone(tone_state,frequency,phase,level,softPhoneID);
// qDebug() << "debugTone " <<tone_state <<"frequency"<<frequency;
//    bool , float , int , float, uint8_t
    //system("amixer -c APE sset 'I2S3 fsync width' 0");
    //usleep(100000);
    //system("amixer -c APE sset 'I2S3 fsync width' 1");

}

void iGate4x::loopGetInfo()
{
    iCount++;
//    if (SocketServer->m_WebSocketClients.size() > 0)
    {
        QString currentTime = QTime::currentTime().toString("hh:mm:ss");
        if (currentTime != lastGetCurrentTime){
            lastGetCurrentTime = currentTime;
            float cputemp = networking->getCPUTemp();
            SocketServer->broadcastSystemDateTime(cputemp);
//            qDebug() << "loopGetInfo" << currentTime << QDate::currentDate().dayOfWeek();
            if (currentTime == "00:00:00")
            {
                int date = QDate::currentDate().dayOfWeek();
                if(date == 1)
                    Truncatesyslog();
            }
        }
    }

    bool eth0lan_ok = false;
    bool eth1lan_ok = false;
    bool bond0lan_ok = false;

    if (iCount == 10)
    {
        eth0lan_ok = networking->getLinkDetected("eth0");
        if (eth0lan_ok)
        {
//            networking->getIPAddress("eth0");
            networking->getIPAddress("bond0");
            eth0.ipaddress = networking->netWorkCardAddr;
            bond0.ipaddress = networking->netWorkCardAddr;
        }
        if (eth0.lan_ok != eth0lan_ok)
        {
            eth0.lan_ok = eth0lan_ok;
            dataLoggerServer->createDataLog(eth0lan_ok,QString("ETH1 : %1").arg(eth0lan_ok ? "UP" : "DOWN"));
        }
//        qDebug() << "lan0 Ok" << eth0.lan_ok << "eth0.ipaddress" << eth0.ipaddress;
    }
    else if (iCount == 15)
    {
        eth1lan_ok = networking->getLinkDetected("eth1");
        if(eth1lan_ok)
        {
//            networking->getIPAddress("eth1");
            networking->getIPAddress("bond0");
            eth1.ipaddress = networking->netWorkCardAddr;
            bond0.ipaddress = networking->netWorkCardAddr;
//            qDebug() << "lan1 Ok" << eth1.lan_ok << "eth1.ipaddress" << eth1.ipaddress;
        }
        if (eth1.lan_ok != eth1lan_ok)
        {
            eth1.lan_ok = eth1lan_ok;
            dataLoggerServer->createDataLog(eth1lan_ok,QString("ETH2 : %1").arg(eth1lan_ok ? "UP" : "DOWN"));
        }

//        iCount = 0;
    }
    else if (iCount == 20)
    {
        bond0lan_ok = networking->getLinkDetected("bond0");
        if(bond0lan_ok)
        {
            networking->getIPAddress("bond0");
            bond0.ipaddress = networking->netWorkCardAddr;
        }
        if (bond0.lan_ok != bond0lan_ok)
        {
            bond0.lan_ok = bond0lan_ok;
            restartSoftphone();
            qDebug() << "on bond0 changed" << bond0.lan_ok << "bond0.ipaddress" << bond0.ipaddress << "Restart All Softphone";
        }

        iCount = 0;
    }


}

void iGate4x::displayThread()
{
    qDebug() << "*********************************************************** displayThread start ********************************************************" ;
    oled = new OLEDDriver("/dev/spidev2.1");
    phyNetworkInfo _eth0;
    phyNetworkInfo _eth1;
    ChannelInfo iGate1;
    ChannelInfo iGate2;
    ChannelInfo iGate3;
    int blinkCount = 0;
    bool _iGate1Run = false;
    bool _iGate2Run = false;
    bool _iGate3Run = false;
    bool softphoneBlink = false;
    while (m_IsEventThreadRunning2)
    {
        if (spi2Busy == false)
        {
            boost::unique_lock<boost::mutex> scoped_lock(io_mutex2);

            if ((eth0.ipaddress != _eth0.ipaddress) || (eth0.lan_ok != _eth0.lan_ok)) onIP1Changed = true;
            if ((eth1.ipaddress != _eth1.ipaddress) || (eth1.lan_ok != _eth1.lan_ok)) onIP2Changed = true;

            _eth0 = eth0;
            _eth1 = eth1;

            //qDebug() << eth0.ipaddress << eth0.lan_ok;
            ::usleep(100000);
            if  (onIP1Changed)
            {
                oled->updateIPAddress("eth0",eth0.ipaddress,eth0.lan_ok);
                onIP1Changed = false;
                qDebug() << "ETH0 Link Detected:" << eth0.lan_ok;
            }
            if  (onIP2Changed)
            {
                oled->updateIPAddress("eth1",eth1.ipaddress,eth1.lan_ok);
                onIP2Changed = false;
                qDebug() << "ETH1 Link Detected:" << eth1.lan_ok;
            }

            blinkCount++;


            if (blinkCount == 5)
            {

                softphoneBlink = (!softphoneBlink);
                Q_FOREACH(SoftPhone *softPhone ,m_softPhone_list)
                {
                    int NumConn = 0;
                    int NumTxRx = 0;
                    Q_FOREACH (ChannelInfo *softPhone_list, m_ChannelInfo_list)
                    {
                        if (softPhone_list->softPhoneID == softPhone->softPhoneID)
                        {
                            NumConn = softPhone_list->numConn;
                            NumTxRx = softPhone_list->numTxRx;
                            break;
                        }
                    }
    //                if (NumConn < NumTxRx)
                    {
                        if (softPhone->softphoneRunning)
                            softPhone->softphoneBlink = softphoneBlink;
                        else
                            softPhone->softphoneBlink = false;
                    }
    //                else if (NumConn >= NumTxRx)
    //                {
    //                    if (softPhone->softphoneRunning)
    //                        softPhone->softphoneBlink = true;
    //                    else
    //                        softPhone->softphoneBlink = false;
    //                }
                    oled->update_igateRun(softPhone->softPhoneID,softPhone->softphoneBlink);
                }

                blinkCount = 0;
            }
            oled->writeOLEDRun();
        }
        else
        {
            ::usleep(100);
        }
    }

}
