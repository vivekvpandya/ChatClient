#include "chatroom.h"
#include "ui_chatroom.h"
#include <QDebug>
#include <peer.h>
#include <message.h>
#include <QNetworkInterface>
chatRoom::chatRoom(QString mnickName,Room room,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatRoom)
{
    ui->setupUi(this);
    nickName = mnickName;
    m_room = room;
    groupAddress = QHostAddress("239.255.43.21");



    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::AnyIPv4, m_room.getPort(), QUdpSocket::ShareAddress);
    udpSocket->joinMulticastGroup(groupAddress);
    qDebug() << nickName + " has joined the room";
    qDebug() << QString::number(m_room.getPort()) + " Room port number ";
   connect(udpSocket, SIGNAL(readyRead()),this, SLOT(processPendingDatagrams()));
    ui->roomDescTopic->setText(m_room.getRoomDesc());
    QSet<Peer> peerSet = m_room.getJoinedNickNames();
    for(Peer peer: peerSet ){
    ui->peerList->addItem(peer.getNickName());
    }


    Message message = Message(MessageType::JoinRoom); // A joinRoom Message
    message.insertDataString(m_room.getRoomName());
    QHostAddress ownIPaddress;
        foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
                ownIPaddress = address;
        }
    Peer peer = Peer(nickName,ownIPaddress);
    message.insertPeerObj(peer);
    QByteArray datagram;
        QDataStream out(&datagram, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_5);
        out << message;

        udpSocket->writeDatagram(datagram, groupAddress, m_room.getPort());

}


chatRoom::~chatRoom()
{
    delete ui;
}

void chatRoom::closeEvent(QCloseEvent *event){
    qDebug() << windowTitle();

    Message message = Message(MessageType::LeaveRoom); // A leave Message
    message.insertDataString(m_room.getRoomName());
    QHostAddress ownIPaddress;
        foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
                ownIPaddress = address;
        }
    Peer peer = Peer(nickName,ownIPaddress);
    message.insertPeerObj(peer);
    QByteArray datagram;
        QDataStream out(&datagram, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_5);
        out << message;

    udpSocket->writeDatagram(datagram, groupAddress, m_room.getPort());
    emit leaveChatRoom(windowTitle());


}

void chatRoom::processPendingDatagrams()
{   Message message;
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        //statusLabel->setText(tr("Received datagram: \"%1\"").arg(datagram.data()));
        qDebug()<<"got here";

        QDataStream in(&datagram, QIODevice::ReadOnly);
           in.setVersion(QDataStream::Qt_5_5);
           in >> message;
           MessageType mtype = message.getMessageType();
           if(mtype == MessageType::Message){
               std::vector<QString> dataStrings = message.getDataStrings();
                QString chatMessage = dataStrings.at(1);
                QString nickName = dataStrings.at(0);
                qDebug() << chatMessage;
                ui->chatArea->append(nickName+" : "+chatMessage);//Need to do some formating
           }
           else if(mtype == MessageType::JoinRoom){
               for(Peer peer:message.getPeerVector()){
                m_room.addNickName(peer);
                ui->chatArea->append("~~~~~~~~~~~~~~~~~~~~ "+peer.getNickName()+" has jonied. ~~~~~~~~~~~~~~~~");
                ui->peerList->addItem(peer.getNickName());
               }


           }
           else if(mtype == MessageType::LeaveRoom){

               for(Peer peer:message.getPeerVector()){
                m_room.removeNickName(peer);
                ui->chatArea->append("~~~~~~~~~~~~~~~~~~~~ "+peer.getNickName()+" has left. ~~~~~~~~~~~~~~~~");
                Qt::MatchFlags flag = Qt::MatchFlags(Qt::MatchFixedString||Qt::MatchCaseSensitive);
            QList<QListWidgetItem*> listOfPeer =    ui->peerList->findItems(peer.getNickName(),flag);
              for(QListWidgetItem* peerWidgetItem:listOfPeer){
                  qDebug()<<"element here!";

             ui->peerList->takeItem(ui->peerList->row(peerWidgetItem));
              }
               }

           }

       /*
        QString chatMessage = QString::fromLatin1(datagram);
        qDebug() << chatMessage;
        ui->chatArea->append(chatMessage);*/
    }
}



void chatRoom::on_sendBtn_clicked()
{
    QString chatMsg = ui->chatMsg->toPlainText();
    /*QString  datagramStr = nickName+":" + chatMsg;
    qDebug()<<"Msg to be sent " + datagramStr;
    QByteArray datagram = datagramStr.toUtf8();
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             groupAddress, m_room.getPort());*/
    Message message = Message(MessageType::Message); // A generic Message
    message.insertDataString(nickName);
    message.insertDataString(chatMsg);
    QByteArray datagram;
        QDataStream out(&datagram, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_5);
        out << message;

        udpSocket->writeDatagram(datagram, groupAddress, m_room.getPort());
}

void chatRoom::on_peerList_itemDoubleClicked(QListWidgetItem *item)
{
       QString peerName = ui->peerList->currentItem()->text();
       QSet<Peer> peerSet = m_room.getJoinedNickNames();
       for(Peer peer: peerSet){
           if(peerName == peer.getNickName())
               emit emitUnicast(peer);
       }

}
