#include "chatroom.h"
#include "ui_chatroom.h"
#include <QDebug>

chatRoom::chatRoom(QString mnickName,int mportNumber,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatRoom)
{
    ui->setupUi(this);
    nickName = mnickName;
    portNumber = mportNumber;
    groupAddress = QHostAddress("239.255.43.21");



    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::AnyIPv4, portNumber, QUdpSocket::ShareAddress);
    udpSocket->joinMulticastGroup(groupAddress);
    qDebug() << nickName + " has joined the room";
    qDebug() << QString::number(portNumber) + " Room port number ";
   connect(udpSocket, SIGNAL(readyRead()),this, SLOT(processPendingDatagrams()));
}


chatRoom::~chatRoom()
{
    delete ui;
}

void chatRoom::closeEvent(QCloseEvent *event){
    qDebug() << windowTitle();
    emit leaveChatRoom(windowTitle());


}

void chatRoom::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        //statusLabel->setText(tr("Received datagram: \"%1\"").arg(datagram.data()));
        qDebug()<<"got here";
        QString chatMessage = QString::fromLatin1(datagram);
        qDebug() << chatMessage;
        ui->chatArea->append(chatMessage);
    }
}



void chatRoom::on_sendBtn_clicked()
{
    QString chatMsg = ui->chatMsg->toPlainText();
    QString  datagramStr = nickName+":" + chatMsg;
    qDebug()<<"Msg to be sent " + datagramStr;
    QByteArray datagram = datagramStr.toUtf8();
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             groupAddress, portNumber);
}
