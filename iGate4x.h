#ifndef IGATE3X_H
#define IGATE3X_H

#include <QObject>
#include "CS42448.h"
#include "PCM3168A.h"
#include "ChatServer.h"
#include "Database.h"
#include "NetWorkMng.h"
#include "SnmpStack.h"
#include <GPIOClass.h>
#include <OLEDDriver.h>
#include <SigmaStudioFW.h>
#include <ChatClient.h>
#include <QTimer>

//DisableSnmp
#define SWVERSION "7.5.2 DEBUG 2211024"
#define HWVERSION "iGate4CH DSP"
// V7.3 Backup and Restore

// 7.4.2 Stop RadioCtrlService when server mode
//"7.4.5 DEBUG 1111024" URI 64 Char
#define GPIO00	"PZ.01"     //"479"		//GPIO33_PZ.01		tegra234-gpio = (23*8)+1 = 185
#define GPIO01	"PQ.05"     //"453"		//GPIO33_PQ.05		tegra234-gpio = (15*8)+5 = 125
#define GPIO02	"PP.06"     //"446"		//GPIO33_PP.06		tegra234-gpio = (14*8)+6 = 118
#define GPIO03	"PCC.00"    //"328"		//GPIO33_PCC.00		tegra234-gpio-aon = (02*8)+0 = 16
#define GPIO04	"PCC.01"    //"329"		//GPIO33_PCC.01		tegra234-gpio-aon = (02*8)+1 = 17
#define GPIO05 	"PCC.02"    //"330"		//GPIO33_PCC.02		tegra234-gpio-aon = (02*8)+2 = 18
#define GPIO06	"PCC.03"    //"331"		//GPIO33_PCC.03		tegra234-gpio-aon = (02*8)+3 = 19
#define GPIO07  "PG.06"     //"389"		//GPIO33_PG.06		tegra234-gpio = (06*8)+6 = 54
#define GPIO08  "PQ.02"     //"450"		//GPIO33_PQ.02		tegra234-gpio = (15*8)+2 = 122 // NV_THERM_FAN_TACH0
#define GPIO09	"PAC.06"    //"492"		//GPIO33_PAC.06		tegra234-gpio = (24*8)+6 = 198
#define GPIO10	"PEE.02"    //"341"		//GPIO33_PEE.02		tegra234-gpio-aon = (04*8)+2 = 34
#define GPIO11	"PQ.06"     //"454"		//GPIO33_PQ.06		tegra234-gpio = (15*8)+6 = 126
#define GPIO12	"PN.01"     //"433"		//GPIO33_PN.01		tegra234-gpio = (13*8)+1 = 105
#define GPIO13  "PH.00"     //"391"		//GPIO33_PH.00		tegra234-gpio = (07*8)+0 = 56
#define GPIO14	"PX.03"     //"465"		//GPIO33_PX.03		tegra234-gpio = (21*8)+3 = 171


#define AUDIOIN_VOLUME_CH1_ADDRESS MOD_SPLT1_ALG0_STAGE0_MULTICTRLSPLITTERS300SLEW2TARGET0_ADDR
#define SIDETONE_VOLUME_CH1_ADDRESS MOD_SPLT1_ALG0_STAGE1_MULTICTRLSPLITTERS300SLEW2TARGET1_ADDR
#define SIDETONE_VOLUME_CH1_MODE_ADDRESS MOD_SPLT1_ALG0_SLEW_MODE_ADDR
#define SIDETONE_VOLUME_CH1_MODE_VALUE MOD_SPLT1_ALG0_SLEW_MODE_FIXPT

#define AUDIOIN_VOLUME_CH2_ADDRESS MOD_SPLT2_ALG0_STAGE0_MULTICTRLSPLITTERS300SLEW1TARGET0_ADDR
#define SIDETONE_VOLUME_CH2_ADDRESS MOD_SPLT2_ALG0_STAGE1_MULTICTRLSPLITTERS300SLEW1TARGET1_ADDR
#define SIDETONE_VOLUME_CH2_MODE_ADDRESS MOD_SPLT2_ALG0_SLEW_MODE_ADDR
#define SIDETONE_VOLUME_CH2_MODE_VALUE MOD_SPLT2_ALG0_SLEW_MODE_FIXPT

#define AUDIOIN_VOLUME_CH3_ADDRESS MOD_SPLT3_ALG0_STAGE0_MULTICTRLSPLITTERS300SLEW3TARGET0_ADDR
#define SIDETONE_VOLUME_CH3_ADDRESS MOD_SPLT3_ALG0_STAGE1_MULTICTRLSPLITTERS300SLEW3TARGET1_ADDR
#define SIDETONE_VOLUME_CH3_MODE_ADDRESS MOD_SPLT3_ALG0_SLEW_MODE_ADDR
#define SIDETONE_VOLUME_CH3_MODE_VALUE MOD_SPLT3_ALG0_SLEW_MODE_FIXPT

#define AUDIOIN_VOLUME_CH4_ADDRESS MOD_SPLT4_ALG0_STAGE0_MULTICTRLSPLITTERS300SLEW4TARGET0_ADDR
#define SIDETONE_VOLUME_CH4_ADDRESS MOD_SPLT4_ALG0_STAGE1_MULTICTRLSPLITTERS300SLEW4TARGET1_ADDR
#define SIDETONE_VOLUME_CH4_MODE_ADDRESS MOD_SPLT4_ALG0_SLEW_MODE_ADDR
#define SIDETONE_VOLUME_CH4_MODE_VALUE MOD_SPLT4_ALG0_SLEW_MODE_FIXPT

#define AUDIOOUT_VOLUME_CH1_TARGET_ADDR MOD_SINGLE1_ALG0_TARGET_ADDR
#define AUDIOOUT_VOLUME_CH1_MOD_ADDR MOD_SINGLE1_ALG0_SLEW_MODE_ADDR
#define AUDIOOUT_VOLUME_CH1_MOD_VALUE MOD_SINGLE1_ALG0_SLEW_MODE_FIXPT

#define AUDIOOUT_VOLUME_CH2_TARGET_ADDR MOD_SINGLE2_ALG0_TARGET_ADDR
#define AUDIOOUT_VOLUME_CH2_MOD_ADDR MOD_SINGLE2_ALG0_SLEW_MODE_ADDR
#define AUDIOOUT_VOLUME_CH2_MOD_VALUE MOD_SINGLE2_ALG0_SLEW_MODE_FIXPT

#define AUDIOOUT_VOLUME_CH3_TARGET_ADDR MOD_SINGLE3_ALG0_TARGET_ADDR
#define AUDIOOUT_VOLUME_CH3_MOD_ADDR MOD_SINGLE3_ALG0_SLEW_MODE_ADDR
#define AUDIOOUT_VOLUME_CH3_MOD_VALUE MOD_SINGLE3_ALG0_SLEW_MODE_FIXPT

#define AUDIOOUT_VOLUME_CH4_TARGET_ADDR MOD_SINGLE4_ALG0_TARGET_ADDR
#define AUDIOOUT_VOLUME_CH4_MOD_ADDR MOD_SINGLE4_ALG0_SLEW_MODE_ADDR
#define AUDIOOUT_VOLUME_CH4_MOD_VALUE MOD_SINGLE4_ALG0_SLEW_MODE_FIXPT

// TONE define
#define TONE_CH1_ADDRESS MOD_TONE1_ALG0_SINEPHASEGAINALGS3001GAIN00_ADDR
#define TONE_LEVEL_CH1_ADDRESS MOD_TONE1_ALG0_SINEPHASEGAINALGS3001GAIN00_ADDR
#define TONE_PHASE_CH1_ADDRESS MOD_TONE1_ALG0_SINEPHASEGAINALGS3001INDEXFPHI00_ADDR
#define TONE_FREQUENCY_CH1_ADDRESS MOD_TONE1_ALG0_INCREMENT_ADDR

#define TONE_CH2_ADDRESS MOD_TONE1_2_ALG0_SINEPHASEGAINALGS3002GAIN00_ADDR
#define TONE_LEVEL_CH2_ADDRESS MOD_TONE1_2_ALG0_SINEPHASEGAINALGS3002GAIN00_ADDR
#define TONE_PHASE_CH2_ADDRESS MOD_TONE1_2_ALG0_SINEPHASEGAINALGS3002INDEXFPHI00_ADDR
#define TONE_FREQUENCY_CH2_ADDRESS MOD_TONE1_2_ALG0_INCREMENT_ADDR

#define TONE_CH3_ADDRESS MOD_TONE1_3_ALG0_SINEPHASEGAINALGS3003GAIN00_ADDR
#define TONE_LEVEL_CH3_ADDRESS MOD_TONE1_3_ALG0_SINEPHASEGAINALGS3003GAIN00_ADDR
#define TONE_PHASE_CH3_ADDRESS MOD_TONE1_3_ALG0_SINEPHASEGAINALGS3003INDEXFPHI00_ADDR
#define TONE_FREQUENCY_CH3_ADDRESS MOD_TONE1_3_ALG0_INCREMENT_ADDR

#define TONE_CH4_ADDRESS MOD_TONE1_4_ALG0_SINEPHASEGAINALGS3004GAIN00_ADDR
#define TONE_LEVEL_CH4_ADDRESS MOD_TONE1_4_ALG0_SINEPHASEGAINALGS3004GAIN00_ADDR
#define TONE_PHASE_CH4_ADDRESS MOD_TONE1_4_ALG0_SINEPHASEGAINALGS3004INDEXFPHI00_ADDR
#define TONE_FREQUENCY_CH4_ADDRESS MOD_TONE1_4_ALG0_INCREMENT_ADDR

// TONE SERVSR
#define TONE_SERVER_CH1_ADDRESS MOD_TONE1_5_ALG0_SINEPHASEGAINALGS3006GAIN00_ADDR
#define TONE_SERVER_LEVEL_CH1_ADDRESS MOD_TONE1_5_ALG0_SINEPHASEGAINALGS3006GAIN00_ADDR
#define TONE_SERVER_PHASE_CH1_ADDRESS MOD_TONE1_5_ALG0_SINEPHASEGAINALGS3006INDEXFPHI00_ADDR
#define TONE_SERVER_FREQUENCY_CH1_ADDRESS MOD_TONE1_5_ALG0_INCREMENT_ADDR

#define TONE_SERVER_CH2_ADDRESS MOD_TONE1_6_ALG0_SINEPHASEGAINALGS3007GAIN00_ADDR
#define TONE_SERVER_LEVEL_CH2_ADDRESS MOD_TONE1_6_ALG0_SINEPHASEGAINALGS3007GAIN00_ADDR
#define TONE_SERVER_PHASE_CH2_ADDRESS MOD_TONE1_6_ALG0_SINEPHASEGAINALGS3007INDEXFPHI00_ADDR
#define TONE_SERVER_FREQUENCY_CH2_ADDRESS MOD_TONE1_6_ALG0_INCREMENT_ADDR

#define TONE_SERVER_CH3_ADDRESS MOD_TONE1_7_ALG0_SINEPHASEGAINALGS3008GAIN00_ADDR
#define TONE_SERVER_LEVEL_CH3_ADDRESS MOD_TONE1_7_ALG0_SINEPHASEGAINALGS3008GAIN00_ADDR
#define TONE_SERVER_PHASE_CH3_ADDRESS MOD_TONE1_7_ALG0_SINEPHASEGAINALGS3008INDEXFPHI00_ADDR
#define TONE_SERVER_FREQUENCY_CH3_ADDRESS MOD_TONE1_7_ALG0_INCREMENT_ADDR

#define TONE_SERVER_CH4_ADDRESS MOD_TONE1_8_ALG0_SINEPHASEGAINALGS3005GAIN00_ADDR
#define TONE_SERVER_LEVEL_CH4_ADDRESS MOD_TONE1_8_ALG0_SINEPHASEGAINALGS3005GAIN00_ADDR
#define TONE_SERVER_PHASE_CH4_ADDRESS MOD_TONE1_8_ALG0_SINEPHASEGAINALGS3005INDEXFPHI00_ADDR
#define TONE_SERVER_FREQUENCY_CH4_ADDRESS MOD_TONE1_8_ALG0_INCREMENT_ADDR
///////////////////////////////////

#define GPIO1_OFFSET 284 //i2c-1 address 0x20
#define GPA1_0 GPIO1_OFFSET+0
#define GPA1_1 GPIO1_OFFSET+1
#define GPA1_2 GPIO1_OFFSET+2
#define GPA1_3 GPIO1_OFFSET+3
#define GPA1_4 GPIO1_OFFSET+4
#define GPA1_5 GPIO1_OFFSET+5
#define GPA1_6 GPIO1_OFFSET+6
#define GPA1_7 GPIO1_OFFSET+7

#define GPB1_0 GPIO1_OFFSET+8
#define GPB1_1 GPIO1_OFFSET+9
#define GPB1_2 GPIO1_OFFSET+10
#define GPB1_3 GPIO1_OFFSET+11
#define GPB1_4 GPIO1_OFFSET+12
#define GPB1_5 GPIO1_OFFSET+13
#define GPB1_6 GPIO1_OFFSET+14
#define GPB1_7 GPIO1_OFFSET+15

#define GPIO2_OFFSET 300 //i2c-1 address 0x21
#define GPA2_0 GPIO2_OFFSET+0
#define GPA2_1 GPIO2_OFFSET+1
#define GPA2_2 GPIO2_OFFSET+2
#define GPA2_3 GPIO2_OFFSET+3
#define GPA2_4 GPIO2_OFFSET+4
#define GPA2_5 GPIO2_OFFSET+5
#define GPA2_6 GPIO2_OFFSET+6
#define GPA2_7 GPIO2_OFFSET+7

#define GPB2_0 GPIO2_OFFSET+8
#define GPB2_1 GPIO2_OFFSET+9
#define GPB2_2 GPIO2_OFFSET+10
#define GPB2_3 GPIO2_OFFSET+11
#define GPB2_4 GPIO2_OFFSET+12
#define GPB2_5 GPIO2_OFFSET+13
#define GPB2_6 GPIO2_OFFSET+14
#define GPB2_7 GPIO2_OFFSET+15

#define PTT_SQL_INPUT1 GPA2_7
#define PTT_SQL_INPUT2 GPA2_6
#define PTT_SQL_INPUT3 GPA2_5
#define PTT_SQL_INPUT4 GPA2_4

#define PTT_SQL_OUTPUT1 GPA2_3
#define PTT_SQL_OUTPUT2 GPA2_2
#define PTT_SQL_OUTPUT3 GPA2_1
#define PTT_SQL_OUTPUT4 GPA2_0


#define LED_RX1 GPB2_0
#define LED_TX1 GPB2_1
#define LED_RX2 GPB2_2
#define LED_TX2 GPB2_3
#define LED_RX3 GPB2_7
#define LED_TX3 GPB2_6
#define LED_RX4 GPB2_5
#define LED_TX4 GPB2_4

#define DC2_DET 6
#define DC1_DET 7

#define GPIO_CODEC_RESET GPIO13
#define GPIO_LCD_RST GPIO10
#define GPIO_USB_RESET GPIO07
#define GPIO_DSP_RESET GPIO12
#define GPIO_DSP_SBOOT GPIO14

#define LEDON 1
#define LEDOFF 0

#define CONNECT2RADIO   0
#define CONNECT2VCS     1

#define RELAY_NC    0
#define RELAY_NO    1

#define TRXMODE_TXRX "TRx"
#define TRXMODE_TX "Tx"
#define TRXMODE_RX "Rx"
#define TRXMODE_RX0 "Rx"
#define TRXMODE_SEPARATE "Separate"

#define CONNECT_DISABLE false
#define CONNECT_ENABLE true
#define RXMODE_DISABLE false
#define RXMODE_ENABLE true
#define TXMODE_DISABLE false
#define TXMODE_ENABLE true

#define DCDETECTED 0

#define SERVER 1
#define CLIENT 2
#define CLIENT_RPT 3
#define SERVER_RPT 4

#define CONNECTED       "Connected "
#define DISCONNECTED    "Disconnected"
#define CONNECTING      "Connecting"
#define RECONNECTING    "Reconnecting"
#define CALLING         "Calling"
#define PTTRX           "TR"
#define PTTON           "Tx"
#define RXON            "Rx"
#define STANDBY         "--"
#define TRXSTANDBY      false
#define TRXMIAN         true

#define QSPIFLASH       true
#define JETSONSPI       false

#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#define FILESETTING "/home/orinnx/.config/ed137converter/settings.ini"

class iGate4x : public QObject
{
    Q_OBJECT
public:
    explicit iGate4x(QObject *parent = nullptr);
signals:

public slots:



private:
    int iCount = 0;
    SnmpStack *snmpStack;
    ChatServer *SocketServer;
    Database   *myDatabase;
    CS42448 *CODEC_CS42448;
    PCM3168A *CODEC_PCM3168A;
    bool enableDataLogger = true;
    ChatClient *dataLoggerServer;

//    MCP23008_I2C *i2cGpioExt;
    uint8_t allLEDStatus = 0xff;
    NetWorkMng *networking;
    GPIOClass *ppt_sql_output1;
    GPIOClass *ppt_sql_output2;
    GPIOClass *ppt_sql_output3;
    GPIOClass *ppt_sql_output4;
    GPIOClass *ppt_sql_input1;
    GPIOClass *ppt_sql_input2;
    GPIOClass *ppt_sql_input3;
    GPIOClass *ppt_sql_input4;
    GPIOClass *codecReset;
    GPIOClass *lcdReset;
    GPIOClass *hubusbReset;
    GPIOClass *dspReset;
    GPIOClass *dspBootSelect;


    GPIOClass *led_rx1;
    GPIOClass *led_rx2;
    GPIOClass *led_rx3;
    GPIOClass *led_rx4;

    GPIOClass *led_tx1;
    GPIOClass *led_tx2;
    GPIOClass *led_tx3;
    GPIOClass *led_tx4;

    OLEDDriver *oled;
    QTimer *loopTimer;
    QTimer *loopTruncatesyslog;
//    QTimer* timerCout;
    uint8_t currentSoftPhoneID;
    QTimer *loopTimerGetAudioLevel;
//    QMap<uint8_t, int> timeLeft;
    ADAU1467* SigmaFirmWareDownLoad;

    unsigned char VoIPVolumeOutCH1 = 10;
    unsigned char VoIPVolumeOutCH2 = 10;
    unsigned char VoIPVolumeOutCH3 = 10;
    unsigned char VoIPVolumeOutCH4 = 10;

    unsigned char VoIPVolumeInCH1 = 10;
    unsigned char VoIPVolumeInCH2 = 10;
    unsigned char VoIPVolumeInCH3 = 10;
    unsigned char VoIPVolumeInCH4 = 10;

    double VoIPVolumeOutDSPCH1 = 1; //0.0001 - 1  **** (-80)dB - 0dB 20logx
    double VoIPVolumeOutDSPCH2 = 1;
    double VoIPVolumeOutDSPCH3 = 1;
    double VoIPVolumeOutDSPCH4 = 1;

    bool psuDCInStatus = false;
    bool psuBattStatus = false;

    bool onIP1Changed = false;
    bool onIP2Changed = false;
    bool onIGate1InfoChange = false;
    bool onIGate2InfoChange = false;
    bool onIGate3InfoChange = false;
    bool onIGate4InfoChange = false;
    bool iGate1Run = false;
    bool iGate2Run = false;
    bool iGate3Run = false;
    bool iGate4Run = false;

    QString lastGetCurrentTime = "";
    QTimer *timer;
    int elapsedTime;

    struct SoftPhone
    {
        QString deviceName;
        int softPhoneID = 0;
        int channelID = 0;
        int sipPort = 5060;
        QString defaultEthernet = "bond0";
        QString sipUser = "";
        int keepAlivePeroid = 0;
        int rtpStartPort = 0;
        QWebSocket *socketClients = NULL;
        uint8_t WireConnectMode = CONNECT2RADIO;
        bool mainRadioTransmitterUsed = false;
        bool mainRadioReceiverUsed = false;
        uint8_t ServerClientMode = CLIENT;
        QStringList uriAllowList;
        quint8  txScheduler = 0;
        quint8  numConnection = 0;
        float   sidetone = 0;
        float   localSidetone = 0;
        bool   localSidetoneOn = false;
        bool    sqlAlwayOn = 0;
        bool    sqlActiveHigh = 0;
        bool softphoneRunning = false;
        bool softphoneBlink = false;
        uint8_t inputLevel = 0;
        uint8_t outputLevel = 0;
        uint8_t outputDSPLevel = 0;
        uint8_t trxMode = 0;
        bool  radioAutoInactive = 0;
        int radioMainStandby = 0;
        bool rxBestSignalEnable = 0;
        uint8_t groupMute = 0;
        uint8_t pttDelay = 0;
        bool tone_state = 0;
        float frequency =  151.4;
        int phase = 0;
        float level = 0;
        bool tone_server_state = 0;
        bool tone_server_ptt = 0;
        float tone_server_frequency =  600.0;
        int tone_server_phase = 0;
        float tone_server_level = 0.0;
        int tone_server_ptt_timeout = 30;
        QTimer* timerCout = nullptr;
        unsigned char VoIPVolumeIn_RadioTest = 10;

    };
//    struct Volume_RadioTest {
//        unsigned char VoIPVolumeIn_RadioTest = 10;
//    };

    double FIRfilter_stateON_INPUT[MOD_FIR1_COUNT] = {0.000389798143361774,0.000651936536801578,0.000993980344817791,
                                                      0.00144776801873279,0.00201772694241966,0.00267145045675678,
                                                      0.00333419990397108,0.0038884561268265,0.00417911130504812,
                                                      0.00402425886959341,0.00323087587718846,0.00161407392739293,
                                                      -0.000981905870412569,-0.00466309957113855,-0.00946787292219549,
                                                      -0.0153537202511853,-0.022190531275701,-0.0297614666016229,
                                                      -0.0377718566433368,-0.045865741450005,-0.0536488839932374,
                                                      -0.0607164008399475,-0.0666826369618758,-0.0712106236774104,
                                                      -0.0740384345617106,0.925,-0.0740384345617106,
                                                      -0.0712106236774104,-0.0666826369618758,-0.0607164008399475,
                                                      -0.0536488839932374,-0.045865741450005,-0.0377718566433368,
                                                      -0.0297614666016229,-0.022190531275701,-0.0153537202511853,
                                                      -0.00946787292219549,-0.00466309957113855,-0.000981905870412569,
                                                      0.00161407392739293,0.00323087587718846,0.00402425886959341,
                                                      0.00417911130504812,0.0038884561268265,0.00333419990397107,
                                                      0.00267145045675678,0.00201772694241966,0.00144776801873279,
                                                      0.00099398034481779,0.000651936536801578,0.000389798143361774};

    double FIRfilter_stateOFF_INPUT[MOD_FIR1_COUNT] = {0.0,0.0,0.0,
                                                 0.0,0.0,0.0,
                                                 0.0,0.0,0.0,
                                                 0.0,0.0,0.0,
                                                 0.0,0.0,0.0,
                                                 0.0,0.0,0.0,
                                                 0.0,0.0,0.0,
                                                 0.0,0.0,0.0,
                                                 0.0,0.0,0.0,
                                                 0.0,0.0,0.0,
                                                 0.0,0.0,0.0,
                                                 0.0,0.0,0.0,
                                                 0.0,0.0,0.0,
                                                 0.0,0.0,0.0,
                                                 0.0,0.0,0.0,
                                                 0.0,0.0,0.0,
                                                 0.0,0.0,1.0};


    double FIRfilter_stateON_OUTPUT[MOD_FIR2_COUNT] = {0.000389798143361774,0.000651936536801578,0.000993980344817791,
                                                      0.00144776801873279,0.00201772694241966,0.00267145045675678,
                                                      0.00333419990397108,0.0038884561268265,0.00417911130504812,
                                                      0.00402425886959341,0.00323087587718846,0.00161407392739293,
                                                      -0.000981905870412569,-0.00466309957113855,-0.00946787292219549,
                                                      -0.0153537202511853,-0.022190531275701,-0.0297614666016229,
                                                      -0.0377718566433368,-0.045865741450005,-0.0536488839932374,
                                                      -0.0607164008399475,-0.0666826369618758,-0.0712106236774104,
                                                      -0.0740384345617106,0.925,-0.0740384345617106,
                                                      -0.0712106236774104,-0.0666826369618758,-0.0607164008399475,
                                                      -0.0536488839932374,-0.045865741450005,-0.0377718566433368,
                                                      -0.0297614666016229,-0.022190531275701,-0.0153537202511853,
                                                      -0.00946787292219549,-0.00466309957113855,-0.000981905870412569,
                                                      0.00161407392739293,0.00323087587718846,0.00402425886959341,
                                                      0.00417911130504812,0.0038884561268265,0.00333419990397107,
                                                      0.00267145045675678,0.00201772694241966,0.00144776801873279,
                                                      0.00099398034481779,0.000651936536801578,0.000389798143361774};
    double FIRfilter_stateOFF_OUTPUT[MOD_FIR2_COUNT]={0.0,0.0,0.0,
                                                       0.0,0.0,0.0,
                                                       0.0,0.0,0.0,
                                                       0.0,0.0,0.0,
                                                       0.0,0.0,0.0,
                                                       0.0,0.0,0.0,
                                                       0.0,0.0,0.0,
                                                       0.0,0.0,0.0,
                                                       0.0,0.0,0.0,
                                                       0.0,0.0,0.0,
                                                       0.0,0.0,0.0,
                                                       0.0,0.0,0.0,
                                                       0.0,0.0,0.0,
                                                       0.0,0.0,0.0,
                                                       0.0,0.0,0.0,
                                                       0.0,0.0,0.0,
                                                       0.0,0.0,1.0};
//    double FIRfilter_stateON_OUTPUT[MOD_FIR2_COUNT] = {-0.00470527982145922,-0.0108062440957348,-0.0274154282022235,
//                                                       -0.0492884760488379,-0.0677798500970232,0.925,
//                                                       -0.0677798500970232,-0.0492884760488379,-0.0274154282022235
//                                                       -0.0108062440957348,-0.00470527982145922
//                                                      };
//    double FIRfilter_stateOFF_OUTPUT[MOD_FIR2_COUNT]={0.0,0.0,0.0,
//                                                       0.0,0.0,0.0,
//                                                       0.0,0.0,0.0,
//                                                       0.0,1.0};

    struct radio
    {
        int id = 0;
        QString callName = "";
        bool enable = false;
        QString trxMode = TRXMODE_TXRX;
        bool txModeEnable = TXMODE_DISABLE;
        bool rxModeEnable = RXMODE_DISABLE;

        QString ipAddress = "";
        int portAddr = 5060;
        bool pingOk = true;

        bool callState = false;
        QString callLastStateConn = "";
        QString uri = "";
        QString callNumberURI = "";

        QString connDuration = "0";
        QString currentTxRxStatus = STANDBY;
        bool currentSQLOn = false;

        int r2sPeriod = 200;
        int rtpPort = 5004;

        int pttLevel = 1;
        int frequency = 0;
        float vswr = 0.0;
        int rssi = 0;
        int SqlchThrhCarrier = 0;
        QString RfPower = "";
        QString radiostatus = "";
        int snmpProfile = 1;
        int radioControl = 0;

        bool call_hangup = false;

        bool mainTx = false;
        bool mainRx = false;

        bool rxVolumeActive = false;
        bool trxFailed = false;

        int getSNMPCount = 0;
        int pingCount = 25;
        int trxControl = 2;
        uint16_t snmpPort = 160;

    };

    struct igateInfo
    {
        int softPhoneID;
        int numConn;
        int numTxRx;
        bool mainTxPTTOn;
        bool mainRxSQLOn;
    };

    struct trx_callin
    {
        QString uri;
        QString duration;
        bool pttOn;
        bool sqlOn;
        bool callState;
    };

    struct ChannelInfo
    {        
        int softPhoneID = 0;
        int channelID = 0;
        int trxMode = 0;
        bool PTTOn = false;
        bool SQLOn = false;
        int numConn = 0;
        int numTxRx = 0;
        QString trxType = "Transceiver";
        radio   mainRadio01;
        radio   mainRadio02;
        radio   standbyRadio01;
        radio   standbyRadio02;

        //ClientInfo
        QString lastPttOn;
        QString pttURI;

        QList<trx_callin *> list_trxCallIn;
    };

    struct phyNetworkInfo{
        QString dhcpmethod = "";
        QString ipaddress = "";
        QString subnet = "";
        QString gateway = "";
        QString pridns = "";
        QString secdns = "";
        QString macAddress = "";
        QString serviceName = "";
        QString phyNetworkName = "";
        bool lan_ok = false;
    };

    struct radioData
    {
        int id = 0;
        QString radioName = "";
        QString trxMode = "";
        QString uri = "";
        QString ipAddress = "";
        int r2sPeriod = 200;
        int sipPort = 5060;
        QString frequency = "";
        bool visible = false;
    };

    struct channel
    {
        int softphoneID = 0;
        int id = 0;
        QString channelName;
        bool    visible = false;

        QString mainRadioType = TRXMODE_TXRX;
        int   mainRadio01ID = 0;
        int   mainRadio02ID = 0;
        QString mainRadioName = "";
        QString standbyRadioName;
        radio   mainRadio01;
        radio   mainRadio02;

        bool standbyEnable = false;
        QString standbyRadioType = TRXMODE_TXRX;
        int   standbyRadio01ID = 0;
        int   standbyRadio02ID = 0;
        radio   standbyRadio01;
        radio   standbyRadio02;

        QString currentTxRxStatus = STANDBY;
        bool currentMainRadioActive = true;

        bool mainRadioTransmitterUsed = TXMODE_DISABLE;
        bool mainRadioReceiverUsed = RXMODE_DISABLE;
        int trxModeEnable = 0; //trx = 1, rx-only=3, tx-only=2, disable all=0

        QString trxmode_show = "";
        int frequency_show = 0;
        int cnt_enable = 0;
        int main_cnt_enable = 0;
        int standby_cnt_enable = 0;
        int cnt_conn = 0;
        int main_cnt_conn = 0;
        int standby_cnt_conn = 0;

        int radiocheduler = 0;

        bool mainRadioEnable = false;
        bool standbyRadioEnable = false;
        bool mainAlwaysConnect = true;
        bool standbyAlwaysConnect = true;
    };
    struct ptt_timeout{
        bool tone_server_ptt = 0;
        bool tone_server_ptt_ch1 = 0;
        bool tone_server_ptt_ch2 = 0;
        bool tone_server_ptt_ch3 = 0;
        bool tone_server_ptt_ch4 = 0;
        int pttTimeout_ch1 = 0;
        int pttTimeout_ch2 = 0;
        int pttTimeout_ch3 = 0;
        int pttTimeout_ch4 = 0;
    };

    struct Set_tone{
        bool tone_state = 0;
        float level_tone_ch1 = 0.0;
        float level_tone_ch2 = 0.0;
        float level_tone_ch3 = 0.0;
        float level_tone_ch4 = 0.0;
        float level = 0.0;
        float tone = 0.0;
        float tone_frequency_ch1 = 0.0;
        float tone_frequency_ch2 = 0.0;
        float tone_frequency_ch3 = 0.0;
        float tone_frequency_ch4 = 0.0;
        float frequency_ch1 = 0.0;
        float frequency_ch2 = 0.0;
        float frequency_ch3 = 0.0;
        float frequency_ch4 = 0.0;
        float frequency = 0.0;
        int phase_tone_ch1 = 0;
        int phase_tone_ch2 = 0;
        int phase_tone_ch3 = 0;
        int phase_tone_ch4 = 0;
        int phase = 0;
    };
    struct Set_tone_server{
        bool tone_server_state = 0;
        float tone_server_frequency = 0.0;
        int  tone_server_phase = 0;
        float tone_server_level = 0.0;
    };

    void get_dcIn_status();


    int updateStatus = 0;
    bool foundfileupdate = false;

    int m_channelID = 0;

    bool ntp = false;
    QString timeLocation = "";
    QString ntpServer = "";
    QString SwVersion = SWVERSION;
    QString HwVersion = HWVERSION;
    int Networktimeout;
    bool FIRInput,FIROutput;
    int inviteMode = SERVER;
    int RadioControllerCH1 = 0;
    int RadioControllerCH2 = 0;
    int RadioControllerCH3 = 0;
    int RadioControllerCH4 = 0;

    ChatClient *newConnect;
    phyNetworkInfo eth0;
    phyNetworkInfo eth1;
    phyNetworkInfo bond0;   
    QList<SoftPhone *> m_softPhone_list;
    QList<ChannelInfo *> m_ChannelInfo_list;
    QList<channel *> channel_call_list;
    QList<radioData *> radio_list;
//    QList<Volume_RadioTest *> volume_in_radiotest;
    Set_tone set_tone;
    Set_tone_server tone_server;
    ptt_timeout ptt_timeout;
//    Set_tone set_tone;



    boost::mutex io_mutex;
    boost::atomic_bool m_IsEventThreadRunning;
    boost::shared_ptr<boost::thread> *m_EventThreadInstance;

    boost::mutex io_mutex2;
    boost::atomic_bool m_IsEventThreadRunning2;
    boost::shared_ptr<boost::thread> *m_EventThreadInstance2;

    bool spi2Busy = false;
    void startAllSoftphone(int softphoneID);
    void stopAllSoftphone(int softphoneID);
    int softPhoneIDCheck(int softPhoneID);

    int callChannelIDCheck(int softphoneID);
    int radioIDCheck(int radioID);
    void radioIDUsedCheck(int radioID);
    void radioIDRemoveUsedCheck(int radioID);
    int softPhoneIDCheckCHInfo(int softPhoneID);
    int checkCallInList(int ChannelInfoListIndex, QString uri);
    void set_Tx_LED(int softPhoneID,bool mainTxPTTOn);
    void set_Rx_LED(int softPhoneID,bool mainRxPTTOn);  
    ////////////////////////////////////////////////
    void handleTimeout(uint8_t softPhoneID);

    struct igateGroupInfo
    {
        int id;
        int softPhoneID;
        int iGateID;
        QString ipaddress;
        QString uriName;
        bool connStatus;
        bool SQLOn;
        bool PTTOn;
        float rssi;
        bool SockerServerConn;
        bool SockerClientConn;
        bool visible = true;
        int groupID;
        uint32_t hexIpAddress;
    };



    struct ChatClientList
    {
        int socketIndex;
        ChatClient *chatClient;
    };

    QList<igateGroupInfo *> igateGroupInfoList;
    int socketIndex = 0;
    QList<ChatClientList *> chatClientList;

    double level_in1;
    double level_in2;
    double level_in3;
    double level_in4;
    double level_out1;
    double level_out2;
    double level_out3;
    double level_out4;
    double log_level_in1;
    double log_level_in2;
    double log_level_in3;
    double log_level_in4;
    double log_level_out1;
    double log_level_out2;
    double log_level_out3;
    double log_level_out4;



    bool serverInit = false;

private slots:
    void getVuMeter();
    void getNetworkPage(QWebSocket *pSender);
    void getHomePage(uint8_t softPhoneID, QWebSocket *pSender);
    void getServerHomePage(uint8_t softPhoneID,QWebSocket *pSender);
    void getIGateGroupConfig(uint8_t softPhoneID,QWebSocket *pSender);
    void appendChannelData(uint8_t softPhoneID, int trxMode, bool mainTxPTTOn, bool mainRxSQLOn, int numConn, int numTxRx, QString trxType, QString main01IPAddr, QString main02IPAddr, QString standby01IPAddr, QString standby02IPAddr);
    void appendClientConnStatus(int softPhoneID, QString lastPttOn, QString trxStatus, QString pttURI, bool mainTxPTTOn, bool mainRxSQLOn, int numConn);
    void appendClientCallInStatus(int softPhoneID, QString uri, QString duration, bool pttOn, bool sqlOn, bool callState);
    void i2cGpioExtReadWriteThread();
    void setLED_txrx_value(uint8_t ledNum, bool status);
    void displayThread();
    void loopGetInfo();
    void newRediodata(uint8_t softPhoneID, bool tonestate, bool pttstate, int timeoutptt, float ServerFrequency, int ServerPhase, float ServerLevel);
    void onNewControler(int softPhoneID, int channelID, int sipPort, QString sipUser, int keepAlivePeroid, int rtpStartPort, uint8_t wireConnectMode, bool mainRadioTransmitterUsed,
                        bool mainRadioReceiverUsed, uint8_t ServerClientMode, quint8  txScheduler, quint8  numConnection, float sidetone, float localSidetone, bool sqlAlwayOn, bool sqlActiveHigh,
                        QString deviceName, int inputLevel, int outputLevel, bool radioAutoInactive, int radioMainStandby, QString defaultEthernet, bool rxBestSignalEnable,
                        uint8_t groupMute, uint8_t pttDelay, uint8_t outputDSPLevel, bool tone_state,float frequency,int phase,float level);
    void onComponentCompleted(int softPhoneId,QString sipuser, int channelId);

    void startSoftphone(int softPhoneID, QWebSocket *socket);
    void removeSoftphone(QWebSocket *newClient);
    void getCtrlInfoPage(int softPhoneID);

    void newradio(int radioID,QString radioName, QString trxMode, QString uri, QString ipAddress, int r2sPeriod, int sipPort, QString frequenicy);
    void newCallChannel(int softPhoneID, int channelID, int visible, QString channelName, QString mainRadioType, int mainRadio01ID, int mainRadio02ID,
                    int standbyEnable, QString standbyRadioType, int standbyRadio01ID, int standbyRadio02ID, QString mainRadioName, QString standbyRadioName, int trxModeEnable,
                        bool mainAlwaysConnect,   bool standbyAlwaysConnect,  bool mainRadioEnable, bool standbyRadioEnable);
//      TONE
    void updateInputTone(bool state, float Frequency ,int Phase , float Level , uint8_t softPhoneID);
    void updateServerTone(bool tonestate, bool pttstate,int timeoutptt, float ServerFrequency, int ServerPhase, float ServerLevel, uint8_t softPhoneID);
    void getTestRadioPage(QWebSocket *pSender);
    void saveTestRadioPage(int softPhoneID);
    void getRedioTestHomePage(uint8_t softPhoneID,QWebSocket *pSender);
    //    void write_FIR_coeffs(int fir_start_addr, int fir_filter_length, double* coefficients);

    void updateFIRfilter(bool firinput, bool firout);

    void updateInputGain(uint8_t value, uint8_t softPhoneID);
    void updateOutputGain(uint8_t value, uint8_t softPhoneID);
    void updateDSPOutputGain(uint8_t value, uint8_t softPhoneID);
    void updateRxBestSignalEnable(bool val, int softPhoneID);
    void updateGroupMute(uint8_t val, int softPhoneID);
    void updatePttDelay(uint8_t val, int softPhoneID);
    void updateURILits(QStringList urlList, int softPhoneID);
    void updateNumconn(int numconn, int softPhoneID);
    void updateSQLDefeat(bool sqlDefeat, int softPhoneID);
    void updateSQLActiveHigh(bool sqlActive, int softPhoneID);
    void updateRadioActive(bool radioAutoInactive, bool txMainRadioUsed, bool rxMainRadioUsed, int radioMainStandby, int softPhoneID);
    void updateRadioAutoInactive(bool value, int softPhoneID);
    void updateMainRadioReceiverUsed(bool value, int softPhoneID);
    void updateMainRadioTransmitterUsed(bool value, int softPhoneID);
    void updateRadioMainStandby(int value, int softPhoneID);
    void updateHostConfig(QString sipuser, int sipport, int keepaliveperoid, QString defaultEthernet, int softPhoneID);
    void updateDeviceName(int softPhoneID, QString deviceName);
    void updateControler(QString sipUser, int keepAlivePeroid, int sipPort, int rtpStartPort, int softPhoneID, int defaultEthernet);
    void updateChannelID(int channelID, int softPhoneID);
    void setRadioController(int RadioController, int softPhoneID);
    void Truncatesyslog();
//    void restartTimerSaveController(int softPhoneID);
    void saveController(int softPhoneID);

    void saveToneData(int softPhoneID);
    void restartSoftphone(int softPhoneID);
    void restartSoftphone();
    void dcInChanged();
    void updateSidetone(float sidetone,int softPhoneID);
    void updateLocalSidetone(float sidetone,int softPhoneID);

    void addNewChannel(QString channelName, QString mainRadioType, int mainRadio01ID, int mainRadio02ID, bool standbyEnable, QString standbyRadioType, int standbyRadio01ID, int standbyRadio02ID, QString mainRadioName, QString standbyRadioName);
    void updateChannel(QString channelName, QString mainRadioType, int mainRadio01ID, int mainRadio02ID, bool standbyEnable, QString standbyRadioType, int standbyRadio01ID, int standbyRadio02ID, QString mainRadioName, QString standbyRadioName, int channelID);
    void removeChannel(int channelID);
    void addNewRadio(QString radioName, QString trxMode, QString uri, QString ipAddress, int r2sPeriod,int sipPort, QString frequenicy);
    void updateRadio(QString radioName, QString trxMode, QString uri, QString ipAddress, int r2sPeriod, int sipPort, QString frequency, int radioID);
    void removeRadio(int radioID);

    void restartnetwork();
    void slotupdateNetwork(uint8_t DHCP, QString LocalAddress, QString Netmask, QString Gateway, QString DNS1, QString DNS2,QString phyNetworkName);
    void updateNetwork(uint8_t dhcpmethod, QString ipaddress, QString subnet, QString gateway, QString pridns, QString DNS2, QString phyNetworkName);
    void updateswitchInviteMode(int value);

    void myConfigurate();
    void getSystemPage(QWebSocket *webSender);
    void updateNTPServer(QString value);
    void setcurrentDatetime(QString dateTime);
    void setLocation(QString location);

    void scanFileUpdate();
    QStringList findFile();
    void updateFirmware();
    void rebootSystem();
    void adcValue(int channel, double value, QString message);

    void resetDSP();
    void DSPBootSelect(bool qspiflash);

    void onLocalSidetoneLoopbackChanged(bool localSidetoneLoopbackOn,int softPhoneID);
    void newIGateGroupMng(int id, int softPhoneID, int iGateID, QString ipaddress, QString uriName);
    void addIGateGroupMng(int softPhoneID,int iGateID,QString ipaddress,QString uriName);
    void deleteIGate(int softPhoneID, int id, int iGateID, QString ipaddress);
    void syncDeleteIGateAnotherGroup(int deleteIGateID, int fromIGateID, QString socketAddress);
    void syncDeleteIGateAnotherGroup(int deleteIGateID, int softPhoneID, QString ipaddress0, QString ipaddress1);
    void clearIGateGroupMng(int softPhoneID);
    void syncIGateGroup(QString toAddress, int softPhoneID, int iGateID, QString ipaddress, QString uriName);
    void deleteIGateGroup(QString toIPAddress,int softPhoneID);
    void deleteSocket(int socketIndex);
    double getAudioLevel(uint16_t x_address, uint16_t xlo_address);
    void loopGetAudioLevel();
    void systemrestore();
    void systembackup();
    void updateNetworkTimeout(int timeout);
    void initServer(int index);

private:
    signals:
    void cppCommand(QVariant jsonMsg);
    void onPTT_Sql_Changed(uint8_t ledNum, bool status);
    void onDCInChanged();



};

#endif // IGATE3X_H
