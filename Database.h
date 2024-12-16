#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QtSql>
#include <QString>
#include <QStringList>
class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QString dbName, QString user, QString password, QString host, QObject *parent = nullptr);
    bool database_createConnection();
    bool checkDefaultEthernetColumeExist();
    bool upgradeDatabase();
    bool passwordVerify(QString password);
    bool getAllChannelByUserID(int softPhoneID);
    bool addColumnRecorder();
    bool addColumnRxBestSignalEnable();
    bool ALTERTABLEcontrolerUri64Char();
    bool addColumnOutputDSPLevel();
    bool addColumnTone();
    bool addColumnToneServer();
    bool addColumnGroupMute();
    bool addColumnPttDelay();
    bool getNewControlerData();
    bool getNewRadioTestData();
    bool getAllRole(int softPhoneID);
    bool getRole(int RoleIDSelect, int softPhoneID);
    bool getAllChannelInRole(int softPhoneID);
    bool getChannelIDInRole();
    bool getCallChannel(int softPhoneID);
    bool getRadioData();
    QStringList getRadioByID(int radioID);
    void restartMysql();
    bool disableRadioInChannel(int radioID);
    bool userLogin(QString userName, QString password);
    int roleIDSelect = 0;
    bool updateOutputModeControler(uint8_t RelayOutputMode, int softPhoneID);
    bool updateRxBestSignalEnable(uint8_t val, int softPhoneID);
    bool updatePttDelay(uint8_t val, int softPhoneID);
    bool updateGroupMute(uint8_t val, int softPhoneID);
    QStringList getUriAllowList(uint8_t softPhoneID);
    int getInviteMode();
    bool updateInviteMode(int invitemode);
    bool updateSidetone(float sidetone,int softPhoneID);
    bool updateLocalSidetone(float sidetone,int softPhoneID);
    void getIGateGroupMng();
    void addIGateGroupMng(int softPhoneID,int iGateID,QString ipaddress,QString uriName);
    void deleteIGateGroup(int id, int softPhoneID);
    void truncateIGateGroup();
    void dumpDatabase();
    void restoreDatabase();
    void fixDatabase();
    void autoBackupTimerStart();
    int checkContollerTable();
    QString getChannelByID(int id);
signals:
    void mysqlError();
    void disconnectRadioByID(int radioID);
    void updateChannelByRadioID(int radioID);
    void  onNewControler(int softPhoneID, int channelID, int sipPort, QString sipUser, int keepAlivePeroid, int rtpStartPort, uint8_t wireConnectMode, bool mainRadioTransmitterUsed, bool mainRadioReceiverUsed,
                        uint8_t ServerClientMode,quint8  txScheduler, quint8  numConnection, float sidetone, float localSidetone, bool sqlAlwayOn, bool sqlActiveHigh, QString deviceName, int inputLevel, int outputLevel, bool radioAutoInactive,int radioMainStandby, QString defaultEthernet, bool rxBestSignalEnable,
                        uint8_t groupMute, uint8_t pttDelay, uint8_t outputDSPLevel, bool tone_state, float frequency, int phase , float level,
                        uint8_t recInputDSPLevel, uint8_t recOutputDSPLevel, QString recServerAddr1, QString recServerAddr2);
    void newradio(int radioID,QString radioName, QString trxMode, QString uri, QString ipAddress, int r2sPeriod,int sipPort, QString frequenicy);
    void newchannel(int channelID,int visible, QString channelName, QString mainRadioType, int mainRadio01ID, int mainRadio02ID,
                    int standbyEnable, QString standbyRadioType, int standbyRadio01ID, int standbyRadio02ID, QString mainRadioName, QString standbyRadioName, int trxModeEnable);
    void newCallChannel(int idInRole, int channelID,int visible, QString channelName, QString mainRadioType, int mainRadio01ID, int mainRadio02ID,
                    int standbyEnable, QString standbyRadioType, int standbyRadio01ID, int standbyRadio02ID, QString mainRadioName, QString standbyRadioName, int trxModeEnable,
                    bool mainAlwaysConnect,   bool standbyAlwaysConnect,  bool mainRadioEnable, bool standbyRadioEnable);
    void newRoleData(int roleID, QString roleName, QStringList channelSelectID);
    void newRoleName(int roleID, QString roleName);

    void newRediotestdata(uint8_t softPhoneID, bool tonestate, bool pttstate, int timeoutptt, float ServerFrequency, int ServerPhase, float ServerLevel);
    void newIGateGroupMng(int id, int softPhoneID, int iGateID, QString ipaddress, QString uriName);

    void backupcompleted(QString fileName);
    void restorecompleted();

    void newRediodata(uint8_t softPhoneID, bool tonestate, bool pttstate, int timeoutptt, float ServerFrequency, int ServerPhase, float ServerLevel);


public slots:
    void autoBackup();
    bool addNewRadio(QString radioName, QString trxMode, QString uri, QString ipAddress, int r2sPeriod, int sipPort, QString frequency);
    bool updateRadio(QString radioName, QString trxMode, QString uri, QString ipAddress, int r2sPeriod, int sipPort, QString frequency, int radioID);
    bool removeRadio(int ID);

    bool addNewChannel(QString channelName, QString mainRadioType, int mainRadio01ID, int mainRadio02ID, bool standbyEnable, QString standbyRadioType, int standbyRadio01ID, int standbyRadio02ID, QString mainRadioName, QString standbyRadioName);
    bool updateChannel(QString channelName, QString mainRadioType, int mainRadio01ID, int mainRadio02ID, bool standbyEnable, QString standbyRadioType, int standbyRadio01ID, int standbyRadio02ID, QString mainRadioName, QString standbyRadioName, int channelID);
    bool removeChannel(int channelID);

    bool updateChannelTxRxEnable(bool mainRadioTransmitterUsed, bool mainRadioReceiverUsed, int softPhoneID);
    bool updateControler(int sipPort, int rtpStartPort, int keepAlivePeroid, QString sipUser, int channelID, int softPhoneID, bool WireConnectMode, bool mainRadioReceiverUsed, bool mainRadioTransmitterUsed,
                         uint8_t trxMode, uint8_t ServerClientMode, uint8_t txScheduler, bool sqlActiveHigh, uint8_t numConnection, double sidetone, bool sqlAlwayOn, QString deviceName, uint8_t inputLevel,
                         uint8_t outputLevel, bool radioAutoInactive, int radioMainStandby, QString defaultEthernet, uint8_t outputDSPLevel, uint8_t recInputDSPLevel, uint8_t recOutputDSPLevel, QString recServerAddr1, QString recServerAddr2);
    bool updateAllowList(int softPhoneID, QStringList uriAllowList);

    bool updateControler(bool Tone_state,float Tone_frequency, int Tone_phase,int Tnone_level, int softPhoneID);

    bool updateRediotest(bool tonestate, bool pttstate, int timeoutptt, float ServerFrequency, int ServerPhase, float ServerLevel, uint8_t softPhoneID);

    bool removeRole(int roleID, int softPhoneID);
    bool addRole(QString roleName, QStringList channelIdSelect, int softPhoneID);
    bool updateRole(int roleID, QString roleName, QStringList channelIdSelect, int softPhoneID);

private:
    QSqlDatabase db;
    bool verifyMac();
    QString getPassword();
    QString _dbName;
    QString _user = "root";
    QTimer *autoBackupDateTimer;
};

#endif // DATABASE_H
