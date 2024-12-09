#include <iGate4x.h>

/*
 *  i2cGpioExtReadWriteThread function
 */



void iGate4x::setLED_txrx_value(uint8_t ledNum, bool status)
{
    if (status)
    {
        allLEDStatus |= (1 << ledNum);
    }else{
        allLEDStatus &= (~(1 << ledNum));
    }
}

void iGate4x::i2cGpioExtReadWriteThread()
{
//    while (m_IsEventThreadRunning)
//    {
//        ::usleep(50000);
//    }
}
void iGate4x::adcValue(int channel, double value, QString message)
{
    switch (channel)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        if (psuDCInStatus != (value > 1.0))
        {
            psuDCInStatus = (value > 1.0);
            qDebug() << "psuDCInStatus" << value*11.0 << psuDCInStatus << value;
            emit onDCInChanged();
        }
        psuDCInStatus = (value > 1.0);
        break;
    case 3:
        if (psuBattStatus != (value > 1.0))
        {
            psuBattStatus = (value > 1.0);
            qDebug() << "psuBattStatus" << value*11.0 << psuBattStatus << value;
            emit onDCInChanged();
        }
        psuBattStatus = (value > 1.0);
        break;
    case 4:
        SocketServer->sendMessageToSoftPhone(message,1);
        break;
    case 5:
        SocketServer->sendMessageToSoftPhone(message,2);
        break;
    case 6:
        SocketServer->sendMessageToSoftPhone(message,3);
        break;
    case 7:
        SocketServer->sendMessageToSoftPhone(message,4);
        break;


    }
}
void iGate4x::dcInChanged()
{
    QString message = QString("{"
                              "\"menuID\"               :\"dcInStatus\","
                              "\"dcIn1\"                :%1,"
                              "\"dcIn2\"                :%2 }"
                              ).arg(psuDCInStatus).arg(psuBattStatus);
    SocketServer->broadcastMessage(message);
}

void iGate4x::get_dcIn_status()
{

}

void iGate4x::startAllSoftphone(int softphoneID)
{
    switch (softphoneID)
    {
    case 0:
        system("systemctl start iGatePlusCH1");
        ::sleep(1);
        system("systemctl start iGatePlusCH2");
        ::sleep(1);
        system("systemctl start iGatePlusCH3");
        ::sleep(1);
        system("systemctl start iGatePlusCH4");
        ::sleep(1);
        break;
    case 1:
        ::sleep(1);
        system("systemctl start iGatePlusCH1");
        break;
    case 2:
        ::sleep(1);
        system("systemctl start iGatePlusCH2");
        break;
    case 3:
        ::sleep(1);
        system("systemctl start iGatePlusCH3");
        break;
    case 4:
        ::sleep(1);
        system("systemctl start iGatePlusCH4");
        break;
    }

}
void iGate4x::stopAllSoftphone(int softphoneID)
{
    switch (softphoneID)
    {
    case 0:
        system("systemctl stop iGatePlusCH1");
        system("systemctl stop iGatePlusCH2");
        system("systemctl stop iGatePlusCH3");
        system("systemctl stop iGatePlusCH4");
        break;
    case 1:
        system("systemctl stop iGatePlusCH1");
        break;
    case 2:
        system("systemctl stop iGatePlusCH2");
        break;
    case 3:
        system("systemctl stop iGatePlusCH3");
        break;
    case 4:
        system("systemctl stop iGatePlusCH4");
        break;
    }
}

void iGate4x::onComponentCompleted(int softPhoneID,QString sipuser, int channelId)
{
    QString  message = QString("{"
                           "\"menuID\"               :\"channelMessage\", "
                           "\"channelID\"            :%1, "
                           "\"numConn\"              :0, "
                           "\"channelName\"          :\"%2\", "
                           "\"radioType\"            :\"\", "
                           "\"frequency\"            :0, "
                           "\"numTxRx\"              :0, "
                           "\"mainRadioName\"        :\"\", "
                           "\"standbyRadioName\"     :\"\", "
                           "\"mainTxConnStatus\"     :0, "
                           "\"mainRxConnStatus\"     :0, "
                           "\"standbyTxConnStatus\"  :0, "
                           "\"standbyRxConnStatus\"  :0, "
                           "\"mainTxPTTOn\"          :0, "
                           "\"mainRxSQLOn\"          :0, "
                           "\"standbyTxPTTOn\"       :0, "
                           "\"standbyRxSQLOn\"       :0, "
                           "\"mainRxRadioVolumeActive\"      :0, "
                           "\"standbyRxRadioVolumeActive\"   :0, "
                           "\"mainRadioTRxType\"             :\"\", "
                           "\"standbyRadioTRxType\"          :\"\", "
                           "\"trxMode\"                      :0, "
                           "\"mainRadioBSSRxLevel\"          :0, "
                           "\"standbyRadioBSSRxLevel\"       :0, "
                           "\"mainRadioTxRFPower\"           :\"\", "
                           "\"standbyRadioTxRFPower\"        :\"\", "
                           "\"mainSqlchThrhCarrier\"         :0, "
                           "\"standbySqlchThrhCarrier\"      :0, "
                           "\"visible\"                      :1, "
                           "\"softphoneID\"                  :%3, "
                           "\"mainTxConnDuration\"           :\"\", "
                           "\"mainRxConnDuration\"           :\"\", "
                           "\"standbyTxConnDuration\"        :\"\", "
                           "\"standbyRxConnDuration\"        :\"\", "
                           "\"mainTxFrequency\"              :\"\", "
                           "\"mainRxFrequency\"              :\"\", "
                           "\"standbyTxFrequency\"           :\"\", "
                           "\"standbyRxFrequency\"           :\"\", "
                           "\"mainTxRadiostatus\"            :\"\", "
                           "\"mainRxRadiostatus\"            :\"\", "
                           "\"standbyTxRadiostatus\"         :\"\", "
                           "\"standbyRxRadiostatus\"         :\"\", "
                           "\"main01_is_mainTx\"             :0, "
                           "\"main01_is_mainRx\"             :0, "
                           "\"main02_is_mainRx\"             :0, "
                           "\"standby01_is_mainTx\"          :0, "
                           "\"standby01_is_mainRx\"          :0, "
                           "\"standby02_is_mainRx\"          :0, "
                           "\"standByEnable\"                :0, "
                           "\"mainRadio01URI\"               :\"\", "
                           "\"mainRadio02URI\"               :\"\", "
                           "\"standbyRadio01URI\"            :\"\", "
                           "\"standbyRadio02URI\"            :\"\" "
                           "}"
                           ).arg(channelId).arg(sipuser).arg(softPhoneID);
         emit cppCommand(message);

    Q_FOREACH(ChannelInfo *channelInfo, m_ChannelInfo_list)
    {
        if (channelInfo->softPhoneID == softPhoneID)
        {
            channelInfo->numConn = 0;
            channelInfo->numTxRx = 0;
            channelInfo->SQLOn = false;
            channelInfo->PTTOn = false;
            channelInfo->trxType = "";
            channelInfo->channelID = 0;
            channelInfo->trxMode = 0;
            int numConn = channelInfo->numConn;
            int numTxRx = channelInfo->numTxRx;
            bool mainTxPTTOn = channelInfo->PTTOn;
            bool mainRxSQLOn = channelInfo->SQLOn;
            QString trxType = channelInfo->trxType;
            int channelID = channelInfo->channelID;
            int trxMode = channelInfo->trxMode;

            QString  message = QString("{"
                           "\"menuID\"          :\"channelMessage\", "
                           "\"softPhoneID\"     :%1, "
                           "\"channelID\"       :%2, "
                           "\"trxMode\"         :%3, "
                           "\"mainTxPTTOn\"     :%4, "
                           "\"mainRxSQLOn\"     :%5, "
                           "\"numConn\"         :%6, "
                           "\"numTxRx\"         :%7,  "
                           "\"trxType\"         :\"%8\"  "
                           "}"
                  ).arg(softPhoneID).arg(channelID).arg(trxMode).arg(mainTxPTTOn).arg(mainRxSQLOn).arg(numConn).arg(numTxRx).arg(trxType);

            SocketServer->broadcastMessage(message);
        }
    }
}

int iGate4x::callChannelIDCheck(int softphoneID)
{
    int i = 0;
    Q_FOREACH (channel *newChannel, channel_call_list)
    {
        if (newChannel->softphoneID == softphoneID)
        {
            return i;
        }
        i++;
    }
    return i;
}
void iGate4x::newIGateGroupMng(int id, int softPhoneID, int iGateID, QString ipaddress, QString uriName)
{
    bool appendData = true;
    Q_FOREACH (igateGroupInfo *newdata, igateGroupInfoList)
    {
        if ((newdata->id == id) || ((newdata->softPhoneID == softPhoneID) & (newdata->iGateID == iGateID) & (newdata->ipaddress == ipaddress)))
        {
            newdata->id = id;
            newdata->softPhoneID = softPhoneID;
            newdata->iGateID = iGateID;
            newdata->ipaddress = ipaddress;
            newdata->uriName = uriName;
            appendData = false;
        }
    }
    if (appendData)
    {
        igateGroupInfo *newdata;
        newdata = new igateGroupInfo;
        newdata->id = id;
        newdata->softPhoneID = softPhoneID;
        newdata->iGateID = iGateID;
        newdata->ipaddress = ipaddress;
        newdata->uriName = uriName;
        igateGroupInfoList.append(newdata);
    }
}
void iGate4x::addIGateGroupMng(int softPhoneID,int iGateID,QString ipaddress,QString uriName)
{
    bool appendData = true;
    qDebug() << "addIGateGroupMng" << softPhoneID << iGateID << ipaddress << uriName;
    Q_FOREACH (igateGroupInfo *newdata, igateGroupInfoList)
    {
        if ((newdata->softPhoneID == softPhoneID) & (newdata->iGateID == iGateID) & (newdata->ipaddress == ipaddress))
        {
            newdata->softPhoneID = softPhoneID;
            newdata->iGateID = iGateID;
            newdata->ipaddress = ipaddress;
            newdata->uriName = uriName;
            appendData = false;
        }
    }
    if (appendData)
    {
        igateGroupInfo *newdata;
        newdata = new igateGroupInfo;
        newdata->softPhoneID = softPhoneID;
        newdata->iGateID = iGateID;
        newdata->ipaddress = ipaddress;
        newdata->uriName = uriName;
        igateGroupInfoList.append(newdata);
        myDatabase->addIGateGroupMng(softPhoneID,iGateID,ipaddress,uriName);

        QString message = QString("{\"menuID\":\"newIgateGroupInfoList\", \"softPhoneID\":%1, \"ipaddress\":\"%2\", \"uriName\":\"%3\", \"visible\":%4}")
                .arg(newdata->iGateID).arg(newdata->ipaddress).arg(newdata->uriName).arg(newdata->visible);
        SocketServer->sendToIGateGroupMNGClient(message,softPhoneID);
    }
    myDatabase->getIGateGroupMng();
}
void iGate4x::clearIGateGroupMng(int softPhoneID)
{
    qDebug() << "clearIGateGroupMng" << softPhoneID;
    QString jsonMessage = QString("{\"menuID\":\"clearIGateGroup\", \"softPhoneID\":%1}").arg(softPhoneID);
    Q_FOREACH (igateGroupInfo *newdata, igateGroupInfoList)
    {
        if (newdata->softPhoneID == softPhoneID)
        {
            newdata->visible = false;
            QString message = QString("{\"menuID\":\"newIgateGroupInfoList\", \"softPhoneID\":%1, \"ipaddress\":\"%2\", \"uriName\":\"%3\", \"visible\":%4}")
                    .arg(newdata->iGateID).arg(newdata->ipaddress).arg(newdata->uriName).arg(newdata->visible);
            SocketServer->sendToIGateGroupMNGClient(message,softPhoneID);

            igateGroupInfoList.removeOne(newdata);
        }
        myDatabase->deleteIGateGroup(0,softPhoneID);
        SocketServer->sendToIGateGroupMNGClient(jsonMessage,softPhoneID);
    }
}
void iGate4x::deleteSocket(int socketIndex)
{

    Q_FOREACH (ChatClientList *newdata, chatClientList)
    {
        if (newdata->socketIndex == socketIndex){
            newdata->chatClient->deleteLater();
            chatClientList.removeOne(newdata);
            qDebug() << "iGate4x::deleteSocket(ChatClient *newConnect)" << socketIndex;
            break;
        }
    }

}
void iGate4x::deleteIGate(int softPhoneID, int id, int iGateID, QString ipaddress)
{
    qDebug() << "deleteIGate" << softPhoneID << iGateID << ipaddress << id;
    if ((id == 0) & (iGateID == 0))
    {
        Q_FOREACH (igateGroupInfo *newdata, igateGroupInfoList)
        {
            if (newdata->softPhoneID == softPhoneID)
            {
                newdata->visible = false;
                myDatabase->deleteIGateGroup(id,softPhoneID);
                QString message = QString("{\"menuID\":\"newIgateGroupInfoList\", \"softPhoneID\":%1, \"ipaddress\":\"%2\", \"uriName\":\"%3\", \"visible\":%4}")
                        .arg(newdata->iGateID).arg(newdata->ipaddress).arg(newdata->uriName).arg(newdata->visible);
                SocketServer->sendToIGateGroupMNGClient(message,softPhoneID);
                syncDeleteIGateAnotherGroup(softPhoneID,iGateID,ipaddress);
                igateGroupInfoList.removeOne(newdata);
                break;
            }
        }
    }
    else
    {
        Q_FOREACH (igateGroupInfo *newdata, igateGroupInfoList)
        {
            if ((newdata->softPhoneID == softPhoneID) & (newdata->id == id))
            {
                newdata->softPhoneID = softPhoneID;
                newdata->visible = false;
                myDatabase->deleteIGateGroup(id,softPhoneID);
                QString message = QString("{\"menuID\":\"newIgateGroupInfoList\", \"softPhoneID\":%1, \"ipaddress\":\"%2\", \"uriName\":\"%3\", \"visible\":%4}")
                        .arg(newdata->iGateID).arg(newdata->ipaddress).arg(newdata->uriName).arg(newdata->visible);
                SocketServer->sendToIGateGroupMNGClient(message,softPhoneID);
                syncDeleteIGateAnotherGroup(softPhoneID,iGateID,ipaddress);
                igateGroupInfoList.removeOne(newdata);
                break;
            }
        }
    }
}
void iGate4x::syncDeleteIGateAnotherGroup(int deleteIGateID, int fromIGateID, QString socketAddress)
{
    socketIndex++;
    qDebug() << "syncDeleteIGateAnotherGroup" << "deleteIGateID" << deleteIGateID << "fromIGateID" << fromIGateID << "socketAddress" << socketAddress << eth0.ipaddress << eth1.ipaddress;
    if ((socketAddress == eth0.ipaddress)||(socketAddress == eth1.ipaddress)){
        Q_FOREACH (igateGroupInfo *newdata, igateGroupInfoList)
        {
            if ((newdata->softPhoneID == fromIGateID) & (newdata->iGateID == deleteIGateID) & ((newdata->ipaddress == eth0.ipaddress)||(newdata->ipaddress == eth1.ipaddress)))
            {
                newdata->visible = false;
                myDatabase->deleteIGateGroup(newdata->id,fromIGateID);
                QString message = QString("{\"menuID\":\"newIgateGroupInfoList\", \"softPhoneID\":%1, \"ipaddress\":\"%2\", \"uriName\":\"%3\", \"visible\":%4}")
                        .arg(newdata->iGateID).arg(newdata->ipaddress).arg(newdata->uriName).arg(newdata->visible);
                SocketServer->sendToIGateGroupMNGClient(message,newdata->iGateID);
                igateGroupInfoList.removeOne(newdata);
                break;
            }
        }
    }
    else
    {
        QString jsonMessage = QString("{\"menuID\":\"syncDeleteIGateAnotherGroup\", \"softPhoneID\":%1, \"iGateID\":%2, \"ipaddress0\":\"%3\", \"ipaddress1\":\"%4\"}").arg(fromIGateID).arg(deleteIGateID).arg(eth0.ipaddress).arg(eth1.ipaddress);
        newConnect = new ChatClient(1234,socketAddress,jsonMessage,socketIndex,false);
        ChatClientList *newChatClient = new ChatClientList;
        newChatClient->chatClient = newConnect;
        newChatClient->socketIndex = socketIndex;
        chatClientList.append(newChatClient);
        connect(newConnect,SIGNAL(connected(int)),
                          this,SLOT(deleteSocket(int)));
    }
}
void iGate4x::syncDeleteIGateAnotherGroup(int deleteIGateID, int softPhoneID, QString ipaddress0, QString ipaddress1)
{
    Q_FOREACH (igateGroupInfo *newdata, igateGroupInfoList)
    {
        if ((newdata->softPhoneID == softPhoneID) & (newdata->iGateID == deleteIGateID) & ((newdata->ipaddress == ipaddress0) || (newdata->ipaddress == ipaddress1)))
        {
            newdata->visible = false;
            myDatabase->deleteIGateGroup(newdata->id,softPhoneID);
            QString message = QString("{\"menuID\":\"newIgateGroupInfoList\", \"softPhoneID\":%1, \"ipaddress\":\"%2\", \"uriName\":\"%3\", \"visible\":%4}")
                    .arg(newdata->iGateID).arg(newdata->ipaddress).arg(newdata->uriName).arg(newdata->visible);
            SocketServer->sendToIGateGroupMNGClient(message,softPhoneID);
            igateGroupInfoList.removeOne(newdata);
            break;
        }
    }
}
void iGate4x::deleteIGateGroup(QString toIPAddress,int softPhoneID)
{
    socketIndex++;
    if ((toIPAddress == eth0.ipaddress)||(toIPAddress == eth1.ipaddress))
        clearIGateGroupMng(softPhoneID);
    else
    {
        QString jsonMessage = QString("{\"menuID\":\"clearIGateGroup\", \"softPhoneID\":%1}").arg(softPhoneID);
        newConnect = new ChatClient(1234,toIPAddress,jsonMessage,socketIndex,false);
        ChatClientList *newChatClient = new ChatClientList;
        newChatClient->chatClient = newConnect;
        newChatClient->socketIndex = socketIndex;
        chatClientList.append(newChatClient);
        connect(newConnect,SIGNAL(connected(int)),
                          this,SLOT(deleteSocket(int)));
    }
}
void iGate4x::syncIGateGroup(QString toAddress, int softPhoneID,int iGateID,QString ipaddress,QString uriName)
{
    socketIndex++;
    if ((toAddress == eth0.ipaddress)||(toAddress == eth1.ipaddress))
        addIGateGroupMng(softPhoneID, iGateID, ipaddress, uriName);
    else
    {
        QString jsonMessage = QString("{\"menuID\":\"newIGateGroup\", \"softPhoneID\":%1, \"ipaddress\":\"%2\", \"iGateID\":%3}").arg(softPhoneID).arg(ipaddress).arg(iGateID);
        newConnect = new ChatClient(1234,toAddress,jsonMessage,socketIndex,false);
        ChatClientList *newChatClient = new ChatClientList;
        newChatClient->chatClient = newConnect;
        newChatClient->socketIndex = socketIndex;
        chatClientList.append(newChatClient);
        connect(newConnect,SIGNAL(connected(int)),
                          this,SLOT(deleteSocket(int)));
    }
    myDatabase->getIGateGroupMng();
}
int iGate4x::radioIDCheck(int radioID)
{
    int i = 0;
    Q_FOREACH (radioData *newRadio, radio_list){
        if (newRadio->id == radioID){
            return i;
        }
        i++;
    }
    return i;
}

void iGate4x::newradio(int radioID, QString radioName, QString trxMode, QString uri, QString ipAddress, int r2sPeriod, int sipPort, QString frequenicy)
{

    int i = (radioIDCheck(radioID));
    qDebug() << "newradio" << radioID << radioName << uri;
    if (i == radio_list.size())
        radio_list.append(new radioData); //Add
    else if(i > radio_list.size()) return;

    //Update
    radio_list.at(i)->id = radioID;
    radio_list.at(i)->radioName = radioName;
    radio_list.at(i)->trxMode = trxMode;
    radio_list.at(i)->ipAddress = ipAddress;
    radio_list.at(i)->r2sPeriod = r2sPeriod;
    radio_list.at(i)->sipPort = sipPort;
    radio_list.at(i)->frequency = frequenicy;
    radio_list.at(i)->uri = uri;
    radio_list.at(i)->visible = true;
}

void iGate4x::newCallChannel(int softPhoneID, int channelID, int visible, QString channelName, QString mainRadioType, int mainRadio01ID, int mainRadio02ID,
                             int standbyEnable, QString standbyRadioType, int standbyRadio01ID, int standbyRadio02ID, QString mainRadioName, QString standbyRadioName,
                             int trxModeEnable, bool mainAlwaysConnect, bool standbyAlwaysConnect, bool mainRadioEnable, bool standbyRadioEnable)
{

    int i = (callChannelIDCheck(softPhoneID));
    qDebug() << "c++: new call channel" << channelID
             << ", softPhoneID" << softPhoneID
             << ", mainRadioName" << mainRadioName
             << ", standbyRadioName" << standbyRadioName
             << ", mainRadioType" << mainRadioType
             << ", mainRadio01ID:" << mainRadio01ID
             << ", mainRadio02ID:" << mainRadio02ID
             << ",standbyEnable:" << standbyEnable;
    if (i == channel_call_list.size())
    {
        channel_call_list.append(new channel);
        qDebug() << "channel_call_list.append(new channel)";
    }
    else if(i > channel_call_list.size()) return;

    m_channelID = channelID;
    channel_call_list.at(i)->softphoneID = softPhoneID;
    channel_call_list.at(i)->id = channelID;
    channel_call_list.at(i)->visible = visible;
    channel_call_list.at(i)->channelName = channelName;
    channel_call_list.at(i)->mainRadioType = mainRadioType;
    channel_call_list.at(i)->mainRadio01ID = mainRadio01ID;
    channel_call_list.at(i)->mainRadio02ID = mainRadio02ID;
    channel_call_list.at(i)->standbyEnable = standbyEnable;
    channel_call_list.at(i)->standbyRadioType = standbyRadioType;
    channel_call_list.at(i)->standbyRadio01ID = standbyRadio01ID;
    channel_call_list.at(i)->standbyRadio02ID = standbyRadio02ID;
    channel_call_list.at(i)->mainRadioName = mainRadioName;
    channel_call_list.at(i)->standbyRadioName = standbyRadioName;
    channel_call_list.at(i)->trxModeEnable = trxModeEnable;
    channel_call_list.at(i)->mainAlwaysConnect = mainAlwaysConnect;
    channel_call_list.at(i)->standbyAlwaysConnect = standbyAlwaysConnect;
    channel_call_list.at(i)->mainRadioEnable = mainRadioEnable;
    channel_call_list.at(i)->standbyRadioEnable = standbyEnable;
    switch (trxModeEnable)
    {
    case 0 :
        channel_call_list.at(i)->mainRadioTransmitterUsed = false;
        channel_call_list.at(i)->mainRadioReceiverUsed = false;
        break;
    case 1 :
        channel_call_list.at(i)->mainRadioTransmitterUsed = true;
        channel_call_list.at(i)->mainRadioReceiverUsed = true;
        break;
    case 2 :
        channel_call_list.at(i)->mainRadioTransmitterUsed = true;
        channel_call_list.at(i)->mainRadioReceiverUsed = false;
        break;
    case 3 :
        channel_call_list.at(i)->mainRadioTransmitterUsed = false;
        channel_call_list.at(i)->mainRadioReceiverUsed = true;
        break;
    }
    if(mainRadioType == TRXMODE_SEPARATE)
    {
        QStringList radioData = myDatabase->getRadioByID(mainRadio01ID);
        if (radioData.size() >= 9)
        {
            channel_call_list.at(i)->mainRadio01.id = radioData.at(0).toInt();
            channel_call_list.at(i)->mainRadio01.callName = radioData.at(1);
            channel_call_list.at(i)->mainRadio01.trxMode = radioData.at(2);
            channel_call_list.at(i)->mainRadio01.uri = radioData.at(3);
            channel_call_list.at(i)->mainRadio01.r2sPeriod = radioData.at(4).toInt();
            channel_call_list.at(i)->mainRadio01.ipAddress = radioData.at(5);
            channel_call_list.at(i)->mainRadio01.rtpPort = radioData.at(6).toInt();
            channel_call_list.at(i)->mainRadio01.frequency = radioData.at(7).toInt();
            channel_call_list.at(i)->mainRadio01.portAddr = radioData.at(8).toInt();
        }

        radioData.clear();
        radioData = myDatabase->getRadioByID(mainRadio02ID);
        if (radioData.size() >= 9)
        {
            channel_call_list.at(i)->mainRadio02.id = radioData.at(0).toInt();
            channel_call_list.at(i)->mainRadio02.callName = radioData.at(1);
            channel_call_list.at(i)->mainRadio02.trxMode = radioData.at(2);
            channel_call_list.at(i)->mainRadio02.uri = radioData.at(3);
            channel_call_list.at(i)->mainRadio02.r2sPeriod = radioData.at(4).toInt();
            channel_call_list.at(i)->mainRadio02.ipAddress = radioData.at(5);
            channel_call_list.at(i)->mainRadio02.rtpPort = radioData.at(6).toInt();
            channel_call_list.at(i)->mainRadio02.frequency = radioData.at(7).toInt();
            channel_call_list.at(i)->mainRadio02.portAddr = radioData.at(8).toInt();
        }
    }

    else
    {
        QStringList radioData = myDatabase->getRadioByID(mainRadio01ID);
        if (radioData.size() >= 9){
            channel_call_list.at(i)->mainRadio01.id = radioData.at(0).toInt();
            channel_call_list.at(i)->mainRadio01.callName = radioData.at(1);
            channel_call_list.at(i)->mainRadio01.trxMode = radioData.at(2);
            channel_call_list.at(i)->mainRadio01.uri = radioData.at(3);
            channel_call_list.at(i)->mainRadio01.r2sPeriod = radioData.at(4).toInt();
            channel_call_list.at(i)->mainRadio01.ipAddress = radioData.at(5);
            channel_call_list.at(i)->mainRadio01.rtpPort = radioData.at(6).toInt();
            channel_call_list.at(i)->mainRadio01.frequency = radioData.at(7).toInt();
            channel_call_list.at(i)->mainRadio01.portAddr = radioData.at(8).toInt();
        }
    }

    if (standbyEnable)
    {
        if(standbyRadioType == TRXMODE_SEPARATE)
        {
            QStringList radioData = myDatabase->getRadioByID(standbyRadio01ID);

            if (radioData.size() >= 9)
            {
                channel_call_list.at(i)->standbyRadio01.id = radioData.at(0).toInt();
                channel_call_list.at(i)->standbyRadio01.callName = radioData.at(1);
                channel_call_list.at(i)->standbyRadio01.trxMode = radioData.at(2);
                channel_call_list.at(i)->standbyRadio01.uri = radioData.at(3);
                channel_call_list.at(i)->standbyRadio01.r2sPeriod = radioData.at(4).toInt();
                channel_call_list.at(i)->standbyRadio01.ipAddress = radioData.at(5);
                channel_call_list.at(i)->standbyRadio01.rtpPort = radioData.at(6).toInt();
                channel_call_list.at(i)->standbyRadio01.frequency = radioData.at(7).toInt();
                channel_call_list.at(i)->standbyRadio01.portAddr = radioData.at(8).toInt();
            }
            radioData.clear();
            radioData = myDatabase->getRadioByID(standbyRadio02ID);
            if (radioData.size() >= 9)
            {
                channel_call_list.at(i)->standbyRadio02.id = radioData.at(0).toInt();
                channel_call_list.at(i)->standbyRadio02.callName = radioData.at(1);
                channel_call_list.at(i)->standbyRadio02.trxMode = radioData.at(2);
                channel_call_list.at(i)->standbyRadio02.uri = radioData.at(3);
                channel_call_list.at(i)->standbyRadio02.r2sPeriod = radioData.at(4).toInt();
                channel_call_list.at(i)->standbyRadio02.ipAddress = radioData.at(5);
                channel_call_list.at(i)->standbyRadio02.rtpPort = radioData.at(6).toInt();
                channel_call_list.at(i)->standbyRadio02.frequency = radioData.at(7).toInt();
                channel_call_list.at(i)->standbyRadio02.portAddr = radioData.at(8).toInt();
            }
        }
        else
        {
            QStringList radioData = myDatabase->getRadioByID(standbyRadio01ID);
            qDebug() << radioData;
            if (radioData.size() >= 9){
                channel_call_list.at(i)->standbyRadio01.id = radioData.at(0).toInt();
                channel_call_list.at(i)->standbyRadio01.callName = radioData.at(1);
                channel_call_list.at(i)->standbyRadio01.trxMode = radioData.at(2);
                channel_call_list.at(i)->standbyRadio01.uri = radioData.at(3);
                channel_call_list.at(i)->standbyRadio01.r2sPeriod = radioData.at(4).toInt();
                channel_call_list.at(i)->standbyRadio01.ipAddress = radioData.at(5);
                channel_call_list.at(i)->standbyRadio01.rtpPort = radioData.at(6).toInt();
                channel_call_list.at(i)->standbyRadio01.frequency = radioData.at(7).toInt();
                channel_call_list.at(i)->standbyRadio01.portAddr = radioData.at(8).toInt();
            }
        }
    }
    else {
        channel_call_list.at(i)->standbyRadio01.id = 0;
        channel_call_list.at(i)->standbyRadio01.callName = "";
        channel_call_list.at(i)->standbyRadio01.trxMode = "";
        channel_call_list.at(i)->standbyRadio01.uri = "";
        channel_call_list.at(i)->standbyRadio01.r2sPeriod = 200;
        channel_call_list.at(i)->standbyRadio01.ipAddress = "";
        channel_call_list.at(i)->standbyRadio01.rtpPort = 0;
        channel_call_list.at(i)->standbyRadio01.frequency = 0;
        channel_call_list.at(i)->standbyRadio01.portAddr = 0;

        channel_call_list.at(i)->standbyRadio02.id = 0;
        channel_call_list.at(i)->standbyRadio02.callName = "";
        channel_call_list.at(i)->standbyRadio02.trxMode = "";
        channel_call_list.at(i)->standbyRadio02.uri = "";
        channel_call_list.at(i)->standbyRadio02.r2sPeriod = 200;
        channel_call_list.at(i)->standbyRadio02.ipAddress = "";
        channel_call_list.at(i)->standbyRadio02.rtpPort = 0;
        channel_call_list.at(i)->standbyRadio02.frequency = 0;
        channel_call_list.at(i)->standbyRadio02.portAddr = 0;
    }
    qDebug() << "function end";
}

void iGate4x::updateInputGain(uint8_t value, uint8_t softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    m_softPhone_list.at(i)->inputLevel = value;
    qDebug() << " m_softPhone_list.at()->inputLevel" << i << m_softPhone_list.at(i)->inputLevel;
    bool i2cwrite_ret = false;
    switch (softPhoneID) {
    case 1:
        VoIPVolumeInCH1 = value;
        if(value != 176)
            m_softPhone_list.at(i)->VoIPVolumeIn_RadioTest = VoIPVolumeInCH1;
        if (CODEC_CS42448->active)
            i2cwrite_ret = CODEC_CS42448->Reg11_16hAINVolumeCtrl(VOIPCH1_I2S1,VoIPVolumeInCH1);
        else if (CODEC_PCM3168A->active)
            i2cwrite_ret = CODEC_PCM3168A->setInputGain(VOIPCH1_I2S1,VoIPVolumeInCH1);
        break;
    case 2:
        VoIPVolumeInCH2 = value;
        if(value != 176)
            m_softPhone_list.at(i)->VoIPVolumeIn_RadioTest = VoIPVolumeInCH2;
        if (CODEC_CS42448->active)
            i2cwrite_ret = CODEC_CS42448->Reg11_16hAINVolumeCtrl(VOIPCH2_I2S1,VoIPVolumeInCH2);
        else if (CODEC_PCM3168A->active)
            i2cwrite_ret = CODEC_PCM3168A->setInputGain(VOIPCH2_I2S1,VoIPVolumeInCH2);
        break;
    case 3:
        VoIPVolumeInCH3 = value;
        if(value != 176)
            m_softPhone_list.at(i)->VoIPVolumeIn_RadioTest = VoIPVolumeInCH3;
        if (CODEC_CS42448->active)
            i2cwrite_ret = CODEC_CS42448->Reg11_16hAINVolumeCtrl(VOIPCH3_I2S1,VoIPVolumeInCH3);
        else if (CODEC_PCM3168A->active)
            i2cwrite_ret = CODEC_PCM3168A->setInputGain(VOIPCH3_I2S1,VoIPVolumeInCH3);
        break;
    case 4:
        VoIPVolumeInCH4 = value;
        if(value != 176)
            m_softPhone_list.at(i)->VoIPVolumeIn_RadioTest = VoIPVolumeInCH4;
        if (CODEC_CS42448->active)
            i2cwrite_ret = CODEC_CS42448->Reg11_16hAINVolumeCtrl(VOIPCH4_I2S1,VoIPVolumeInCH4);
        else if (CODEC_PCM3168A->active)
            i2cwrite_ret = CODEC_PCM3168A->setInputGain(VOIPCH4_I2S1,VoIPVolumeInCH4);
        break;
    }
    saveController(softPhoneID);
    dataLoggerServer->createDataLog(i2cwrite_ret,QString("Set Codec Input Gain SoftphoneID %1, value %2").arg(softPhoneID).arg(value));
}


void iGate4x::updateOutputGain(uint8_t value, uint8_t softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    bool i2cwrite_ret = false;
    m_softPhone_list.at(i)->outputLevel = value;
    qDebug() << "updateOutputGain softPhoneID" << softPhoneID << value;

    switch (softPhoneID) {
    case 1:
        VoIPVolumeOutCH1 = value;
        i2cwrite_ret = CODEC_CS42448->Reg08_0FhAOUTXVolumeCtrl(VOIPCH1_I2S1,VoIPVolumeOutCH1);
        break;
    case 2:
        VoIPVolumeOutCH2 = value;
        i2cwrite_ret = CODEC_CS42448->Reg08_0FhAOUTXVolumeCtrl(VOIPCH2_I2S1,VoIPVolumeOutCH2);
        break;
    case 3:
        VoIPVolumeOutCH3 = value;
        i2cwrite_ret = CODEC_CS42448->Reg08_0FhAOUTXVolumeCtrl(VOIPCH3_I2S1,VoIPVolumeOutCH3);
        break;
    case 4:
        VoIPVolumeOutCH4 = value;
        i2cwrite_ret = CODEC_CS42448->Reg08_0FhAOUTXVolumeCtrl(VOIPCH4_I2S1,VoIPVolumeOutCH4);
        break;
    }
    saveController(softPhoneID);
    dataLoggerServer->createDataLog(i2cwrite_ret,QString("Set Output Codec Gain SoftphoneID %1, value %2").arg(softPhoneID).arg(value));
}


void iGate4x::updateDSPOutputGain(uint8_t value, uint8_t softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    m_softPhone_list.at(i)->outputDSPLevel = value;
    double setValue = 1.0;
    double dbValue = 0;
    if (value == 0) dbValue = 0;
    else dbValue =  double(value/(-2));
    if (value == 0) setValue = 1;
    else {
        setValue = pow(10,dbValue/20.0);
    }
    switch (softPhoneID) {
    case 1:
        VoIPVolumeOutDSPCH1 = setValue;
        SigmaFirmWareDownLoad->setModuleSingleVolume(AUDIOOUT_VOLUME_CH1_TARGET_ADDR,AUDIOOUT_VOLUME_CH1_MOD_ADDR,AUDIOOUT_VOLUME_CH1_MOD_VALUE,VoIPVolumeOutDSPCH1);
        break;
    case 2:
        VoIPVolumeOutDSPCH2 = setValue;
        SigmaFirmWareDownLoad->setModuleSingleVolume(AUDIOOUT_VOLUME_CH2_TARGET_ADDR,AUDIOOUT_VOLUME_CH2_MOD_ADDR,AUDIOOUT_VOLUME_CH2_MOD_VALUE,VoIPVolumeOutDSPCH2);
        break;
    case 3:
        VoIPVolumeOutDSPCH3 = setValue;
        SigmaFirmWareDownLoad->setModuleSingleVolume(AUDIOOUT_VOLUME_CH3_TARGET_ADDR,AUDIOOUT_VOLUME_CH3_MOD_ADDR,AUDIOOUT_VOLUME_CH3_MOD_VALUE,VoIPVolumeOutDSPCH3);
        break;
    case 4:
        VoIPVolumeOutDSPCH4 = setValue;
        SigmaFirmWareDownLoad->setModuleSingleVolume(AUDIOOUT_VOLUME_CH4_TARGET_ADDR,AUDIOOUT_VOLUME_CH4_MOD_ADDR,AUDIOOUT_VOLUME_CH4_MOD_VALUE,VoIPVolumeOutDSPCH4);
        break;
    }

    saveController(softPhoneID);
    dataLoggerServer->createDataLog(INFO,QString("Set Output DSP Gain SoftphoneID %1, value %2").arg(softPhoneID).arg(value));
}


void iGate4x::updateInputTone(bool state, float Frequency ,int Phase , float Level , uint8_t softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    m_softPhone_list.at(i) -> tone_state = state;
    m_softPhone_list.at(i) -> frequency = Frequency;
    m_softPhone_list.at(i) -> level = Level;
    m_softPhone_list.at(i) -> phase =  Phase; // 2048/360
//    saveToneData(softPhoneID);
//    saveController(softPhoneID);
//    qDebug() <<"m_softPhone_list.at(i) -> tone_state" << i << m_softPhone_list.at(i) -> tone_state;
    set_tone.tone_state = state;
    set_tone.frequency = Frequency/4000;
    if (Phase == 0 ){
        set_tone.phase = 0;
    }
    else {
        set_tone.phase = 5.68888889*Phase;
    }
    set_tone.level = std::pow(10.0,(Level/20.0));

//    qDebug() <<"debugtonech_t" <<51<< "tone_state" << set_tone.tone_state << "frequency" <<  set_tone.frequency << "phase" << set_tone.phase << "level" << Level;
//    SigmaFirmWareDownLoad->setToneVolume(TONE_CH1_ADDRESS,1);
//    frequency_tone Frequency_tone;
//    Frequency_tone.tone_frequency_ch1 = 67.0/4000;
//    SigmaFirmWareDownLoad->setToneVolume(TONE_FREQUENCY_CH1_ADDRESS,Frequency_tone.tone_frequency_ch1);
//    level_tone Level_tone;
//    Level_tone.level_tone_ch1 = std::pow(10,(1.0/20));
//    SigmaFirmWareDownLoad->setToneVolume(TONE_LEVEL_CH1_ADDRESS,Level_tone.level_tone_ch1);
//    phase_tone Phase_tone;
//    Phase_tone.phase_tone_ch1 = 5.68888889*1; // 2048/360
//    SigmaFirmWareDownLoad->setToneVolume(TONE_PHASE_CH1_ADDRESS,Phase_tone.phase_tone_ch1);

//    qDebug() << "TONE_LEVEL" << 10*log10(50) << "dB";
//    double frequency = 151.4/4000;
//    level_tone_ch1 = std::pow(10,(-40/20));
//     qDebug() << "TONE_LEVEL10" << Frequency_tone.tone_frequency_ch1 << "dB";
//     qDebug() << "level_tone_ch1" << Level_tone.level_tone_ch1;
//     qDebug() << "Phase_tone" << Phase_tone.phase_tone_ch1;

    if (softPhoneID == 1){
        if(set_tone.tone_state == 1){
            set_tone.tone_frequency_ch1 = set_tone.frequency;
            set_tone.level_tone_ch1 = set_tone.level;
            set_tone.phase_tone_ch1 = set_tone.phase;
            SigmaFirmWareDownLoad->setToneVolume(TONE_CH1_ADDRESS,1);
            SigmaFirmWareDownLoad->setToneVolume(TONE_FREQUENCY_CH1_ADDRESS,set_tone.tone_frequency_ch1);
            SigmaFirmWareDownLoad->setToneVolume(TONE_PHASE_CH1_ADDRESS,set_tone.phase_tone_ch1);
            SigmaFirmWareDownLoad->setToneVolume(TONE_LEVEL_CH1_ADDRESS,set_tone.level_tone_ch1);

//            qDebug() <<"debugtonech1" << "tone_state" << set_tone.tone_state << "frequency" << set_tone.tone_frequency_ch1 << "phase" << set_tone.phase_tone_ch1 << "level" << set_tone.phase_tone_ch1;
        }
        else {
            SigmaFirmWareDownLoad->setToneVolume(TONE_CH1_ADDRESS,0);
        }}
    if (softPhoneID == 2){
        if(set_tone.tone_state == 1){
            set_tone.tone_frequency_ch2 = set_tone.frequency;
            set_tone.level_tone_ch2 = set_tone.level;
            set_tone.phase_tone_ch2 = set_tone.phase;
            SigmaFirmWareDownLoad->setToneVolume(TONE_CH2_ADDRESS,1);
            SigmaFirmWareDownLoad->setToneVolume(TONE_FREQUENCY_CH2_ADDRESS,set_tone.tone_frequency_ch2);
            SigmaFirmWareDownLoad->setToneVolume(TONE_PHASE_CH2_ADDRESS,set_tone.phase_tone_ch2);
            SigmaFirmWareDownLoad->setToneVolume(TONE_LEVEL_CH2_ADDRESS,set_tone.level_tone_ch2);
        }
        else {
            SigmaFirmWareDownLoad->setToneVolume(TONE_CH2_ADDRESS,0);
        }}
    if (softPhoneID == 3){
        if(set_tone.tone_state == 1){
            set_tone.tone_frequency_ch3 = set_tone.frequency;
            set_tone.level_tone_ch3 = set_tone.level;
            set_tone.phase_tone_ch3 = set_tone.phase;
            SigmaFirmWareDownLoad->setToneVolume(TONE_CH3_ADDRESS,1);
            SigmaFirmWareDownLoad->setToneVolume(TONE_FREQUENCY_CH3_ADDRESS,set_tone.tone_frequency_ch3);
            SigmaFirmWareDownLoad->setToneVolume(TONE_PHASE_CH3_ADDRESS,set_tone.phase_tone_ch3);
            SigmaFirmWareDownLoad->setToneVolume(TONE_LEVEL_CH3_ADDRESS,set_tone.level_tone_ch3);
        }
        else {
            SigmaFirmWareDownLoad->setToneVolume(TONE_CH3_ADDRESS,0);
        }}
    if (softPhoneID == 4){
        if(set_tone.tone_state == 1){
            set_tone.tone_frequency_ch4 = set_tone.frequency;
            set_tone.level_tone_ch4 = set_tone.level;
            set_tone.phase_tone_ch4 = set_tone.phase;
            SigmaFirmWareDownLoad->setToneVolume(TONE_CH4_ADDRESS,1);
            SigmaFirmWareDownLoad->setToneVolume(TONE_FREQUENCY_CH4_ADDRESS,set_tone.tone_frequency_ch4);
            SigmaFirmWareDownLoad->setToneVolume(TONE_PHASE_CH4_ADDRESS,set_tone.phase_tone_ch4);
            SigmaFirmWareDownLoad->setToneVolume(TONE_LEVEL_CH4_ADDRESS,set_tone.level_tone_ch4);
        }
        else {
            SigmaFirmWareDownLoad->setToneVolume(TONE_CH4_ADDRESS,0);
        }}

     saveToneData(softPhoneID);
     dataLoggerServer->createDataLog(INFO,QString("Set Pilot Tone SoftphoneID %1, Frequency %2, Level %3, Phase %3").arg(softPhoneID).arg(Frequency).arg(Level).arg(Phase));
    }
void iGate4x::updateServerTone(bool tonestate, bool pttstate, int timeoutptt, float ServerFrequency, int ServerPhase, float ServerLevel, uint8_t softPhoneID) {

    QVector<uint8_t> toneAddress = {0, TONE_SERVER_CH1_ADDRESS, TONE_SERVER_CH2_ADDRESS, TONE_SERVER_CH3_ADDRESS, TONE_SERVER_CH4_ADDRESS};
    QVector<uint8_t> tonefrequency = {0, TONE_SERVER_FREQUENCY_CH1_ADDRESS, TONE_SERVER_FREQUENCY_CH2_ADDRESS, TONE_SERVER_FREQUENCY_CH3_ADDRESS, TONE_SERVER_FREQUENCY_CH4_ADDRESS};
    QVector<uint8_t> tonelevel = {0, TONE_SERVER_LEVEL_CH1_ADDRESS, TONE_SERVER_LEVEL_CH2_ADDRESS, TONE_SERVER_LEVEL_CH3_ADDRESS, TONE_SERVER_LEVEL_CH4_ADDRESS};
    QVector<uint8_t> tonephase = {0, TONE_SERVER_PHASE_CH1_ADDRESS, TONE_SERVER_PHASE_CH2_ADDRESS, TONE_SERVER_PHASE_CH3_ADDRESS, TONE_SERVER_PHASE_CH4_ADDRESS};

    tone_server.tone_server_state = tonestate;
    ptt_timeout.tone_server_ptt = pttstate;

    int i = softPhoneIDCheck(softPhoneID);
    if (i < 0 || i >= m_softPhone_list.size()) {
        qWarning() << "Invalid SoftPhoneID:" << softPhoneID;
        return;
    }

    m_softPhone_list.at(i)->tone_server_state = tonestate;
    m_softPhone_list.at(i)->tone_server_ptt = pttstate;
    m_softPhone_list.at(i)->tone_server_frequency = ServerFrequency;
    m_softPhone_list.at(i)->tone_server_level = ServerLevel;
    m_softPhone_list.at(i)->tone_server_phase = ServerPhase;
    m_softPhone_list.at(i)->tone_server_ptt_timeout = timeoutptt;

    if (tonestate && pttstate) {
        Q_FOREACH(SoftPhone *softPhone, m_softPhone_list) {
            if (softPhone && softPhone->softPhoneID == softPhoneID) {
                if (softPhone->timerCout == nullptr) {
                    softPhone->timerCout = new QTimer(this);
                    connect(softPhone->timerCout, &QTimer::timeout, [=]() {
                        handleTimeout(softPhoneID);
                        softPhone->timerCout->stop();
                    });
                }

                if (!softPhone->timerCout->isActive()) {
                    softPhone->timerCout->start(timeoutptt * 1000);
                }
                break;
            }
        }
    } else {
        Q_FOREACH(SoftPhone *softPhone, m_softPhone_list) {
            if (softPhone && softPhone->softPhoneID == softPhoneID) {
                if (softPhone->timerCout) {
                    softPhone->timerCout->stop();
                    delete softPhone->timerCout;
                    softPhone->timerCout = nullptr;
                    handleTimeout(softPhoneID);
                    break;
                }
//                handleTimeout(softPhoneID);
//                break;
            }
        }
    }

    tone_server.tone_server_frequency = ServerFrequency / 4000;
    tone_server.tone_server_level = std::pow(10.0, ServerLevel / 20.0);
    tone_server.tone_server_phase = ServerPhase * 5.68888889;

    if (softPhoneID >= 1 && softPhoneID <= 4) {
        if (tonestate == 1 && pttstate == 1){
        SigmaFirmWareDownLoad->setToneVolume(toneAddress[softPhoneID], 1);
        SigmaFirmWareDownLoad->setToneVolume(tonefrequency[softPhoneID], tone_server.tone_server_frequency);
        SigmaFirmWareDownLoad->setToneVolume(tonelevel[softPhoneID], tone_server.tone_server_level);
        SigmaFirmWareDownLoad->setToneVolume(tonephase[softPhoneID], tone_server.tone_server_phase);
        updateInputGain(176,softPhoneID);
        QString message = QString("{\"menuID\":\"pttTestMode\", \"softPhoneID\":%1, \"value\":%2}")
                          .arg(softPhoneID)
                          .arg(pttstate);
        SocketServer->sendMessageToSoftPhone(message, softPhoneID);
        dataLoggerServer->createDataLog(INFO,QString("Radio PTT Test SoftphoneID %1, Frequency %2, Level %3, Phase %3").arg(softPhoneID).arg(ServerFrequency).arg(ServerLevel).arg(ServerPhase));
        }
    }
}
void iGate4x::handleTimeout(uint8_t softPhoneID) {
    QVector<uint8_t> toneAddress = {0, TONE_SERVER_CH1_ADDRESS, TONE_SERVER_CH2_ADDRESS, TONE_SERVER_CH3_ADDRESS, TONE_SERVER_CH4_ADDRESS};
    int i = softPhoneIDCheck(softPhoneID);

    int tone_state = m_softPhone_list.at(i)->tone_server_state;
    int ptt_state = m_softPhone_list.at(i)->tone_server_ptt;
    float frequency = m_softPhone_list.at(i)->tone_server_frequency;
    float level = m_softPhone_list.at(i)->tone_server_level;
    float phase = m_softPhone_list.at(i)->tone_server_phase;
    int ptt_timeout_value = m_softPhone_list.at(i)->tone_server_ptt_timeout;
    int inputLevel =  m_softPhone_list.at(i)->VoIPVolumeIn_RadioTest;
//    updateInputGain()
//    qDebug() << "handleTimeout called for softPhoneID:" << softPhoneID
//             << "ToneState:" << tone_state
//             << "PttState:" << ptt_state
//             << "Frequency:" << frequency
//             << "Level:" << level
//             << "Phase:" << phase;
    for (uint8_t id = 1; id <= 4; ++id) {
        if (softPhoneID == id) {
            SigmaFirmWareDownLoad->setToneVolume(toneAddress[softPhoneID],0);
             updateInputGain(inputLevel,softPhoneID);
            m_softPhone_list.at(i)->tone_server_state = 0;
            m_softPhone_list.at(i)->tone_server_ptt = 0;
            tone_state = 0;
            ptt_state = 0;
//            updateInputGain(inputLevel,softPhoneID);
            QString message = QString(
                "{\"menuID\":\"UpdateToneServer\",\"UpdateToneServerState\":\"%1\","
                "\"pttState\":\"%2\",\"ToneServerFrequency\":\"%3\","
                "\"ToneServerPhase\":\"%4\",\"ToneServerLevel\":\"%5\",\"timeoutptt\":\"%6\","
                "\"softPhoneID\":\"%7\"}")
                .arg(tone_state)
                .arg(ptt_state)
                .arg(frequency)
                .arg(phase)
                .arg(level)
                .arg(ptt_timeout_value)
                .arg(softPhoneID);
            SocketServer->sendToWebMessageClient(message);
            QString  message2 = QString("{"
                                       "\"menuID\":\"pttTestMode\", "
                                       "\"softPhoneID\":%1, "
                                       "\"value\":%2 }"
                                       ).arg(softPhoneID).arg(ptt_state);
            SocketServer->sendMessageToSoftPhone(message2,softPhoneID);
            qDebug() <<message2;
            qDebug() << "updateToneServerCh" << id
//                     << "Address" << addresses[id]
                     << "ToneServerState" << tone_state
                     << "PttState" << ptt_state
                     << "Frequency" << frequency
                     << "Phase" << phase
                     << "Level" << level;
            saveTestRadioPage(softPhoneID);
            dataLoggerServer->createDataLog(INFO,QString("Radio PTT Test Timeout SoftphoneID %1").arg(softPhoneID));

//            qDebug() << "updateToneServerCh" << id
////                     << "Address" << addresses[id]
//                     << "ToneServerState" << tone_state
//                     << "PttState" << ptt_state
//                     << "Frequency" << frequency
//                     << "Phase" << phase
//                     << "Level" << level;
            break; // Exit loop once the correct softPhoneID is processed
        }
    }
}
void iGate4x::updateFIRfilter(bool firinput, bool firoutput){
//       qDebug() << "FIRinput11 : " << firinput << "FIRoutput11 : " << firoutput;
//    SigmaFirmWareDownLoad->setFIRfilter(MOD_FIR1_ALG0_FIRSIGMA300ALG1FIRCOEFF0_ADDR, MOD_FIR1_COUNT,FIRfilter_stateON_INPUT);
    QSettings *settings;
    const QString cfgfile = FILESETTING;
    qDebug() << "Loading configuration from:" << cfgfile;
    if(QDir::isAbsolutePath(cfgfile)){
        settings = new QSettings(cfgfile,QSettings::IniFormat);
        qDebug() << "Configuration file:" << settings->fileName();
        settings->setValue("FIRFilter/HighPassInput",firinput);
        settings->setValue("FIRFilter/HighPassOutput",firoutput);
        FIRInput = firinput;
        FIROutput = firoutput;
        if(FIRInput == true){
              SigmaFirmWareDownLoad->setFIRfilter(MOD_FIR1_ALG0_FIRSIGMA300ALG1FIRCOEFF0_ADDR, MOD_FIR1_COUNT,FIRfilter_stateON_INPUT);
//                SigmaFirmWareDownLoad->setFIRfilter(MOD_FIR1_2_ALG0_FIRSIGMA300ALG2FIRCOEFF0_ADDR, MOD_FIR1_2_COUNT,FIRfilter_stateON_OUTPUT);
//                qDebug() << "input : ON";
          }
        else if(FIRInput == true){
              SigmaFirmWareDownLoad->setFIRfilter(MOD_FIR2_ALG0_FIRSIGMA300ALG5FIRCOEFF0_ADDR, MOD_FIR2_COUNT,FIRfilter_stateON_OUTPUT);
//                qDebug() << "output : ON";
        }
        else {
              SigmaFirmWareDownLoad->setFIRfilter(MOD_FIR1_ALG0_FIRSIGMA300ALG1FIRCOEFF0_ADDR,MOD_FIR1_COUNT,FIRfilter_stateOFF_INPUT);
              SigmaFirmWareDownLoad->setFIRfilter(MOD_FIR2_ALG0_FIRSIGMA300ALG5FIRCOEFF0_ADDR, MOD_FIR2_COUNT,FIRfilter_stateOFF_OUTPUT);
        }
       }
      delete settings;

}
void iGate4x::updateURILits(QStringList urlList, int softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    m_softPhone_list.at(i)->uriAllowList = urlList;
    myDatabase->updateAllowList(softPhoneID,urlList);
}
void iGate4x::updateRxBestSignalEnable(bool val, int softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    m_softPhone_list.at(i)->rxBestSignalEnable = val;
    myDatabase->updateRxBestSignalEnable(val,softPhoneID);
}
void iGate4x::updateGroupMute(uint8_t val, int softPhoneID)
{
    Q_FOREACH(SoftPhone *softPhone, m_softPhone_list)
    {
        if ((softPhone->softPhoneID == softPhoneID) & (softPhone->socketClients != nullptr))
        {
            softPhone->groupMute = val;
            QString  message = QString("{"
                                       "\"menuID\"               :\"updateGroupMute\", "
                                       "\"groupMute\"            :%1, "
                                       "\"softPhoneID\"          :%2 }"
                                       ).arg(softPhone->groupMute).arg(softPhoneID);
            SocketServer->sendMessage(softPhone->socketClients,message);
        }
    }
    myDatabase->updateGroupMute(val,softPhoneID);
}
void iGate4x::updatePttDelay(uint8_t val, int softPhoneID)
{
    Q_FOREACH(SoftPhone *softPhone, m_softPhone_list)
    {
        if ((softPhone->softPhoneID == softPhoneID) & (softPhone->socketClients != nullptr))
        {
            softPhone->pttDelay = val;
            QString  message = QString("{"
                                       "\"menuID\"               :\"updatePttDelay\", "
                                       "\"pttDelay\"            :%1, "
                                       "\"softPhoneID\"          :%2 }"
                                       ).arg(softPhone->pttDelay).arg(softPhoneID);
            SocketServer->sendMessage(softPhone->socketClients,message);
        }
    }
    myDatabase->updatePttDelay(val,softPhoneID);
}
void iGate4x::updateNumconn(int numconn, int softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    m_softPhone_list.at(i)->numConnection = numconn;
    QStringList uriAllowList  = m_softPhone_list.at(i)->uriAllowList;
    QString uri1 = uriAllowList.at(0);
    QString uri2 = uriAllowList.at(1);
    QString uri3 = uriAllowList.at(2);
    QString uri4 = uriAllowList.at(3);
    QString uri5 = uriAllowList.at(4);
    QString uri6 = uriAllowList.at(5);
    QString uri7 = uriAllowList.at(6);
    QString uri8 = uriAllowList.at(7);
    SocketServer->updateAllowedUri(softPhoneID, numconn, uri1, uri2, uri3, uri4, uri5, uri6, uri7, uri8);
    saveController(softPhoneID);

}
void iGate4x::updateSQLDefeat(bool sqlDefeat, int softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    m_softPhone_list.at(i)->sqlAlwayOn = sqlDefeat;
    saveController(softPhoneID);
}
void iGate4x::updateRadioActive(bool radioAutoInactive, bool txMainRadioUsed, bool rxMainRadioUsed, int radioMainStandby, int softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    m_softPhone_list.at(i)->mainRadioTransmitterUsed = txMainRadioUsed;
    m_softPhone_list.at(i)->mainRadioReceiverUsed = rxMainRadioUsed;
    m_softPhone_list.at(i)->radioAutoInactive = radioAutoInactive;
    qDebug() << "updateRadioActive m_softPhone_list.at(i)->radioMainStandby = radioMainStandby;" << m_softPhone_list.at(i)->radioMainStandby;
    m_softPhone_list.at(i)->radioMainStandby = radioMainStandby;

    saveController(softPhoneID);
}
void iGate4x::updateRadioAutoInactive(bool value, int softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    m_softPhone_list.at(i)->radioAutoInactive = value;

    saveController(softPhoneID);
    qDebug() << "updateRadioAutoInactive" << softPhoneID << m_softPhone_list.at(i)->radioAutoInactive;
}
void iGate4x::updateMainRadioReceiverUsed(bool value, int softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    m_softPhone_list.at(i)->mainRadioReceiverUsed = value;

    saveController(softPhoneID);
}
void iGate4x::updateMainRadioTransmitterUsed(bool value, int softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    m_softPhone_list.at(i)->mainRadioTransmitterUsed = value;

    saveController(softPhoneID);
}
void iGate4x::updateRadioMainStandby(int value, int softPhoneID)
{
//    qDebug() << "softPhoneID" << softPhoneID << "updateRadioMainStandby" << value;
    int i = (softPhoneIDCheck(softPhoneID));
    m_softPhone_list.at(i)->radioMainStandby = value;

    saveController(softPhoneID);
}
void iGate4x::updateSQLActiveHigh(bool sqlActive, int softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    m_softPhone_list.at(i)->sqlActiveHigh = sqlActive;
    saveController(softPhoneID);
}
void iGate4x::updateHostConfig(QString sipuser, int sipport, int keepaliveperoid, QString defaultEthernet, int softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    m_softPhone_list.at(i)->sipUser = sipuser;
    m_softPhone_list.at(i)->keepAlivePeroid = keepaliveperoid;
    m_softPhone_list.at(i)->sipPort = sipport;
    m_softPhone_list.at(i)->defaultEthernet = defaultEthernet;
    saveController(softPhoneID);
}
void iGate4x::updateDeviceName(int softPhoneID, QString deviceName)
{
    int i = (softPhoneIDCheck(softPhoneID));
    m_softPhone_list.at(i)->deviceName = deviceName;
    saveController(softPhoneID);
}

void iGate4x::updateControler(QString sipUser, int keepAlivePeroid, int sipPort, int rtpStartPort, int softPhoneID, int defaultEthernetId)
{
    qDebug() << "updateControler" << sipUser << keepAlivePeroid << sipPort << rtpStartPort << softPhoneID;
    int i = (softPhoneIDCheck(softPhoneID));
    int channelID = 0;
    uint8_t ServerClientMode = 0;
    uint8_t WireConnectMode = 0;
    quint8  txScheduler;
    quint8  numConnection;
    double   sidetone;
    bool    sqlAlwayOn;
    bool    sqlActiveHigh;
    QString defaultEthernet = "eth0";
    int clock_rate             = 8000;
    bool mainRadioTxUsed;
    bool mainRadioRxUsed;
    bool radioAutoInactive;
    int radioMainStandby;
    bool rxBestSignalEnable;
    uint8_t groupMute;
    uint8_t pttDelay;
    bool Tone_state;
    float Tone_frequency;
    int Tone_phase;
    int Tone_level;
    stopAllSoftphone(softPhoneID);
    startAllSoftphone(softPhoneID);

    if (defaultEthernetId == 1) defaultEthernet = "eth1";

    if (channelID == 0){
        onComponentCompleted(softPhoneID,sipUser,channelID);
    }

    m_softPhone_list.at(i)->sipUser = sipUser;
    m_softPhone_list.at(i)->defaultEthernet = defaultEthernet;
    m_softPhone_list.at(i)->sipPort = sipPort;
    channelID = m_softPhone_list.at(i)->channelID;
    m_softPhone_list.at(i)->rtpStartPort = rtpStartPort;
    m_softPhone_list.at(i)->keepAlivePeroid = keepAlivePeroid;
    ServerClientMode = m_softPhone_list.at(i)->ServerClientMode;
    WireConnectMode = m_softPhone_list.at(i)->WireConnectMode;
    txScheduler = m_softPhone_list.at(i)->txScheduler;
    numConnection = m_softPhone_list.at(i)->numConnection;
    sidetone = m_softPhone_list.at(i)->sidetone;
    sqlAlwayOn = m_softPhone_list.at(i)->sqlAlwayOn;
    sqlActiveHigh = m_softPhone_list.at(i)->sqlActiveHigh;
    mainRadioTxUsed = m_softPhone_list.at(i)->mainRadioTransmitterUsed;
    mainRadioRxUsed = m_softPhone_list.at(i)->mainRadioReceiverUsed;
    radioAutoInactive = m_softPhone_list.at(i)->radioAutoInactive;
    radioMainStandby = m_softPhone_list.at(i)->radioMainStandby;
    rxBestSignalEnable = m_softPhone_list.at(i)->rxBestSignalEnable;
    groupMute = m_softPhone_list.at(i)->groupMute;
    pttDelay = m_softPhone_list.at(i)->pttDelay;
    m_softPhone_list.at(i)->softphoneRunning = true;
    Tone_state = m_softPhone_list.at(i)-> tone_state;
    Tone_frequency =  m_softPhone_list.at(i) -> frequency;
    Tone_phase = m_softPhone_list.at(i) -> phase;
    Tone_level = m_softPhone_list.at(i) -> level;
    oled->update_iGateInfo(softPhoneID,0,0,0,0);
    saveController(softPhoneID);

//    updateChannelID(channelID,softPhoneID);
    qDebug() << "startSoftphone" << softPhoneID << sipUser << channelID;
    QString message = QString("{"
                      "\"menuID\"               :\"updateControler2\", "
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
                      "\"groupMute\"                :%21, "
                      "\"pttDelay\"                 :%22, "
                      "\"outputToneState\"          :%23, "
                      "\"outputToneFrequency\"      :%24, "
                      "\"outputTonePhase\"          :%25, "
                      "\"outputToneLevel\"          :%26 "
                      "}"
                      ).arg(softPhoneID).arg(sipUser).arg(sipPort).arg(channelID).arg(rtpStartPort).arg(keepAlivePeroid).arg(defaultEthernet).arg(ServerClientMode).arg(txScheduler)
            .arg(WireConnectMode).arg(numConnection).arg(sidetone).arg(sqlAlwayOn).arg(sqlActiveHigh).arg(clock_rate).arg(mainRadioTxUsed).arg(mainRadioRxUsed).arg(radioAutoInactive).arg(radioMainStandby)
            .arg(rxBestSignalEnable).arg(groupMute).arg(pttDelay).arg(Tone_state).arg(Tone_frequency).arg(Tone_phase).arg(Tone_level);
    if (m_softPhone_list.at(i)->socketClients != nullptr)
        m_softPhone_list.at(i)->socketClients->sendTextMessage(message);
    saveController(softPhoneID);
}

void iGate4x::setRadioController(int RadioController, int softPhoneID)
{
    QSettings *settings;
    const QString cfgfile = FILESETTING;
    qDebug() << "Loading configuration from:" << cfgfile;
    if(QDir::isAbsolutePath(cfgfile)){
        settings = new QSettings(cfgfile,QSettings::IniFormat);
        qDebug() << "Configuration file:" << settings->fileName();
        QString key = QString("RadioController/softPhoneID%1").arg(softPhoneID);
        settings->setValue(key,RadioController);

        switch (softPhoneID) {
        case 1:
            RadioControllerCH1 = RadioController;
            break;
        case 2:
            RadioControllerCH2 = RadioController;
            break;
        case 3:
            RadioControllerCH3 = RadioController;
            break;
        case 4:
            RadioControllerCH4 = RadioController;
            break;

        }
    }
/* **********************************************************************/
    QString commandstop1 = QString("systemctl stop iComRadioControllerCH%1.service").arg(softPhoneID);
    QString commandstop2 = QString("systemctl stop RadioPortCH%1.service").arg(softPhoneID);
    QString commandstop3 = QString("systemctl stop iGateIC7300CH%1.service").arg(softPhoneID);
    QString commandstop4 = QString("systemctl stop iGateRCS3000CH%1.service").arg(softPhoneID);
    QString commandstop5 = QString("systemctl stop iGateA120CH%1.service").arg(softPhoneID);
    QString commandstop6 = QString("systemctl stop iGateERX3003CH%1.service").arg(softPhoneID);
    system(commandstop1.toStdString().c_str());
    system(commandstop2.toStdString().c_str());
    system(commandstop3.toStdString().c_str());
    system(commandstop4.toStdString().c_str());
    system(commandstop5.toStdString().c_str());
    system(commandstop6.toStdString().c_str());
    if(RadioController == 0) {//iCom CommandV2
//        QString commandstop = QString("systemctl stop RadioControllerCH%1.service").arg(softPhoneID);
        QString commandstop1 = QString("systemctl stop iComRadioControllerCH%1.service").arg(softPhoneID);
        QString commandstop2 = QString("systemctl stop RadioPortCH%1.service").arg(softPhoneID);
        QString commandstop3 = QString("systemctl stop iGateIC7300CH%1.service").arg(softPhoneID);
        QString commandstop4 = QString("systemctl stop iGateRCS3000CH%1.service").arg(softPhoneID);
        QString commandstop5 = QString("systemctl stop iGateA120CH%1.service").arg(softPhoneID);
        QString commandstop6 = QString("systemctl stop iGateERX3003CH%1.service").arg(softPhoneID);
//        QString commandstop = QString("systemctl stop RadioPortCH%1.service").arg(softPhoneID);
//        system(commandstop.toStdString().c_str());
//        QThread::msleep(100);
        system(commandstop1.toStdString().c_str());
        system(commandstop2.toStdString().c_str());
        system(commandstop3.toStdString().c_str());
        system(commandstop4.toStdString().c_str());
        system(commandstop5.toStdString().c_str());
        system(commandstop5.toStdString().c_str());
    }
    else if(RadioController == 1) {// Disabled
        QString commandstart = QString("systemctl start iComRadioControllerCH%1.service").arg(softPhoneID);
        system(commandstart.toStdString().c_str());
    }
    else if (RadioController == 2) {
//       QString commandstop = QString("systemctl stop iComRadioControllerCH%1.service").arg(softPhoneID);
       QString commandstart = QString("systemctl start RadioPortCH%1.service").arg(softPhoneID);
       system(commandstart.toStdString().c_str());
    }
    else if (RadioController == 3){
       QString commandstart = QString("systemctl start iGateIC7300CH%1.service").arg(softPhoneID);
       system(commandstart.toStdString().c_str());
    }
    else if (RadioController == 4){
       QString commandstart = QString("systemctl start iGateRCS3000CH%1.service").arg(softPhoneID);
       system(commandstart.toStdString().c_str());
    }
    else if (RadioController == 5){
       QString commandstart = QString("systemctl start iGateA120CH%1.service").arg(softPhoneID);
       system(commandstart.toStdString().c_str());
    }
    else if (RadioController == 6){
       QString commandstart = QString("systemctl start iGateERX3003CH%1.service").arg(softPhoneID);
       system(commandstart.toStdString().c_str());
    }
    delete settings;
}

void iGate4x::updateChannelID(int channelID, int softPhoneID)
{
    for (int i=0;i<100000;i++) ;


    int i = (softPhoneIDCheck(softPhoneID));
    qDebug() << "updateChannelID" << softPhoneID << channelID;
    if (i == m_softPhone_list.size())
        m_softPhone_list.append(new SoftPhone); //Add
    else if(i > m_softPhone_list.size()) return;

    //Update
    QString sipUser;
    int sipPort;
    int rtpStartPort;
    int keepAlivePeroid;
    sipUser = m_softPhone_list.at(i)->sipUser;
    sipPort = m_softPhone_list.at(i)->sipPort;
    QString defaultEthernet = m_softPhone_list.at(i)->defaultEthernet;
    m_softPhone_list.at(i)->channelID = channelID;
    rtpStartPort = m_softPhone_list.at(i)->rtpStartPort;
    keepAlivePeroid = m_softPhone_list.at(i)->keepAlivePeroid;

    saveController(softPhoneID);
    myDatabase->getCallChannel(softPhoneID);
    stopAllSoftphone(softPhoneID);
    startAllSoftphone(softPhoneID);

    if (channelID == 0)
    {
        onComponentCompleted(softPhoneID,sipUser,channelID);
    }
    QWebSocket *socket = m_softPhone_list.at(i)->socketClients;

    if ( m_softPhone_list.at(i)->ServerClientMode == SERVER)
    {
        i = (callChannelIDCheck(softPhoneID));
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

        QString message = QString("{"
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
    QString message = QString("{"
                              "\"menuID\"                :\"updateControler\", "
                              "\"sipUser\"               :\"%1\", "
                              "\"keepAlivePeroid\"       :%2, "
                              "\"sipPort\"               :%3, "
                              "\"rtpStartPort\"          :%4, "
                              "\"channelID\"             :%5, "
                              "\"defaultEthernet\"       :\"%6\", "
                              "\"softPhoneID\"           :%7 "
                      "}"
                      ).arg(sipUser).arg(keepAlivePeroid).arg(sipPort).arg(rtpStartPort).arg(channelID).arg(defaultEthernet).arg(softPhoneID);
    SocketServer->broadcastMessage(message);
}
void iGate4x::addNewChannel(QString channelName, QString mainRadioType, int mainRadio01ID, int mainRadio02ID, bool standbyEnable, QString standbyRadioType, int standbyRadio01ID, int standbyRadio02ID, QString mainRadioName, QString standbyRadioName)
{
    bool sqlquery = myDatabase->addNewChannel(channelName,  mainRadioType,  mainRadio01ID,  mainRadio02ID,  standbyEnable,  standbyRadioType,  standbyRadio01ID,  standbyRadio02ID,  mainRadioName,  standbyRadioName);
    dataLoggerServer->createDataLog(sqlquery,QString("Insert new channel: Name: %1").arg(channelName));

}
void iGate4x::updateChannel(QString channelName, QString mainRadioType, int mainRadio01ID, int mainRadio02ID, bool standbyEnable,
                                    QString standbyRadioType, int standbyRadio01ID, int standbyRadio02ID, QString mainRadioName, QString standbyRadioName, int channelID)
{
    bool sqlquery = myDatabase->updateChannel( channelName,  mainRadioType,  mainRadio01ID,  mainRadio02ID,  standbyEnable,  standbyRadioType,  standbyRadio01ID,  standbyRadio02ID,  mainRadioName,  standbyRadioName,  channelID);

    dataLoggerServer->createDataLog(sqlquery,QString("Modify channel: Name: %1").arg(channelName));
    int softPhoneID = 0;

    Q_FOREACH (SoftPhone *softPhone_list, m_softPhone_list)
    {
        if (softPhone_list->channelID == channelID)
        {
            softPhoneID = softPhone_list->softPhoneID;
            updateChannelID(channelID,softPhoneID);
        }
    }


}

void iGate4x::removeChannel(int channelID)
{
    int softPhoneID = 0;
    QString channelName = myDatabase->getChannelByID(channelID);
    bool sqlquery = myDatabase->removeChannel( channelID);
    dataLoggerServer->createDataLog(sqlquery,QString("Delete channel: Name: %1").arg(channelName));

    Q_FOREACH (SoftPhone *softPhone_list, m_softPhone_list)
    {
        if (softPhone_list->channelID == channelID)
        {
            softPhone_list->channelID = 0;
            softPhoneID = softPhone_list->softPhoneID;
            saveController(softPhoneID);
            updateChannelID(channelID,softPhoneID);            
        }
    }
}
void iGate4x::radioIDUsedCheck(int radioID)
{
    Q_FOREACH (SoftPhone *softPhone_list, m_softPhone_list)
    {
        int softPhoneID = softPhone_list->softPhoneID;
        int channelID = softPhone_list->channelID;
        Q_FOREACH (channel *Channel, channel_call_list)
        {
            if (channelID == Channel->id)
            {
                if ((Channel->mainRadio01ID == radioID) || (Channel->mainRadio02ID == radioID) || (Channel->standbyRadio01ID == radioID) || (Channel->standbyRadio02ID == radioID))
                {
                    updateChannelID(channelID,softPhoneID);
                }
            }
        }
    }
}

void iGate4x::radioIDRemoveUsedCheck(int radioID)
{
    Q_FOREACH (SoftPhone *softPhone_list, m_softPhone_list)
    {
        int softPhoneID = softPhone_list->softPhoneID;
        int channelID = softPhone_list->channelID;
        Q_FOREACH (channel *Channel, channel_call_list)
        {
            if (channelID == Channel->id)
            {
                if ((Channel->mainRadio01ID == radioID) || (Channel->mainRadio02ID == radioID) || (Channel->standbyRadio01ID == radioID) || (Channel->standbyRadio02ID == radioID))
                {
                    removeChannel(channelID);
                }
            }
        }
    }
}

void iGate4x::addNewRadio(QString radioName, QString trxMode, QString uri, QString ipAddress, int r2sPeriod,int sipPort, QString frequency)
{
    bool sqlquery = myDatabase->addNewRadio( radioName,  trxMode,  uri,  ipAddress,  r2sPeriod, sipPort,  frequency);
    dataLoggerServer->createDataLog(sqlquery,QString("Insert New Radio: Name: %1, SIP sip:%2@%3:%4, TRX Mode: %5, r2sPeriod: %6 msec").arg(radioName).arg(uri).arg(ipAddress).arg(sipPort).arg(trxMode).arg(r2sPeriod));

}
void iGate4x::updateRadio(QString radioName, QString trxMode, QString uri, QString ipAddress, int r2sPeriod, int sipPort, QString frequency, int radioID)
{
   bool sqlquery = myDatabase->updateRadio( radioName,  trxMode,  uri,  ipAddress,  r2sPeriod,  sipPort,  frequency,  radioID);
   dataLoggerServer->createDataLog(sqlquery,QString("Modify Radio: Name: %1, SIP sip:%2@%3:%4, TRX Mode: %5, r2sPeriod: %6 msec").arg(radioName).arg(uri).arg(ipAddress).arg(sipPort).arg(trxMode).arg(r2sPeriod));
   radioIDUsedCheck(radioID);
}

void iGate4x::removeRadio(int radioID)
{
    QStringList radioData = myDatabase->getRadioByID(radioID);
    if (radioData.size() >= 9)
    {
        int id = radioData.at(0).toInt();
        QString radioName = radioData.at(1);
        QString trxMode = radioData.at(2);
        QString uri = radioData.at(3);
        int r2sPeriod = radioData.at(4).toInt();
        QString ipAddress = radioData.at(5);
        int rtpPort = radioData.at(6).toInt();
        int frequency = radioData.at(7).toInt();
        int sipPort = radioData.at(8).toInt();

        dataLoggerServer->createDataLog(INFO,QString("Delete Radio: Name: %1, SIP sip:%2@%3:%4, TRX Mode: %5, r2sPeriod: %6 msec").arg(radioName).arg(uri).arg(ipAddress).arg(sipPort).arg(trxMode).arg(r2sPeriod));
    }

    radioData.clear();

    myDatabase->removeRadio(radioID);
    radioIDRemoveUsedCheck(radioID);
}
void iGate4x::Truncatesyslog()
{
    system("truncate -s 0 /var/log/syslog");
    qDebug() << "truncate -s 0 /var/log/syslog";
}
void iGate4x::saveController(int softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    int sipPort = m_softPhone_list.at(i)->sipPort;
    int rtpStartPort = m_softPhone_list.at(i)->rtpStartPort;
    int keepAlivePeroid = m_softPhone_list.at(i)->keepAlivePeroid;
    QString sipUser = m_softPhone_list.at(i)->sipUser;
    int channelID = m_softPhone_list.at(i)->channelID;
    bool WireConnectMode = m_softPhone_list.at(i)->WireConnectMode;
    bool mainRadioReceiverUsed = m_softPhone_list.at(i)->mainRadioReceiverUsed;
    bool mainRadioTransmitterUsed = m_softPhone_list.at(i)->mainRadioTransmitterUsed;
    uint8_t trxMode = m_softPhone_list.at(i)->trxMode;
    uint8_t ServerClientMode = m_softPhone_list.at(i)->ServerClientMode;
    uint8_t txScheduler = m_softPhone_list.at(i)->txScheduler;
    bool sqlActiveHigh = m_softPhone_list.at(i)->sqlActiveHigh;
    uint8_t numConnection = m_softPhone_list.at(i)->numConnection;
    double sidetone = m_softPhone_list.at(i)->sidetone;
    bool sqlAlwayOn = m_softPhone_list.at(i)->sqlAlwayOn;
    QString deviceName = m_softPhone_list.at(i)->deviceName;
    uint8_t inputLevel = m_softPhone_list.at(i)->inputLevel;
    uint8_t outputLevel = m_softPhone_list.at(i)->outputLevel;
    uint8_t outputDSPLevel = m_softPhone_list.at(i)->outputDSPLevel;
    bool radioAutoInactive = m_softPhone_list.at(i)->radioAutoInactive;
    int radioMainStandby = m_softPhone_list.at(i)->radioMainStandby;
    QString defaultEthernet = m_softPhone_list.at(i)->defaultEthernet;

    myDatabase->updateControler( sipPort, rtpStartPort, keepAlivePeroid, sipUser, channelID, softPhoneID, WireConnectMode, mainRadioReceiverUsed, mainRadioTransmitterUsed, trxMode,
                                 ServerClientMode, txScheduler, sqlActiveHigh, numConnection, sidetone, sqlAlwayOn, deviceName, inputLevel, outputLevel,radioAutoInactive,radioMainStandby,
                                 defaultEthernet,outputDSPLevel);
}
void iGate4x::saveToneData(int softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    bool Tone_state = m_softPhone_list.at(i)-> tone_state ;
    float Tone_frequency =  m_softPhone_list.at(i) -> frequency;
    int Tone_phase = m_softPhone_list.at(i) -> phase;
    int Tnone_level = m_softPhone_list.at(i) -> level;

    myDatabase->updateControler(Tone_state,Tone_frequency,Tone_phase,Tnone_level,softPhoneID);
}

void iGate4x::saveTestRadioPage(int softPhoneID)
{
    int i = (softPhoneIDCheck(softPhoneID));
    bool Tone_server_state = m_softPhone_list.at(i)->tone_server_state;
    bool Tone_server_ptt = m_softPhone_list.at(i)-> tone_server_ptt;
    float tone_server_frequency = m_softPhone_list.at(i)->tone_server_frequency;
    int tone_server_phase = m_softPhone_list.at(i)->tone_server_phase;
    float tone_server_level = m_softPhone_list.at(i)->tone_server_level;
    float tone_server_ptt_timeout = m_softPhone_list.at(i)->tone_server_ptt_timeout;
    myDatabase->updateRediotest(Tone_server_state,Tone_server_ptt,tone_server_ptt_timeout,tone_server_frequency,tone_server_phase,tone_server_level,softPhoneID);
}


void iGate4x::updateSidetone(float sidetone,int softPhoneID)
{
    myDatabase->updateSidetone(sidetone,softPhoneID);
    Q_FOREACH(SoftPhone *softPhone, m_softPhone_list)
    {
        if ((softPhone->softPhoneID == softPhoneID) & (softPhone->socketClients != nullptr))
        {
            softPhone->sidetone = sidetone;
            QString  message = QString("{"
                                       "\"menuID\"               :\"updateSidetone\", "
                                       "\"sidetone\"             :%1, "
                                       "\"softPhoneID\"          :%2 }"
                                       ).arg(sidetone).arg(softPhoneID);
            SocketServer->sendMessage(softPhone->socketClients,message);
            break;
        }
    }
}
void iGate4x::updateLocalSidetone(float sidetone,int softPhoneID)
{
    myDatabase->updateLocalSidetone(sidetone,softPhoneID);

    Q_FOREACH(SoftPhone *softPhone, m_softPhone_list)
    {
        if ((softPhone->softPhoneID == softPhoneID) & (softPhone->socketClients != nullptr))
        {
            softPhone->localSidetone = sidetone;
            QString  message = QString("{"
                                       "\"menuID\"               :\"updateLocalSidetone\", "
                                       "\"localSidetone\"             :%1, "
                                       "\"softPhoneID\"          :%2 }"
                                       ).arg(sidetone).arg(softPhoneID);
            SocketServer->sendMessage(softPhone->socketClients,message);

            switch (softPhoneID)
            {
            case 1:
                if (softPhone->localSidetoneOn)
                    SigmaFirmWareDownLoad->setMixerVolume(SIDETONE_VOLUME_CH1_ADDRESS,SIDETONE_VOLUME_CH1_MODE_ADDRESS,SIDETONE_VOLUME_CH1_MODE_VALUE,sidetone);
                break;
            case 2:
                if (softPhone->localSidetoneOn)
                    SigmaFirmWareDownLoad->setMixerVolume(SIDETONE_VOLUME_CH2_ADDRESS,SIDETONE_VOLUME_CH2_MODE_ADDRESS,SIDETONE_VOLUME_CH2_MODE_VALUE,sidetone);
                break;
            case 3:
                if (softPhone->localSidetoneOn)
                    SigmaFirmWareDownLoad->setMixerVolume(SIDETONE_VOLUME_CH3_ADDRESS,SIDETONE_VOLUME_CH3_MODE_ADDRESS,SIDETONE_VOLUME_CH3_MODE_VALUE,sidetone);
                break;
            case 4:
                if (softPhone->localSidetoneOn)
                    SigmaFirmWareDownLoad->setMixerVolume(SIDETONE_VOLUME_CH4_ADDRESS,SIDETONE_VOLUME_CH4_MODE_ADDRESS,SIDETONE_VOLUME_CH4_MODE_VALUE,sidetone);
                break;
            }

            break;
        }
    }
}
void iGate4x::restartSoftphone()
{
    Q_FOREACH(SoftPhone *softPhone, m_softPhone_list){
        if (softPhone->socketClients != nullptr)
        {
            QString  message = QString("{"
                                       "\"menuID\"               :\"restartSoftphone\", "
                                       "\"softPhoneID\"            :%1 }"
                                       ).arg(softPhone->softPhoneID);
            SocketServer->sendMessage(softPhone->socketClients,message);
        }
    }
}
void iGate4x::restartSoftphone(int softPhoneID)
{
    Q_FOREACH(SoftPhone *softPhone, m_softPhone_list){
        if ((softPhone->softPhoneID == softPhoneID) & (softPhone->socketClients != nullptr))
        {
            QString  message = QString("{"
                                       "\"menuID\"               :\"restartSoftphone\", "
                                       "\"softPhoneID\"            :%1 }"
                                       ).arg(softPhoneID);
            SocketServer->sendMessage(softPhone->socketClients,message);
            break;
        }
    }
}
void iGate4x::restartnetwork(){
    networking->resetNetwork2();
}

void iGate4x::slotupdateNetwork(uint8_t DHCP, QString LocalAddress, QString Netmask, QString Gateway, QString DNS1, QString DNS2,QString phyNetworkName)
{
    updateNetwork(DHCP, LocalAddress, Netmask, Gateway, DNS1, DNS2, phyNetworkName);
}

void iGate4x::updateNetwork(uint8_t DHCP, QString LocalAddress, QString Netmask, QString Gateway, QString DNS1, QString DNS2,QString phyNetworkName){
    QSettings *settings;
    const QString cfgfile = FILESETTING;
    qDebug() << "Loading configuration from:" << cfgfile;
//    if (eth0.dhcpmethod.contains("on") == false)
//        networking->initPhyNetworkConfig(eth0.ipaddress,eth0.subnet.toInt(),eth0.gateway,eth0.pridns,eth0.secdns,"eth0");
//    if (eth1.dhcpmethod.contains("on") == false)
//        networking->initPhyNetworkConfig(eth1.ipaddress,eth1.subnet.toInt(),eth1.gateway,eth1.pridns,eth1.secdns,"eth1");
//    if (bond0.dhcpmethod.contains("on") == false)
//        networking->initPhyNetworkConfig(bond0.ipaddress,bond0.subnet.toInt(),bond0.gateway,bond0.pridns,bond0.secdns,"bond0");

    if(QDir::isAbsolutePath(cfgfile)){
        settings = new QSettings(cfgfile,QSettings::IniFormat);
        qDebug() << "Configuration file:" << settings->fileName();
        QString strDhcpMethod = "off";
        if (DHCP) strDhcpMethod = "on";


        if(phyNetworkName == "eth0")
        {
            settings->setValue("NETWORK_ETH0/DHCP",strDhcpMethod);
            settings->setValue("NETWORK_ETH0/LocalAddress",LocalAddress);
            settings->setValue("NETWORK_ETH0/Netmask",Netmask);
            settings->setValue("NETWORK_ETH0/Gateway",Gateway);
            settings->setValue("NETWORK_ETH0/DNS1",DNS1);
            settings->setValue("NETWORK_ETH0/DNS2",DNS2);
        }
        else if (phyNetworkName == "eth1")
        {
            settings->setValue("NETWORK_ETH1/DHCP",strDhcpMethod);
            settings->setValue("NETWORK_ETH1/LocalAddress",LocalAddress);
            settings->setValue("NETWORK_ETH1/Netmask",Netmask);
            settings->setValue("NETWORK_ETH1/Gateway",Gateway);
            settings->setValue("NETWORK_ETH1/DNS1",DNS1);
            settings->setValue("NETWORK_ETH1/DNS2",DNS2);
        }
        else if (phyNetworkName == "bond0")
        {
            settings->setValue("NETWORK_ETH0/DHCP",strDhcpMethod);
            settings->setValue("NETWORK_ETH0/LocalAddress",LocalAddress);
            settings->setValue("NETWORK_ETH0/Netmask",Netmask);
            settings->setValue("NETWORK_ETH0/Gateway",Gateway);
            settings->setValue("NETWORK_ETH0/DNS1",DNS1);
            settings->setValue("NETWORK_ETH0/DNS2",DNS2);

            settings->setValue("NETWORK_BOND0/DHCP",strDhcpMethod);
            settings->setValue("NETWORK_BOND0/LocalAddress",LocalAddress);
            settings->setValue("NETWORK_BOND0/Netmask",Netmask);
            settings->setValue("NETWORK_BOND0/Gateway",Gateway);
            settings->setValue("NETWORK_BOND0/DNS1",DNS1);
            settings->setValue("NETWORK_BOND0/DNS2",DNS2);
        }

        if (DHCP)
        {
            networking->setDHCPIpAddr3(phyNetworkName);
        }
        else{
            networking->setStaticIpAddr3(LocalAddress,Netmask,Gateway,DNS1,DNS2,phyNetworkName);
        }

        if(phyNetworkName == "eth0")
        {
            eth0.dhcpmethod = strDhcpMethod;
            eth0.ipaddress = LocalAddress;
            eth0.subnet = Netmask;
            eth0.gateway = Gateway;
            eth0.pridns = DNS1;
            eth0.secdns = DNS2;
        }
        else if (phyNetworkName == "eth1")
        {
            eth1.dhcpmethod = strDhcpMethod;
            eth1.ipaddress = LocalAddress;
            eth1.subnet = Netmask;
            eth1.gateway = Gateway;
            eth1.pridns = DNS1;
            eth1.secdns = DNS2;
        }

        else if (phyNetworkName == "bond0")
        {
            eth0.dhcpmethod = strDhcpMethod;
            eth0.ipaddress = LocalAddress;
            eth0.subnet = Netmask;
            eth0.gateway = Gateway;
            eth0.pridns = DNS1;
            eth0.secdns = DNS2;

            bond0.dhcpmethod = strDhcpMethod;
            bond0.ipaddress = LocalAddress;
            bond0.subnet = Netmask;
            bond0.gateway = Gateway;
            bond0.pridns = DNS1;
            bond0.secdns = DNS2;

            restartSoftphone();
        }

        //restartnetwork();
    }
    delete settings;
}

void iGate4x::myConfigurate()
{
    QSettings *settings;
    const QString cfgfile = FILESETTING;
    qDebug() << "Loading configuration from:" << cfgfile;
    if(QDir::isAbsolutePath(cfgfile))
    {
        settings = new QSettings(cfgfile,QSettings::IniFormat);
        qDebug() << "Configuration file:" << settings->fileName();
        FIRInput                 = settings->value("FIRFilter/HighPassInput","true").toString().contains("true");
        FIROutput                = settings->value("FIRFilter/HighPassOutput","true").toString().contains("true");

        ntpServer                = settings->value("NETWORK/ntpServer","").toString();
        ntp                      = settings->value("NETWORK/ntp","true").toString().contains("true");
        Networktimeout           = settings->value("NETWORK/timeoutnetwork",100).toInt();
        eth0.dhcpmethod          = settings->value("NETWORK_ETH0/DHCP","on").toString();
        eth0.ipaddress           = settings->value("NETWORK_ETH0/LocalAddress","192.168.10.9").toString();
        eth0.subnet              = settings->value("NETWORK_ETH0/Netmask","255.255.255.0").toString();
        eth0.gateway             = settings->value("NETWORK_ETH0/Gateway","").toString();
        eth0.pridns              = settings->value("NETWORK_ETH0/DNS1","").toString();
        eth0.secdns              = settings->value("NETWORK_ETH0/DNS2","").toString();


        eth1.dhcpmethod          = settings->value("NETWORK_ETH1/DHCP","on").toString();
        eth1.ipaddress           = settings->value("NETWORK_ETH1/LocalAddress","192.168.10.9").toString();
        eth1.subnet              = settings->value("NETWORK_ETH1/Netmask","255.255.255.0").toString();
        eth1.gateway             = settings->value("NETWORK_ETH1/Gateway","").toString();
        eth1.pridns              = settings->value("NETWORK_ETH1/DNS1","").toString();
        eth1.secdns              = settings->value("NETWORK_ETH1/DNS2","").toString();


        bond0.dhcpmethod         = settings->value("NETWORK_BOND0/DHCP","on").toString();
        bond0.ipaddress          = settings->value("NETWORK_BOND0/LocalAddress","192.168.10.9").toString();
        bond0.subnet             = settings->value("NETWORK_BOND0/Netmask","255.255.255.0").toString();
        bond0.gateway            = settings->value("NETWORK_BOND0/Gateway","").toString();
        bond0.pridns             = settings->value("NETWORK_BOND0/DNS1","").toString();
        bond0.secdns             = settings->value("NETWORK_BOND0/DNS2","").toString();
        inviteMode               = settings->value("SIP_SETTINGS/inviteMode",SERVER).toInt();

        timeLocation             = settings->value("NETWORK/timeLocation","").toString();
        RadioControllerCH1       = settings->value("RadioController/softPhoneID1",1).toInt();
        RadioControllerCH2       = settings->value("RadioController/softPhoneID2",1).toInt();
        RadioControllerCH3       = settings->value("RadioController/softPhoneID3",1).toInt();
        RadioControllerCH4       = settings->value("RadioController/softPhoneID4",1).toInt();

        enableDataLogger         = settings->value("SIP_SETTINGS/enableDataLogger","false").toString().contains("true");
    }
    else{
        qDebug() << "Loading configuration from:" << cfgfile << " FILE NOT FOUND!";
    }
    qDebug() << "Loading configuration completed";
    delete settings;
}


void iGate4x::updateswitchInviteMode(int value)
{
    qDebug() << "updateswitchInviteMode" << value;
    inviteMode = myDatabase->getInviteMode();
    if ((value > 0) & (value <= SERVER_RPT))
    {
        if (value != inviteMode)
        {
            QSettings *settings;
            const QString cfgfile = FILESETTING;
            qDebug() << "Loading configuration from:" << cfgfile;
            if(QDir::isAbsolutePath(cfgfile)){
                settings = new QSettings(cfgfile,QSettings::IniFormat);
                qDebug() << "Configuration file:" << settings->fileName();
                settings->setValue("SIP_SETTINGS/inviteMode",value);
            }
            else{
                qDebug() << "Loading configuration from:" << cfgfile << " FILE NOT FOUND!";
            }
            delete settings;

            inviteMode = value;
            myDatabase->updateInviteMode(inviteMode);
            Q_FOREACH (SoftPhone *softPhone_list, m_softPhone_list)
            {
                softPhone_list->ServerClientMode = inviteMode;
            }
            stopAllSoftphone(0);
            oled->update_iGateInfo(1,0,0,0,0);
            oled->update_iGateInfo(2,0,0,0,0);
            oled->update_iGateInfo(3,0,0,0,0);
            sleep(1);
            system("systemctl restart iGate4x.service");
        }
    }
}
void iGate4x::getSystemPage(QWebSocket *webSender)
{
    int dateTimeMethod;
    if (ntp){
        dateTimeMethod = 1;
    }else{
        dateTimeMethod = 2;
    }

    QString message = QString("{\"menuID\":\"system\", \"SwVersion\":\"%1\", \"HwVersion\":\"%2\", \"dateTimeMethod\":\"%3\", \"ntpServer\":\"%4\", \"location\":\"%5\", \"inviteMode\":%6}")
            .arg(SwVersion).arg(HwVersion).arg(dateTimeMethod).arg(ntpServer).arg(timeLocation).arg(inviteMode);
    SocketServer->sendToWebMessageClient(message,webSender);
    dcInChanged();
}

void iGate4x::updateNTPServer(QString value)
{
    QSettings *settings;
    const QString cfgfile = FILESETTING;
    qDebug() << "Loading configuration from:" << cfgfile;
    if(QDir::isAbsolutePath(cfgfile)){
        settings = new QSettings(cfgfile,QSettings::IniFormat);
        qDebug() << "Configuration file:" << settings->fileName();
        settings->setValue("NETWORK/ntpServer",value);
        settings->setValue("NETWORK/ntp","true");
        networking->setNTPServer(value);
        networking->resetNtp();
        ntpServer=value;
    }
    ntp = true;
    delete settings;
}
void iGate4x::updateNetworkTimeout(int timeout)
{
//    qDebug()<<"networktimeout"<<timeout;
//    networking->setTimeOutNetwork(timeout);
    QSettings *settings;
    const QString cfgfile = FILESETTING;
    qDebug() << "Loading configuration from:" << cfgfile;
    if(QDir::isAbsolutePath(cfgfile)){
        settings = new QSettings(cfgfile,QSettings::IniFormat);
        qDebug() << "Configuration file:" << settings->fileName();
        settings->setValue("NETWORK/timeoutnetwork",timeout);
        Networktimeout = timeout;
        networking->setTimeOutNetwork(timeout);
    }
      delete settings;
}

void iGate4x::setLocation(QString location){
    if (!location.contains("Select")){
        QString command = QString("ln -sf /usr/share/zoneinfo/%1  /etc/localtime").arg(location);
        system(command.toStdString().c_str());
        timeLocation = location;
    }
    QSettings *settings;
    const QString cfgfile = FILESETTING;
    qDebug() << "Loading configuration from:" << cfgfile;
    if(QDir::isAbsolutePath(cfgfile)){
        settings = new QSettings(cfgfile,QSettings::IniFormat);
        qDebug() << "Configuration file:" << settings->fileName();
        settings->setValue("NETWORK/timeLocation",timeLocation);
    }
}
void iGate4x::setcurrentDatetime(QString dateTime){
    QString date;
    QString time;
    if (dateTime.split(" ").size() >= 2){
        date = QString(dateTime.split(" ").at(0)).replace("/","-");
        time = QString(dateTime.split(" ").at(1))+":00";
        system("timedatectl set-ntp 0");
        QString command = QString("date --set '%1 %2'").arg(date).arg(time);
        system(command.toStdString().c_str());
        system("hwclock -w > /dev/null 2>&1 &");
    }

    QSettings *settings;
    const QString cfgfile = FILESETTING;
    qDebug() << "Loading configuration from:" << cfgfile;
    if(QDir::isAbsolutePath(cfgfile)){
        settings = new QSettings(cfgfile,QSettings::IniFormat);
        qDebug() << "Configuration file:" << settings->fileName();
        settings->setValue("NETWORK/ntp","false");
    }
    ntp = false;
    delete settings;
}

QStringList iGate4x::findFile()
{
    QStringList listfilename;
    QString ss="/var/www/html/uploads/";
    const char *sss ; sss = ss.toStdString().c_str();
    QDir dir1("/var/www/html/uploads/");
    QString filepath;
    QString filename;
    QFileInfoList fi1List( dir1.entryInfoList( QDir::Files, QDir::Name) );
    foreach( const QFileInfo & fi1, fi1List ) {
        filepath = QString::fromUtf8(fi1.absoluteFilePath().toLocal8Bit());
        filename = QString::fromUtf8(fi1.fileName().toLocal8Bit());
        listfilename << filepath;
        qDebug() << filepath;// << filepath.toUtf8().toHex();
    }
    return listfilename;
}
void iGate4x::systemrestore()
{
    sync();
    sleep(1);
    myDatabase->restoreDatabase();
    QSettings *settings;
    const QString cfgfile = FILESETTING;
    qDebug() << "Loading configuration from:" << cfgfile;
    if(QDir::isAbsolutePath(cfgfile)){
        settings = new QSettings(cfgfile,QSettings::IniFormat);
        qDebug() << "Configuration file:" << settings->fileName();
        settings->setValue("NETWORK_ETH0/DHCP",eth0.dhcpmethod);
        settings->setValue("NETWORK_ETH0/LocalAddress",eth0.ipaddress);
        settings->setValue("NETWORK_ETH0/Netmask",eth0.subnet);
        settings->setValue("NETWORK_ETH0/Gateway",eth0.gateway);
        settings->setValue("NETWORK_ETH0/DNS1",eth0.pridns);
        settings->setValue("NETWORK_ETH0/DNS2",eth0.secdns);
        settings->setValue("NETWORK_ETH1/DHCP",eth1.dhcpmethod);
        settings->setValue("NETWORK_ETH1/LocalAddress",eth1.ipaddress);
        settings->setValue("NETWORK_ETH1/Netmask",eth1.subnet);
        settings->setValue("NETWORK_ETH1/Gateway",eth1.gateway);
        settings->setValue("NETWORK_ETH1/DNS1",eth1.pridns);
        settings->setValue("NETWORK_ETH1/DNS2",eth1.secdns);
        settings->setValue("NETWORK_BOND0/DHCP",bond0.dhcpmethod);
        settings->setValue("NETWORK_BOND0/LocalAddress",bond0.ipaddress);
        settings->setValue("NETWORK_BOND0/Netmask",bond0.subnet);
        settings->setValue("NETWORK_BOND0/Gateway",bond0.gateway);
        settings->setValue("NETWORK_BOND0/DNS1",bond0.pridns);
        settings->setValue("NETWORK_BOND0/DNS2",bond0.secdns);
    }
    delete settings;

    sleep(1);
    system("sync");
    system("systemctl restart iGate4x.service");
}
void iGate4x::systembackup()
{
    myDatabase->dumpDatabase();
}
void iGate4x::scanFileUpdate()
{
    QStringList fileupdate;
    fileupdate = findFile();
    if (fileupdate.size() > 0){
        if(foundfileupdate == false)
            updateFirmware();
    }
}

void iGate4x::updateFirmware()
{
    foundfileupdate = true;
    QStringList fileupdate;
    fileupdate = findFile();
    system("mkdir -p /tmp/update");

    if (fileupdate.size() > 0)
    {
        qDebug() << "Start update";
        updateStatus = 1;
        QString message = QString("{\"menuID\":\"update\", \"updateStatus\":%1}").arg(updateStatus);
        emit cppCommand(message);
        SocketServer->sendToWebMessageClient(message);
        QString commandCopyFile = "cp " + QString(fileupdate.at(0)) + " /tmp/update/update.tar";
        system(commandCopyFile.toStdString().c_str());
        system("tar -xf /tmp/update/update.tar -C /tmp/update/");
        system("sh /tmp/update/update.sh");
        updateStatus = 2;
        message = QString("{\"menuID\":\"update\", \"updateStatus\":%1}").arg(updateStatus);
        emit cppCommand(message);
        SocketServer->sendToWebMessageClient(message);
        qDebug() << "Update complete";
//        exit(0);
    }
    foundfileupdate = false;
}


void iGate4x::rebootSystem()
{
    stopAllSoftphone(0);
    for (int i=0;i<100;i++) {
        QThread::msleep(11);
    }
    system("systemctl restart iGate4x.service");
}


void iGate4x::resetDSP()
{
    dspReset->resetGpio();
    QThread::msleep(100);
    dspReset->setGpio();
}

void iGate4x::DSPBootSelect(bool qspiflash)
{
    if (qspiflash)
    {
        dspBootSelect->setGpio();
    }
    else
    {
        dspBootSelect->resetGpio();
    }
}
