#include "Database.h"
#include <QDateTime>
#include <QStringList>
#include <QString>
#include <QProcess>
Database::Database(QString dbName, QString user, QString password, QString host,QObject *parent) :
    QObject(parent)
{
    _dbName = dbName;
    QString mysqlcheck = QString("mysqlcheck -u%1 -p%2 %3").arg(user).arg(password).arg(dbName);
    system(mysqlcheck.toStdString().c_str());
    QThread::sleep(0.5);
//    QString myisamchk = QString("myisamchk -r /var/lib/mysql/%1/%2").arg(dbName).arg("controler");
//    system(myisamchk.toStdString().c_str());
    QThread::sleep(0.5);
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(host);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(password);
    autoBackupDateTimer = new QTimer();
    connect(autoBackupDateTimer,SIGNAL(timeout()),this,SLOT(autoBackup()));
}
void Database::dumpDatabase()
{
    system("rm -f /var/www/html/database/igate4ch_backup*.bin");
    system("rm -f /tmp/database/igate4ch_backup*.bin");
    system("sync");
    system("mkdir -p /var/www/html/database");
    system("mkdir -p /tmp/database");
    system("chown -R www-data:www-data /var/www/html/database");
    system("rm /var/www/html/database/igate4ch_backup*.bin");
    QString commandRestorSQL = QString("mysqldump -uroot -p %1 > /tmp/database/igate4ch_backup.bin").arg(_dbName);
    system(commandRestorSQL.toStdString().c_str());
    QString fileName = QString("/var/www/html/database/igate4ch_backup%1.bin").arg(QDateTime::currentDateTime().toSecsSinceEpoch());
    QString tarCommand = QString("tar -cf %1 /home/orinnx/.config/ed137converter/settings.ini /tmp/database/igate4ch_backup.bin").arg(fileName);
    system(tarCommand.toStdString().c_str());
    system("chown -R www-data:www-data /var/www/html/database/*");
    system("sync");
    emit backupcompleted(fileName);
}
int Database::checkContollerTable()
{
    int contillerNum = 0;
    QString query = QString("SELECT ServerClientMode FROM controler ORDER BY id");
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return contillerNum;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    else
    {
        while (qry.next()) {
            contillerNum     += 1;
        }
    }
    db.close();
    return  contillerNum;
}
void Database::fixDatabase()
{
    system("systemctl restart apache2");
    QString commandRestorSQL = QString("mysql -uroot %1 < /etc/autobackup.sql").arg(_dbName);
    system(commandRestorSQL.toStdString().c_str());
    system("sync");
    qDebug() << "fixDatabase";
}
void Database::autoBackupTimerStart()
{
    autoBackupDateTimer->stop();
    autoBackupDateTimer->start(3000);
}
void Database::autoBackup()
{
    autoBackupDateTimer->stop();
    QString CurrentdateTime = QDateTime::currentDateTime().toString("yyMMdd hhmmss");
    QString commandRestorSQL = QString("mysqldump -uroot %1 > /etc/autobackup.sql").arg(_dbName);
    system(commandRestorSQL.toStdString().c_str());
    system("sync");
    qDebug() << CurrentdateTime << "Auto Backup";
}
void Database::restoreDatabase()
{
    system("systemctl restart apache2");
    system("tar -xf /var/www/html/uploads/database.tar -C /");
    QString commandRestorSQL = QString("mysql -uroot %1 < /tmp/database/igate4ch_backup.bin").arg(_dbName);
    system(commandRestorSQL.toStdString().c_str());
    system("rm -f /var/www/html/database/*");
    system("rm -f /var/www/html/uploads/database.tar");
    system("sync");
    emit restorecompleted();
}
void Database::restartMysql(){
    system("systemctl stop mysqld");
    system("systemctl start mysqld");

    qDebug() << "Restart MySQL";
}

bool Database::passwordVerify(QString password){
    QString query = QString("SELECT password FROM hashlet LIMIT 1");
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QString hashPassword;
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }else{
        while (qry.next()) {
            hashPassword = qry.value(0).toString();
        }
    }
    db.close();
    QString prog = "/bin/bash";//shell
    QStringList arguments;
    QProcess getAddressProcess;
    QString output;

    arguments.clear();
    arguments << "-c" << QString("echo %1 | hashlet hmac").arg(password);
    getAddressProcess.start(prog , arguments);
    getAddressProcess.waitForFinished(3000);
    output = getAddressProcess.readAll();
    if (output == "") {
        qDebug() << "output == \"\"";
        return false;
    }else if(!output.contains(hashPassword)){
        qDebug() << "output != hashPassword";
        return false;
    }

    system("mkdir -p /etc/ed137");
    if (verifyMac()){
        qDebug() << "mac true";


        if (hashPassword != ""){
            QString filename = "/etc/ed137/checkpass.sh";
            QString data = QString("#!/bin/bash\n"
                                   "su - pi -s /bin/bash -c \"echo $1 | hashlet offline-hmac -r $2\"\n"
                                   "echo $? > /etc/ed137/checkpass\n");
            system("mkdir -p /etc/ed137");

            QByteArray dataAyyay(data.toLocal8Bit());
            QFile file(filename);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream out(&file);
            out << dataAyyay;
            file.close();
            arguments.clear();
            arguments << "-c" << QString("sh /etc/ed137/checkpass.sh %1 %2").arg(password).arg(hashPassword);
            getAddressProcess.start(prog , arguments);
            getAddressProcess.waitForFinished(-1);
            output = getAddressProcess.readAll();
            qDebug() << output;

            arguments.clear();
            arguments << "-c" << QString("cat /etc/ed137/checkpass");
            getAddressProcess.start(prog , arguments);
            getAddressProcess.waitForFinished(-1);
            output = getAddressProcess.readAll();
            qDebug() << output;
            system("rm -r /etc/ed137");
            if (output.contains("0\n")){
                return true;
            }
            return false;
        }

    }else{
        qDebug() << "mac false";
    }
    system("rm -r /etc/ed137");
    return false;
}

bool Database::verifyMac(){
    QString query = QString("SELECT mac, challenge FROM hashlet LIMIT 1");
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QString mac, challenge;
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }else{
        while (qry.next()) {
            mac = qry.value(0).toString();
            challenge = qry.value(1).toString();
        }
    }
    db.close();

    QString prog = "/bin/bash";//shell
    QStringList arguments;
    QProcess getAddressProcess;
    QString output;

    QString filename = "/etc/ed137/checkmac.sh";
    QString data = QString("#!/bin/bash\n"
                           "su - pi -s /bin/bash -c \"hashlet offline-verify -c $1 -r $2\"\n"
                           "echo $? > /etc/ed137/checkmac\n");
    system("mkdir -p /etc/ed137");
    QByteArray dataAyyay(data.toLocal8Bit());
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << dataAyyay;
    file.close();

    arguments << "-c" << QString("sh /etc/ed137/checkmac.sh %1 %2").arg(challenge).arg(mac);
    getAddressProcess.start(prog , arguments);
    getAddressProcess.waitForFinished(1000);
    output = getAddressProcess.readAll();

    arguments.clear();
    arguments << "-c" << QString("cat /etc/ed137/checkmac");
    getAddressProcess.start(prog , arguments);
    getAddressProcess.waitForFinished(1000);
    output = getAddressProcess.readAll();
    arguments.clear();

    if (output.contains("0\n"))
        return true;
    return false;
}

bool Database::database_createConnection()
{
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    db.close();
    qDebug() << "Database connected";    

    upgradeDatabase();
    return true;
}
bool Database::updateControler(int sipPort,int rtpStartPort,int keepAlivePeroid,QString sipUser,int channelID,int softPhoneID,bool WireConnectMode,bool mainRadioReceiverUsed,bool mainRadioTransmitterUsed,uint8_t trxMode,
                               uint8_t ServerClientMode,uint8_t txScheduler,bool sqlActiveHigh,uint8_t numConnection,double sidetone,bool sqlAlwayOn,QString deviceName,uint8_t inputLevel,uint8_t outputLevel,bool radioAutoInactive,
                               int radioMainStandby, QString defaultEthernet, uint8_t outputDSPLevel, uint8_t recInputDSPLevel, uint8_t recOutputDSPLevel, QString recServerAddr1, QString recServerAddr2)
{
    QString query = QString("UPDATE controler SET sipPort=%1 , rtpStartPort=%2 , keepAlivePeroid=%3 , sipUser='%4' , channelID=%5 , WireConnectMode=%6 , mainRadioReceiverUsed=%7 , mainRadioTransmitterUsed=%8 ,"
                            " trxMode=%9 , ServerClientMode=%10 , txScheduler=%11 , sqlActiveHigh=%12 , numConnection=%13 ,"
                            "sidetone=%14 , sqlAlwayOn=%15 , deviceName='%16' , inputLevel=%17 , outputLevel=%18  , "
                            "radioAutoInactive=%19, radioMainStandby=%20, defaultEthernet='%21', outputDSPLevel=%22, "
                            "recorderAddress1='%23', recorderAddress2='%24', recInputLevel='%25', recOutputLevel=%26 "
                            "WHERE softPhoneID=%27")
            .arg(sipPort).arg(rtpStartPort).arg(keepAlivePeroid).arg(sipUser).arg(channelID).arg(WireConnectMode).arg(mainRadioReceiverUsed).arg(mainRadioTransmitterUsed)
            .arg(trxMode).arg(ServerClientMode).arg(txScheduler).arg(sqlActiveHigh).arg(numConnection).arg(sidetone).arg(sqlAlwayOn).arg(deviceName).arg(inputLevel)
            .arg(outputLevel).arg(radioAutoInactive).arg(radioMainStandby).arg(defaultEthernet).arg(outputDSPLevel)
            .arg(recServerAddr1).arg(recServerAddr2).arg(recInputDSPLevel).arg(recOutputDSPLevel)
            .arg(softPhoneID);
//    qDebug() << query;
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
        printf("***********************SQL Error*****************\n%s\n",query.toStdString().c_str());
    }
    db.close();        
    autoBackupTimerStart();
    return true;
}
bool Database::updateControler(bool Tone_state,float Tone_frequency, int Tone_phase,int Tnone_level, int softPhoneID)
{
        QString query = QString("UPDATE controler SET outputToneState=%1, outputToneFrequency=%2 ,outputTonePhase=%3, outputToneLevel=%4  WHERE softPhoneID=%5")
                                .arg(Tone_state).arg(Tone_frequency).arg(Tone_phase).arg(Tnone_level).arg(softPhoneID);
//        qDebug() << query;
        if (!db.open()) {
            qDebug() << "database error! database can not open.";
            emit mysqlError();
            restartMysql();
            return false;
        }
        QSqlQuery qry;
        qry.prepare(query);
        if (!qry.exec()){
            qDebug() << qry.lastError();
            printf("***********************SQL Error*****************\n%s\n",query.toStdString().c_str());
        }
        db.close();
        autoBackupTimerStart();
        return true;
}
bool Database::updateRediotest(bool tonestate, bool pttstate, int timeoutptt, float ServerFrequency, int ServerPhase, float ServerLevel, uint8_t softPhoneID)
{

//    QString query = "SELECT ToneServerState,ToneServerPTT,ToneServerTimeout,ToneServerFrequency,ToneServerPhase,ToneServerLevel FROM controler LIMIT 1";
    QString query = QString("UPDATE controler SET ToneServerState=%1, ToneServerPTT=%2, ToneServerTimeout=%3, ToneServerFrequency=%4, ToneServerPhase=%5, ToneServerLevel=%6  WHERE softPhoneID=%7")
                            .arg(tonestate).arg(pttstate).arg(timeoutptt).arg(ServerFrequency).arg(ServerPhase).arg(ServerLevel).arg(softPhoneID);
//        qDebug() << query;
        if (!db.open()) {
            qDebug() << "database error! database can not open.";
            emit mysqlError();
            restartMysql();
            return false;
        }
        QSqlQuery qry;
        qry.prepare(query);
        if (!qry.exec()){
            qDebug() << qry.lastError();
            printf("***********************SQL Error*****************\n%s\n",query.toStdString().c_str());
        }
        db.close();
        autoBackupTimerStart();
        return true;
}

bool Database::updateAllowList(int softPhoneID, QStringList uriAllowList)
{
    QString query;

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;

    query = QString("DELETE FROM allowList WHERE softPhoneID=%1").arg(softPhoneID);
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    Q_FOREACH(QString uri, uriAllowList)
    {
        query = QString("INSERT INTO allowList (uri, softPhoneID) VALUES ('%1', %2)").arg(uri).arg(softPhoneID);
        qry.prepare(query);
        if (!qry.exec()){
            qDebug() << qry.lastError();
        }
    }

    db.close();
    return  true;
}

bool Database::updateOutputModeControler(uint8_t RelayOutputMode, int softPhoneID)
{
    QString query = QString("UPDATE controler SET RelayOutputMode='%1' WHERE softPhoneID=%2")
            .arg(RelayOutputMode).arg(softPhoneID);
//    qDebug() << query;
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();
    autoBackupTimerStart();
    return true;
}

bool Database::updateRxBestSignalEnable(uint8_t val, int softPhoneID)
{
    QString query = QString("UPDATE controler SET rxBestSignalEnable='%1' WHERE softPhoneID=%2")
            .arg(val).arg(softPhoneID);
//    qDebug() << query;
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();
    autoBackupTimerStart();
    return true;
}
bool Database::updatePttDelay(uint8_t val, int softPhoneID)
{
    QString query = QString("UPDATE controler SET pttDelay='%1' WHERE softPhoneID=%2")
            .arg(val).arg(softPhoneID);
//    qDebug() << query;
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();
    autoBackupTimerStart();
    return true;
}
bool Database::updateGroupMute(uint8_t val, int softPhoneID)
{
    QString query = QString("UPDATE controler SET groupMute='%1' WHERE softPhoneID=%2")
            .arg(val).arg(softPhoneID);
//    qDebug() << query;
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();
    autoBackupTimerStart();
    return true;
}

bool Database::addNewChannel(QString channelName, QString mainRadioType, int mainRadio01ID, int mainRadio02ID, bool standbyEnable, QString standbyRadioType, int standbyRadio01ID, int standbyRadio02ID, QString mainRadioName, QString standbyRadioName)
{
    QString query = QString("INSERT INTO channel (channelName, mainRadioType, mainRadio01ID, mainRadio02ID, standbyEnable, standbyRadioType, standbyRadio01ID, standbyRadio02ID, mainRadioName, standbyRadioName, visible) "
                            "VALUES ('%1', '%2', %3, %4, %5, '%6', %7, %8, '%9', '%10', 1)")
            .arg(channelName).arg(mainRadioType).arg(mainRadio01ID).arg(mainRadio02ID).arg(standbyEnable).arg(standbyRadioType).arg(standbyRadio01ID).arg(standbyRadio02ID).arg(mainRadioName).arg(standbyRadioName);
//    qDebug() << query;
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();
    return true;
}
bool Database::updateChannelTxRxEnable(bool mainRadioTransmitterUsed, bool mainRadioReceiverUsed, int softPhoneID)
{
    QString query = QString("UPDATE controler SET mainRadioTransmitterUsed=%1, mainRadioReceiverUsed=%2 WHERE softPhoneID=%3")
            .arg(mainRadioTransmitterUsed).arg(mainRadioReceiverUsed).arg(softPhoneID);
//    qDebug() << query;
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();
    autoBackupTimerStart();
    return true;
}
bool Database::updateChannel(QString channelName, QString mainRadioType, int mainRadio01ID, int mainRadio02ID, bool standbyEnable, QString standbyRadioType, int standbyRadio01ID, int standbyRadio02ID, QString mainRadioName, QString standbyRadioName, int channelID)
{
    QString query = QString("UPDATE channel SET channelName='%1', mainRadioType='%2', mainRadio01ID=%3, mainRadio02ID=%4, standbyEnable=%5, standbyRadioType='%6',"
                            " standbyRadio01ID=%7, standbyRadio02ID=%8, mainRadioName='%9', standbyRadioName='%10' WHERE id=%11")
            .arg(channelName).arg(mainRadioType).arg(mainRadio01ID).arg(mainRadio02ID).arg(standbyEnable).arg(standbyRadioType)
            .arg(standbyRadio01ID).arg(standbyRadio02ID).arg(mainRadioName).arg(standbyRadioName).arg(channelID);
//    qDebug() << query;
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();
    return true;
}

bool Database::userLogin(QString userName, QString password)
{
    QString sipUser = "iGate";
    int UserID = 0;
    QString query = QString("SELECT UserID FROM member WHERE username='%1' AND password= PASSWORD('%2') AND Status='GUI' LIMIT 1").arg(userName).arg(password);
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    else
    {
        while (qry.next()) {
            UserID         = qry.value(0).toInt();
        }
    }
    db.close();

    if (UserID != 0)
        return true;
    return  false;
}

QStringList Database::getUriAllowList(uint8_t softPhoneID)
{
    QStringList uriAllowList;
    QString query = QString("SELECT uri FROM allowList WHERE softPhoneID=%1 ORDER BY id ASC LIMIT 8").arg(softPhoneID);
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return uriAllowList;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    else
    {
        while (qry.next()) {
            uriAllowList     << qry.value(0).toString();
        }
    }
    db.close();
    if (uriAllowList.size() < 8)
    {
        for (int i = uriAllowList.size();i < 8; i++) {
            uriAllowList << "";
        }
    }
    return  uriAllowList;
}
bool Database::updateSidetone(float sidetone,int softPhoneID)
{
    QString query = QString("UPDATE controler SET sidetone=%1 WHERE softPhoneID=%2").arg(sidetone).arg(softPhoneID);
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    //qDebug() << query;
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();
    autoBackupTimerStart();
    return true;
}
bool Database::updateLocalSidetone(float sidetone,int softPhoneID)
{
    QString query = QString("UPDATE controler SET localSidetone=%1 WHERE softPhoneID=%2").arg(sidetone).arg(softPhoneID);
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    //qDebug() << query;
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();
    autoBackupTimerStart();
    return true;
}
int Database::getInviteMode()
{
    int inviteMode = 0;
    QString query = QString("SELECT ServerClientMode FROM controler ORDER BY id ASC LIMIT 1");
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return inviteMode;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    else
    {
        while (qry.next()) {
            inviteMode     = qry.value(0).toInt();
        }
    }
    db.close();
    return  inviteMode;
}
void Database::getIGateGroupMng()
{
    int id;
    int softPhoneID;
    int iGateID;
    QString ipaddress;
    QString uriName;
    QString query = QString("SELECT id, softphoneID, iGateID, ipAddress, uriName FROM iGateGroupMng ORDER BY id ASC");
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    else
    {
        while (qry.next())
        {
            id              = qry.value(0).toInt();
            softPhoneID     = qry.value(1).toInt();
            iGateID         = qry.value(2).toInt();
            ipaddress       = qry.value(3).toString();
            uriName         = qry.value(4).toString();
            emit newIGateGroupMng( id,  softPhoneID,  iGateID,  ipaddress,  uriName);
        }
    }
    db.close();
}

void Database::addIGateGroupMng(int softPhoneID,int iGateID,QString ipaddress,QString uriName)
{
    QString query = QString("INSERT INTO iGateGroupMng (softphoneID, iGateID, ipAddress, uriName) VALUES (%1, %2, '%3', '%4')")
            .arg(softPhoneID).arg(iGateID).arg(ipaddress).arg(uriName);
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return;
    }
//    qDebug() << query;
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();
}

void Database::deleteIGateGroup(int id, int softPhoneID)
{
    QString query;

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return;
    }
    QSqlQuery qry;

    if (id > 0)
        query = QString("DELETE FROM iGateGroupMng WHERE id=%1 AND softPhoneID=%2").arg(id).arg(softPhoneID);
    else
        query = QString("DELETE FROM iGateGroupMng WHERE softPhoneID=%1").arg(softPhoneID);

    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    db.close();
}
void Database::truncateIGateGroup()
{
    QString query;

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return;
    }
    QSqlQuery qry;

    query = QString("TRUNCATE TABLE iGateGroupMng");

    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    db.close();
}

bool Database::updateInviteMode(int invitemode)
{
    QString query = QString("UPDATE controler SET ServerClientMode=%1").arg(invitemode);
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    //qDebug() << query;
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();
    autoBackupTimerStart();
    return true;
}
bool Database::ALTERTABLEcontrolerUri64Char()
{
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    QString query = QString("ALTER TABLE `controler` CHANGE `sipUser` `sipUser` VARCHAR(64)  CHARACTER SET utf8mb3  COLLATE utf8mb3_general_ci  NOT NULL  DEFAULT 'iGate'");
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    query = QString("ALTER TABLE `radio` CHANGE `uri` `uri` VARCHAR(64)  CHARACTER SET utf8mb3  COLLATE utf8mb3_general_ci  NOT NULL  DEFAULT 'uri'");
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    query = QString("ALTER TABLE `radio` CHANGE `name` `name` VARCHAR(64)  CHARACTER SET utf8mb3  COLLATE utf8mb3_general_ci  NOT NULL  DEFAULT 'Radio'");
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();
    autoBackupTimerStart();
    return true;
}
bool Database::addColumnOutputDSPLevel()
{
    bool notExist = false;
    QString query = "SELECT outputDSPLevel FROM controler LIMIT 1";

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    //qDebug() << query;
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
        notExist = true;
    }

    if (notExist){
        query = QString("ALTER TABLE controler ADD COLUMN outputDSPLevel INT(3) NOT NULL DEFAULT 0");
        qry.prepare(query);
        if (!qry.exec()){
            qDebug() << qry.lastError();
        }

        query = QString("UPDATE controler SET outputLevel=0");
    }
    db.close();
    autoBackupTimerStart();
    return true;
}
bool Database::addColumnTone()
{
    bool notExist = false;
    QString query = "SELECT outputToneState,outputToneFrequency,outputTonePhase,outputToneLevel FROM controler LIMIT 1";

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);

    if (!qry.exec()){
        qDebug() << qry.lastError();
        notExist = true;
    }

    if (notExist){
        query = QString("ALTER TABLE controler ADD COLUMN outputToneState TINYINT(1) NOT NULL DEFAULT 0,"
                        "ADD COLUMN outputToneFrequency DOUBLE(5,1) NOT NULL DEFAULT 0,"
                        "ADD COLUMN outputTonePhase INT NOT NULL DEFAULT 0,"
                        "ADD COLUMN outputToneLevel INT NOT NULL DEFAULT 0");
        qDebug() <<"query"<<query;
        qry.prepare(query);
        if (!qry.exec()){
            qDebug() << qry.lastError();
        }
    }
    db.close();
    autoBackupTimerStart();
    return true;
}

bool Database::addColumnToneServer()
{
    bool notExist = false;
    QString query = "SELECT ToneServerState,ToneServerPTT,ToneServerTimeout,ToneServerFrequency,ToneServerPhase,ToneServerLevel FROM controler LIMIT 1";

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);

    if (!qry.exec()){
        qDebug() << qry.lastError();
        notExist = true;
    }

    if (notExist){
        query = QString("ALTER TABLE controler ADD COLUMN ToneServerState TINYINT(1) NOT NULL DEFAULT 0,"
                        "ADD COLUMN ToneServerPTT TINYINT(1) NOT NULL DEFAULT 0,"
                        "ADD COLUMN ToneServerTimeout INT NOT NULL DEFAULT 30,"
                        "ADD COLUMN ToneServerFrequency DOUBLE(5,1) NOT NULL DEFAULT 1000,"
                        "ADD COLUMN ToneServerPhase INT NOT NULL DEFAULT 0,"
                        "ADD COLUMN ToneServerLevel INT NOT NULL DEFAULT -20");
        qDebug() <<"query"<<query;
        qry.prepare(query);
        if (!qry.exec()){
            qDebug() << qry.lastError();
        }
    }
    db.close();
    autoBackupTimerStart();
    return true;
}
bool Database::addColumnRecorder()
{
    bool notExist = false;
    QString query = "SELECT recorderAddress1 FROM controler LIMIT 1";

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    //qDebug() << query;
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
        notExist = true;
    }

    if (notExist){
        query = QString("ALTER TABLE controler ADD COLUMN ("
                        "recorderAddress1 VARCHAR(32) NOT NULL DEFAULT '', "
                        "recorderAddress2 VARCHAR(32) NOT NULL DEFAULT '', "
                        "recInputLevel DOUBLE(5,2) NOT NULL DEFAULT 0.0, "
                        "recOutputLevel DOUBLE(5,2) NOT NULL DEFAULT 0.0 "
                        ")");
        qry.prepare(query);
        if (!qry.exec()){
            qDebug() << qry.lastError();
        }
    }
    db.close();
    return true;
}
bool Database::addColumnRxBestSignalEnable()
{
    bool notExist = false;
    QString query = "SELECT rxBestSignalEnable FROM controler LIMIT 1";

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    //qDebug() << query;
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
        notExist = true;
    }

    if (notExist){
        query = QString("ALTER TABLE controler ADD COLUMN rxBestSignalEnable TINYINT(1) NOT NULL DEFAULT 0");
        qry.prepare(query);
        if (!qry.exec()){
            qDebug() << qry.lastError();
        }
    }
    db.close();
    return true;
}

bool Database::addColumnPttDelay()
{
    bool notExist = false;
    QString query = "SELECT pttDelay FROM controler LIMIT 1";

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    //qDebug() << query;
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
        notExist = true;
    }

    if (notExist){
        query = QString("ALTER TABLE controler ADD COLUMN pttDelay TINYINT(2) NOT NULL DEFAULT 0");
        qry.prepare(query);
        if (!qry.exec()){
            qDebug() << qry.lastError();
        }
    }
    db.close();
    return true;
}
bool Database::addColumnGroupMute()
{
    bool notExist = false;
    QString query = "SELECT groupMute FROM controler LIMIT 1";

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    //qDebug() << query;
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
        notExist = true;
    }

    if (notExist){
        query = QString("ALTER TABLE controler ADD COLUMN groupMute TINYINT(1) NOT NULL DEFAULT 0");
        qry.prepare(query);
        if (!qry.exec()){
            qDebug() << qry.lastError();
        }
    }
    db.close();
    return true;
}
bool Database::getNewRadioTestData()
{
    int softPhoneID = 0;
    bool tone_server_state = 0;
    bool tone_server_ptt = 0;
    int  server_timeout_ptt = 0;
    float tone_server_frequency = 0.0;
    int  tone_server_phase = 0;
    float tone_server_level = 0.0;
    QString query = QString("SELECT id, ToneServerState, ToneServerPTT, ToneServerTimeout, ToneServerFrequency, ToneServerPhase, ToneServerLevel FROM controler ORDER BY id ASC LIMIT 8;");
    if (!db.open())
    {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    else
    {
        while (qry.next()) {
            softPhoneID     = qry.value(0).toInt();
            tone_server_state  = qry.value(1).toInt();
            tone_server_ptt  = qry.value(2).toInt();
            server_timeout_ptt  = qry.value(3).toInt();
            tone_server_frequency = qry.value(4).toDouble();
            tone_server_phase    = qry.value(5).toInt();
            tone_server_level    = qry.value(6).toDouble();
            qDebug() << "newRediodata" << tone_server_frequency << "Level" << tone_server_level << "softPhoneID" << softPhoneID;
            emit newRediodata(softPhoneID, tone_server_state, tone_server_ptt, server_timeout_ptt , tone_server_frequency,tone_server_phase, tone_server_level);
        }
    }
    db.close();
    return true;
}
bool Database::getNewControlerData()
{
    int softPhoneID = 0;
    int channelID = 0;
    int sipPort = 5060;
    bool mainRadioTransmitterUsed = false;
    bool mainRadioReceiverUsed = false;
    QString sipUser = "";
    int keepAlivePeroid = 200;
    int rtpStartPort = 5002;
    uint8_t ServerClientMode = 0;
    uint8_t WireConnectMode = 0;//NomalOpen_Close
    quint8  txScheduler;
    quint8  numConnection;
    double   sidetone;
    double   localSidetone;
    bool    sqlAlwayOn;
    bool    sqlActiveHigh;
    QString deviceName;
    int inputLevel;
    int outputLevel;
    bool radioAutoInactive;
    int radioMainStandby;

    bool outputToneState;
    int outputToneFrequency ;
    float outputTonePhase;
    float outputToneLevel;

    QString defaultEthernet;
    bool rxBestSignalEnable; // FW V2
    uint8_t groupMute;
    uint8_t pttDelay;
    uint8_t outputDSPLevel;
    uint8_t recInputDSPLevel;
    uint8_t recOutputDSPLevel;
    QString recServerAddr1;
    QString recServerAddr2;
    QString query = QString("SELECT id, sipUser, sipPort, keepAlivePeroid, rtpStartPort, channelID, WireConnectMode, mainRadioTransmitterUsed, mainRadioReceiverUsed, ServerClientMode,txScheduler, numConnection, sidetone, sqlAlwayOn, sqlActiveHigh, deviceName, inputLevel, outputLevel, radioAutoInactive, radioMainStandby,defaultEthernet, rxBestSignalEnable, localSidetone, groupMute, pttDelay, outputDSPLevel,outputToneState, outputToneFrequency, outputTonePhase ,outputToneLevel, "
                            "recorderAddress1, recorderAddress2, recInputLevel, recOutputLevel FROM controler ORDER BY id ASC LIMIT 8;");
    if (!db.open())
    {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    else
    {
        while (qry.next()) {
            softPhoneID     = qry.value(0).toInt();
            sipUser         = qry.value(1).toString();
            sipPort         = qry.value(2).toInt();
            keepAlivePeroid = qry.value(3).toInt();
            rtpStartPort    = qry.value(4).toInt();
            channelID       = qry.value(5).toInt();
            WireConnectMode = qry.value(6).toInt();
            mainRadioTransmitterUsed  = qry.value(7).toInt() != 0;
            mainRadioReceiverUsed   = qry.value(8).toInt() != 0;
            ServerClientMode= qry.value(9).toInt();
            txScheduler     = qry.value(10).toInt();
            numConnection   = qry.value(11).toInt();
            sidetone        = qry.value(12).toDouble();
            sqlAlwayOn      = qry.value(13).toInt() != 0;
            sqlActiveHigh   = qry.value(14).toInt() != 0;
            deviceName      = qry.value(15).toString();
            inputLevel      = qry.value(16).toInt();
            outputLevel     = qry.value(17).toInt();
            radioAutoInactive = qry.value(18).toInt() != 0;
            radioMainStandby  = qry.value(19).toInt();
            defaultEthernet = qry.value(20).toString();
            rxBestSignalEnable = qry.value(21).toInt() != 0;
            localSidetone        = qry.value(22).toDouble();
            groupMute        = qry.value(23).toInt();
            pttDelay        = qry.value(24).toInt();
            outputDSPLevel        = qry.value(25).toInt();
            outputToneState  = qry.value(26).toInt();
            outputToneFrequency = qry.value(27).toDouble();
            outputTonePhase    = qry.value(28).toInt();
            outputToneLevel    = qry.value(29).toDouble();
            recServerAddr1  = qry.value(30).toString();
            recServerAddr2  = qry.value(31).toString();
            recInputDSPLevel    = qry.value(32).toInt();
            recOutputDSPLevel    = qry.value(33).toDouble();
//            qDebug() << "outputfrequency" << outputToneFrequency << "Level" << outputTonePhase << "softPhoneID" << softPhoneID;
            emit onNewControler(softPhoneID, channelID, sipPort, sipUser, keepAlivePeroid, rtpStartPort,WireConnectMode, mainRadioTransmitterUsed, mainRadioReceiverUsed, ServerClientMode,txScheduler,
                                numConnection,sidetone,localSidetone,sqlAlwayOn,sqlActiveHigh, deviceName,inputLevel,outputLevel,radioAutoInactive,radioMainStandby, defaultEthernet,rxBestSignalEnable,groupMute,
                                pttDelay,outputDSPLevel,outputToneState,outputToneFrequency,outputTonePhase, outputToneLevel,recInputDSPLevel, recOutputDSPLevel, recServerAddr1, recServerAddr2);
        }
    }
    db.close();
    return true;
}
bool Database::checkDefaultEthernetColumeExist()
{
    QString query;

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    query = QString("SELECT defaultEthernet FROM controler LIMIT 1");
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
        return false;
    }

    else
    {
    }
    db.close();
    return  true;
}
bool Database::upgradeDatabase()
{
    bool DefaultEthernetColumeExist = checkDefaultEthernetColumeExist();

    QString query;

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;

    if (DefaultEthernetColumeExist == false)
    {
        query = QString("ALTER TABLE controler ADD defaultEthernet VARCHAR(8) NOT NULL DEFAULT'eth0'");
        qry.prepare(query);
        if (!qry.exec())
        {
            qDebug() << qry.lastError();
            return false;
        }

        else
        {
            qDebug() << query;
        }
    }
    db.close();

    addColumnGroupMute();
    addColumnPttDelay();
    return true;
}
bool Database::getAllChannelByUserID(int softPhoneID)
{
    int channelID;
    bool visible;
    QString channelName;
    QString mainRadioType;
    int mainRadio01ID;
    int mainRadio02ID;
    bool standbyEnable;
    QString standbyRadioType;
    int standbyRadio01ID;
    int standbyRadio02ID;
    QString mainRadioName;
    QString standbyRadioName;
    int trxModeEnable = 0;
    bool mainRadioTransmitterUsed = false;
    bool mainRadioReceiverUsed = false;
    QString query = QString("SELECT id, visible, channelName, mainRadioType, mainRadio01ID, mainRadio02ID, standbyEnable, standbyRadioType, standbyRadio01ID, standbyRadio02ID, mainRadioName, standbyRadioName, mainRadioTransmitterUsed, mainRadioReceiverUsed  FROM channel WHERE softPhoneID=%1 ORDER BY id").arg(softPhoneID);
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    else
    {
        while (qry.next()) {
            channelID       = qry.value(0).toInt();
            visible         = qry.value(1).toInt();
            channelName     = qry.value(2).toString();
            mainRadioType   = qry.value(3).toString();
            mainRadio01ID   = qry.value(4).toInt();
            mainRadio02ID   = qry.value(5).toInt();
            standbyEnable   = qry.value(6).toInt();
            standbyRadioType= qry.value(7).toString();
            standbyRadio01ID= qry.value(8).toInt();
            standbyRadio02ID= qry.value(9).toInt();
            mainRadioName   = qry.value(10).toString();
            standbyRadioName= qry.value(11).toString();
            mainRadioTransmitterUsed        = qry.value(12).toInt() == 1;
            mainRadioReceiverUsed        = qry.value(13).toInt() == 1;
            //trx = 1, rx-only=3, tx-only=2, disable all=0
            if (mainRadioTransmitterUsed&mainRadioReceiverUsed) trxModeEnable = 1;
            else if ((mainRadioTransmitterUsed)&(!mainRadioReceiverUsed)) trxModeEnable = 2;
            else if ((!mainRadioTransmitterUsed)&(mainRadioReceiverUsed)) trxModeEnable = 3;
            else trxModeEnable = 0;
            emit newchannel(channelID ,visible, channelName, mainRadioType, mainRadio01ID, mainRadio02ID, standbyEnable, standbyRadioType, standbyRadio01ID, standbyRadio02ID, mainRadioName, standbyRadioName, trxModeEnable);
        }
    }
    db.close();
    return true;
}
bool Database::getAllRole(int softPhoneID)
{
    QString query;

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    QStringList roleIDList;

    query = QString("SELECT roleID FROM role WHERE softPhoneID=%1 AND channelIdInRole=1 ORDER BY roleID").arg(softPhoneID);
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    else
    {
        while (qry.next()) {
            roleIDList             << qry.value(0).toString();
        }
    }
    db.close();

    Q_FOREACH(QString roleID, roleIDList){
        if (roleID.toInt() > 0){
            getRole(roleID.toInt(),softPhoneID);
        }
    }


    return  true;
}

bool Database::removeRole(int roleID, int softPhoneID)
{
    QString query;

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;

    query = QString("DELETE FROM role WHERE roleID=%2 AND softPhoneID=%3").arg(roleID).arg(softPhoneID);
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    db.close();
    return  true;
}
bool Database::addRole(QString roleName, QStringList channelIdSelect, int softPhoneID)
{
    int newRoleID=0;
    QString query;

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    query = QString("SELECT roleID FROM role WHERE softPhoneID=%1 ORDER BY roleID DESC LIMIT 1").arg(softPhoneID);

    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    else
    {
        while (qry.next()) {
            newRoleID     = qry.value(0).toInt();
        }
    }

    int i = 1;
    qDebug() << "Create new Role : Role Name:" << roleName  << "Role ID:" << newRoleID+1 << channelIdSelect;
    Q_FOREACH(QString channelID, channelIdSelect) {
        int channelid = channelID.toInt();
        query = QString("INSERT INTO role (softPhoneID, visible, channelID, channelIdInRole, roleName, roleID)  VALUES (%1, 1, %2, %3, '%4', %5)")
                .arg(softPhoneID).arg(channelid).arg(i).arg(roleName).arg(newRoleID+1);
        qry.prepare(query);
        if (!qry.exec()){
            qDebug() << qry.lastError();
        }
        i++;
    }
    db.close();

    return true;
}
bool Database::updateRole(int roleID, QString roleName, QStringList channelIdSelect, int softPhoneID)
{
    QString query;

    qDebug() << "Update Role : Role Name:" << roleName  << "roleID:" << roleID << channelIdSelect;
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;

    query = QString("UPDATE role SET roleName='%1' WHERE roleID=%2 AND softPhoneID=%3").arg(roleName).arg(roleID).arg(softPhoneID);

    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    int j=0;
    Q_FOREACH(QString channelID, channelIdSelect) {
        int channelid = channelID.toInt();
        j++;
        query = QString("UPDATE role SET channelID=%1 WHERE roleID=%2 AND softPhoneID=%3 AND channelIdInRole=%4").arg(channelid).arg(roleID).arg(softPhoneID).arg(j);
//        qDebug() << query;
        qry.prepare(query);
        if (!qry.exec()){
            qDebug() << qry.lastError();
        }
    }

    db.close();
    return  true;
}

bool Database::getRole(int RoleIDSelect, int softPhoneID)
{
    QString query;

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    QString roleName;
    int channelIdInRole;
    QStringList roleChannelID;

    query = QString("SELECT channelIdInRole, roleName, channelID FROM role WHERE softPhoneID=%1 AND channelIdInRole>0 AND roleID=%2 ORDER BY channelIdInRole LIMIT 10").arg(softPhoneID).arg(RoleIDSelect);
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    else
    {
        while (qry.next()) {
            channelIdInRole             = qry.value(0).toInt();
            roleName                    = qry.value(1).toString();
            roleChannelID     << qry.value(2).toString();
        }
        emit newRoleData(RoleIDSelect,roleName,roleChannelID);
    }
    db.close();
    return  true;
}

bool Database::getAllChannelInRole(int softPhoneID)
{
    int channelID;
    bool visible;
    QString channelName = "";
    QString mainRadioType = "";
    int mainRadio01ID = 0;
    int mainRadio02ID = 0;
    bool standbyEnable = 0;
    QString standbyRadioType = "";
    int standbyRadio01ID = 0;
    int standbyRadio02ID = 0;
    QString mainRadioName = "";
    QString standbyRadioName = "";
    QString query;
    int trxModeEnable = 0;
    bool mainRadioTransmitterUsed = false;
    bool mainRadioReceiverUsed = false;
    bool mainAlwaysConnect = true;
    bool standbyAlwaysConnect = true;
    bool mainRadioEnable = false;

    for(int i=0; i<3; i++)
    {
        emit newCallChannel(i+1, 0 ,0, channelName, mainRadioType, mainRadio01ID, mainRadio02ID, standbyEnable, standbyRadioType, standbyRadio01ID, standbyRadio02ID, mainRadioName, standbyRadioName, trxModeEnable, mainAlwaysConnect,    standbyAlwaysConnect,   mainRadioEnable,  standbyEnable);
    }

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    QString roleName;
    int channelIdInRole;
    int roleChannelID[10] = {0,0,0,0,0,0,0,0,0,0};

    query = QString("SELECT channelIdInRole, roleName, channelID FROM role WHERE softPhoneID=%1 AND channelID>0 AND roleID=%2 ORDER BY channelIdInRole LIMIT 10").arg(softPhoneID).arg(roleIDSelect);
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    else
    {
        while (qry.next()) {
            channelIdInRole             = qry.value(0).toInt();
            roleName                    = qry.value(1).toString();
            roleChannelID[channelIdInRole-1]     = qry.value(2).toInt();
        }
//        emit newRoleData(RoleIDSelect,roleName,roleChannelID);
    }

    query = QString("SELECT id, visible, channelName, mainRadioType, mainRadio01ID, mainRadio02ID, standbyEnable, standbyRadioType, standbyRadio01ID,"
                    " standbyRadio02ID, mainRadioName, standbyRadioName, mainRadioTransmitterUsed, mainRadioReceiverUsed  FROM channel WHERE softPhoneID=%1 AND"
                    " (id=%2 OR id=%3 OR id=%4 OR id=%5 OR id=%6 OR id=%7 OR id=%8 OR id=%9 OR id=%10 OR id=%11) ")
            .arg(softPhoneID).arg(roleChannelID[0]).arg(roleChannelID[1]).arg(roleChannelID[2]).arg(roleChannelID[3]).arg(roleChannelID[4]).arg(roleChannelID[5])
            .arg(roleChannelID[6]).arg(roleChannelID[7]).arg(roleChannelID[8]).arg(roleChannelID[9]);
//    qDebug() << query;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    else
    {
        while (qry.next())
        {
            int i = 0;
            channelID       = qry.value(0).toInt();
            for(i=0; i<10; i++){
                if(roleChannelID[i] == channelID) break;
            }
            visible         = qry.value(1).toInt();
            channelName     = qry.value(2).toString();
            mainRadioType   = qry.value(3).toString();
            mainRadio01ID   = qry.value(4).toInt();
            mainRadio02ID   = qry.value(5).toInt();
            standbyEnable   = qry.value(6).toInt();
            standbyRadioType= qry.value(7).toString();
            standbyRadio01ID= qry.value(8).toInt();
            standbyRadio02ID= qry.value(9).toInt();
            mainRadioName   = qry.value(10).toString();
            standbyRadioName= qry.value(11).toString();
            mainRadioTransmitterUsed        = qry.value(12).toInt() == 1;
            mainRadioReceiverUsed        = qry.value(13).toInt() == 1;
            //trx = 1, rx-only=3, tx-only=2, disable all=0
            if (mainRadioTransmitterUsed&mainRadioReceiverUsed) trxModeEnable = 1;
            else if ((mainRadioTransmitterUsed)&(!mainRadioReceiverUsed)) trxModeEnable = 2;
            else if ((!mainRadioTransmitterUsed)&(mainRadioReceiverUsed)) trxModeEnable = 3;
            else trxModeEnable = 0;

            emit newCallChannel(i+1, channelID ,visible, channelName, mainRadioType, mainRadio01ID, mainRadio02ID, standbyEnable, standbyRadioType, standbyRadio01ID, standbyRadio02ID, mainRadioName, standbyRadioName, trxModeEnable, mainAlwaysConnect,    standbyAlwaysConnect,   mainRadioEnable,  standbyEnable);
        }
    }
    db.close();
    return true;
}
bool Database::getCallChannel(int softPhoneID)
{
    int channelID = 0;
    bool visible;
    QString channelName = "";
    QString mainRadioType = "";
    int mainRadio01ID = 0;
    int mainRadio02ID = 0;
    bool standbyEnable = 0;
    QString standbyRadioType = "";
    int standbyRadio01ID = 0;
    int standbyRadio02ID = 0;
    QString mainRadioName = "";
    QString standbyRadioName = "";
    QString query;
    int trxModeEnable = 0;
    bool mainRadioTransmitterUsed = false;
    bool mainRadioReceiverUsed = false;
    bool mainAlwaysConnect = true;
    bool standbyAlwaysConnect = true;
    bool mainRadioEnable = false;
    bool standbyRadioEnable = false;

    emit newCallChannel(softPhoneID, 0 ,0, channelName, mainRadioType, mainRadio01ID, mainRadio02ID, standbyEnable, standbyRadioType, standbyRadio01ID, standbyRadio02ID, mainRadioName,
                        standbyRadioName, trxModeEnable, mainAlwaysConnect,    standbyAlwaysConnect,   mainRadioEnable,  standbyRadioEnable);

    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;

    query = QString("SELECT channelID, mainRadioTransmitterUsed, mainRadioReceiverUsed FROM controler WHERE softPhoneID=%1 LIMIT 1").arg(softPhoneID);
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    else
    {
        while (qry.next()) {
            channelID       = qry.value(0).toInt();
            mainRadioTransmitterUsed        = qry.value(1).toInt();
            mainRadioReceiverUsed        = qry.value(2).toInt();
        }
    }

    query = QString("SELECT id, visible, channelName, mainRadioType, mainRadio01ID, mainRadio02ID, standbyEnable, standbyRadioType, standbyRadio01ID,"
                    " standbyRadio02ID, mainRadioName, standbyRadioName, mainRadioEnable, standbyRadioEnable, mainAlwaysConnect, standbyAlwaysConnect FROM channel WHERE "
                    " id=%1").arg(channelID);

//    qDebug() << query;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    else
    {
        while (qry.next()) {
            channelID       = qry.value(0).toInt();
            visible         = qry.value(1).toInt();
            channelName     = qry.value(2).toString();
            mainRadioType   = qry.value(3).toString();
            mainRadio01ID   = qry.value(4).toInt();
            mainRadio02ID   = qry.value(5).toInt();
            standbyEnable   = qry.value(6).toInt();
            standbyRadioType= qry.value(7).toString();
            standbyRadio01ID= qry.value(8).toInt();
            standbyRadio02ID= qry.value(9).toInt();
            mainRadioName   = qry.value(10).toString();
            standbyRadioName= qry.value(11).toString();
            mainRadioEnable     = qry.value(1).toInt();
            standbyRadioEnable  = qry.value(1).toInt();
            mainAlwaysConnect   = qry.value(1).toInt();
            standbyAlwaysConnect= qry.value(1).toInt();
            //trx = 1, rx-only=3, tx-only=2, disable all=0
            if (mainRadioTransmitterUsed&mainRadioReceiverUsed) trxModeEnable = 1;
            else if ((mainRadioTransmitterUsed)&(!mainRadioReceiverUsed)) trxModeEnable = 2;
            else if ((!mainRadioTransmitterUsed)&(mainRadioReceiverUsed)) trxModeEnable = 3;
            else trxModeEnable = 0;

            emit newCallChannel(softPhoneID, channelID ,visible, channelName, mainRadioType, mainRadio01ID, mainRadio02ID, standbyEnable, standbyRadioType, standbyRadio01ID, standbyRadio02ID, mainRadioName, standbyRadioName, trxModeEnable, mainAlwaysConnect,    standbyAlwaysConnect,   mainRadioEnable,  standbyRadioEnable);
        }
    }
    db.close();
    return true;
}
bool Database::getRadioData()
{
    int radioID;
    QString radioName;
    QString trxMode;
    QString uri;
    int r2sPeriod;
    int sipPort;
    QString ipAddress;
    QString frequency;
    QString query = QString("SELECT id, name, trxMode, uri, r2sPeriod, ipAddress, sipPort, frequency  FROM radio");
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    else
    {
        while (qry.next()) {
            radioID         = qry.value(0).toInt();
            radioName       = qry.value(1).toString();
            trxMode         = qry.value(2).toString();
            uri             = qry.value(3).toString();
            r2sPeriod       = qry.value(4).toInt();
            ipAddress       = qry.value(5).toString();
            sipPort         = qry.value(6).toInt();
            frequency       = qry.value(7).toString();
            emit newradio(radioID,radioName,trxMode,uri,ipAddress,r2sPeriod,sipPort, frequency);
        }
    }
    db.close();
    return true;
}

bool Database::addNewRadio(QString radioName, QString trxMode, QString uri, QString ipAddress, int r2sPeriod,int sipPort, QString frequency)
{
    QString query = QString("INSERT INTO radio (name, trxMode, uri, ipAddress, r2sPeriod, sipPort) VALUES ('%1', '%2', '%3', '%4', %5, %6)")
            .arg(radioName).arg(trxMode).arg(uri).arg(ipAddress).arg(r2sPeriod).arg(sipPort);
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
//    qDebug() << query;
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();
    return true;
}

bool Database::updateRadio(QString radioName, QString trxMode, QString uri, QString ipAddress, int r2sPeriod,int sipPort, QString frequency, int radioID)
{
    QString query = QString("UPDATE radio SET name='%1', trxMode='%2', uri='%3', ipAddress='%4', r2sPeriod=%5, sipPort=%6 WHERE id=%7")
            .arg(radioName).arg(trxMode).arg(uri).arg(ipAddress).arg(r2sPeriod).arg(sipPort).arg(radioID);
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();
    return true;
}
bool Database::disableRadioInChannel(int radioID)
{
    QString query;
    QSqlQuery qry;
    query = QString("UPDATE channel SET mainRadio01ID=0 WHERE mainRadio01ID=%1").arg(radioID);
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }

    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    query = QString("UPDATE channel SET mainRadio02ID=0 WHERE mainRadio02ID=%1").arg(radioID);
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    query = QString("UPDATE channel SET standByRadio01ID=0 WHERE standByRadio01ID=%1").arg(radioID);
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    query = QString("UPDATE channel SET standByRadio02ID=0 WHERE standByRadio02ID=%1").arg(radioID);
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    db.close();

    return true;
}
bool Database::removeRadio(int radioID)
{
    QString query = QString("DELETE FROM radio WHERE id=%1").arg(radioID);
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();

    disableRadioInChannel(radioID);
    return true;
}
QString Database::getChannelByID(int id)
{
    QSqlQuery qry;
    QString channelName = "";
    QString query = QString("SELECT channelName FROM channel WHERE id=%1 LIMIT 1").arg(id);
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return "";
    }
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }

    else
    {
        while (qry.next()) {
            channelName       = qry.value(0).toString();
        }
    }
    db.close();
    return channelName;
}
bool Database::removeChannel(int channelID)
{
    QString query = QString("DELETE FROM channel WHERE id=%1").arg(channelID);
    if (!db.open()) {
        qDebug() << "database error! database can not open.";
        emit mysqlError();
        restartMysql();
        return false;
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    db.close();
    return true;
}

QStringList Database::getRadioByID(int radioID)
{
    QStringList radioData;
    QString query = QString("SELECT id, name, trxMode, uri, r2sPeriod, ipAddress, rtpPort, frequency, sipPort FROM radio WHERE id=%1 LIMIT 1").arg(radioID);
    for (int i = 0; i < 9;i++) {
        radioData << "";
    }
    bool openDB = db.isOpen();
    if (openDB == false){
        if (!db.open()) {
            qDebug() << "database error! database can not open.";
            emit mysqlError();
            restartMysql();
            return radioData;
        }
    }
    QSqlQuery qry;
    qry.prepare(query);
    if (!qry.exec()){
        qDebug() << qry.lastError();
    }
    else
    {
        radioData.clear();
        while (qry.next()){
            for (int i = 0; i < 9;i++) {
                radioData << qry.value(i).toString();
            }
        }
    }
    if (openDB == false){
        db.close();
    }
    if (radioData.size() < 9){
        for (int i = 0; i < 9;i++) {
            radioData << "";
        }
    }
    return  radioData;
}
