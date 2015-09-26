#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDataStream>
#include "sample.h"
#include "peer.h"
#include "message.h"
#include <chatroom.h>
#include <QNetworkInterface>
#include <unicastchat.h>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
    //connect(socket,SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));

    tcpServerForUnicast = new QTcpServer(this);

    // whenever a user connects, it will emit signal
    connect(tcpServerForUnicast, SIGNAL(newConnection()), this, SLOT(newConnectionForUnicast()));

    if(!tcpServerForUnicast->listen(QHostAddress::Any, 17000))
    {   // this message should be added to GUI
         qDebug() << "Server could not start";
    }
    else{

        qDebug() << "Server started!";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::on_connectBtn_clicked()
{
    QString hostAddress = ui->hostAddress->text();
    QString portNumber = ui->portNum->text();
    qint64 portNum = portNumber.toLongLong();
    nickNameStr = ui->nickName->text();
    if (nickNameStr.isEmpty() == true){
        QMessageBox nickerror;
        nickerror.setText("Nick Name is compulsory");
        nickerror.exec();
       // ui->setupUi(this);
        return -1;
    }

    if(socket->isValid() == true){
        return 0;
    }else{
    qDebug() << nickNameStr;
    Message message = Message(MessageType::GetRoomDetails);
    socket->connectToHost(hostAddress,portNum);
    socket->waitForConnected();

             QByteArray block;
            QDataStream out(&block, QIODevice::ReadWrite);
            out.setVersion(QDataStream::Qt_5_5);
        //! [4] //! [6]
            out << (quint16)0;
            //out << s;
            //out << peer;
            out << message;
            out.device()->seek(0);

            out << (quint16)(block.size() - sizeof(quint16));
        //! [6] //! [7]
           qDebug()<<QString(block);
           socket->write(block);
           socket->flush();

           socket->waitForBytesWritten(3000);
           qDebug()<<"getRoomDetails : sent!";
   /* QString responseString="getRoomDetails: ";

    QByteArray tempByteArray = responseString.toUtf8();
    const char *tempChar = tempByteArray.data();
    socket->write(tempChar);

    socket->flush();
*/
  }
           //socket->waitForBytesWritten(3000);
return 0;
}

void MainWindow::connected(){

    qDebug() << "Connected !";

}

void MainWindow::disconnected(){

    qDebug() << "Disconnected !";
}

void MainWindow::readyRead(){

    qDebug() << "Reading ... " <<'\n';
    ui->roomList->clear();
    quint16 blockSize=0;
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_5);

    if (blockSize == 0) {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
            return;
//! [8]

//! [10]
        in >> blockSize;
    }

    if (socket->bytesAvailable() < blockSize)
        return;
//! [10] //! [11]

   // Sample sample;
   // in >> sample;
   // qDebug() << sample.message+"Message Recieved";
   // qDebug()<<sample.sender+"From Sender ";
   // Peer peer;
    //in >> peer;

    //qDebug() << peer.getNickName();
    //qDebug() << peer.getpeerAddress();
    Message message ;
    in >> message;

    MessageType mtype = message.getMessageType();
    if(mtype == MessageType::RoomDetails){

        qDebug() << "RoomDetails";
        std::vector<Room> roomVector = message.getRoomVector();
        for(Room room: roomVector){
                    MainWindow::rooms.insert(room.getRoomName(),room);
                    ui->roomList->addItem(room.getRoomName());
        }
    }
    else if (mtype == MessageType::GetRoomDetails){
        qDebug() <<"GetRoomDetails";
        // Drop this message
    }

    std::vector<QString> strings = message.getDataStrings();
    for(QString str : strings){
        qDebug() << str;
    }


// Old string logic
//    QString dataString =  QString::fromLatin1(socket->readAll());
//    qDebug() << dataString;
//    QStringList roomsList = dataString.split(":",QString::SkipEmptyParts);
//    for (QStringList::iterator it = roomsList.begin();
//             it != roomsList.end(); ++it) {

//      int distance =  it - roomsList.begin();
//      if(distance % 2 == 0){
//        ui->roomList->addItem(*it);
//        qDebug() << *it;
//        rooms.insert(*it,(*(it+1)).toInt());
//      }
//        }


}

void MainWindow::on_roomList_doubleClicked(const QModelIndex &index)
{
    qDebug() << index.row();
    QString roomName = ui->roomList->currentItem()->text();

//    Message message = Message(MessageType::ChekNickAvailability);
//    message.insertDataString(nickNameStr);
//    message.insertDataString(roomName);

//             QByteArray block;
//            QDataStream out(&block, QIODevice::ReadWrite);
//            out.setVersion(QDataStream::Qt_5_5);
//        //! [4] //! [6]
//            out << (quint16)0;
//            //out << s;
//            //out << peer;
//            out << message;
//            out.device()->seek(0);

//            out << (quint16)(block.size() - sizeof(quint16));
//        //! [6] //! [7]
//           qDebug()<<QString(block);
//           socket->write(block);
//           socket->flush();

//           socket->waitForBytesWritten(3000);

           Room room = rooms[roomName];
           chatRoom *chatRoomObj = new chatRoom(nickNameStr,room);

           connect(chatRoomObj, SIGNAL(leaveChatRoom(QString)), this,SLOT(leaveChatRoom(QString)));
           connect(chatRoomObj,SIGNAL(emitUnicast(Peer)), this, SLOT(connectToPeer(Peer)));
           chatRoomObj->setWindowTitle(roomName);

           chatRoomObj->show();

           Message message = Message(MessageType::JoinRoom);
           message.insertDataString(roomName);
           QHostAddress ownIPaddress;
               foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
                   if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
                       ownIPaddress = address;
               }
           Peer peer = Peer(nickNameStr,ownIPaddress);

           message.insertPeerObj(peer);
           QByteArray block;
          QDataStream out(&block, QIODevice::ReadWrite);
          out.setVersion(QDataStream::Qt_5_5);
       //! [4] //! [6]
          out << (quint16)0;
          //out << s;
          //out << peer;
          out << message;
          out.device()->seek(0);

          out << (quint16)(block.size() - sizeof(quint16));
       //! [6] //! [7]
         qDebug()<<QString(block);
         socket->write(block);
         socket->flush();

         socket->waitForBytesWritten(3000);
         qDebug()<<"joinRoom : sent!";
         //Need to sync local copy of Room obj
         // Need to send multicast message so that other UI will be updated
         // Multicast Will be sent from ChatRoom object it self

          /* QString responseString="joinRoom:"+nickNameStr+":"+ui->roomList->currentItem()->text();

           QByteArray tempByteArray = responseString.toUtf8();
           const char *tempChar = tempByteArray.data();
           socket->write(tempChar);

           socket->flush(); */

}


void MainWindow::leaveChatRoom(QString roomName){

    Message message = Message(MessageType::LeaveRoom);
    message.insertDataString(roomName);
    QHostAddress ownIPaddress;
        foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
                ownIPaddress = address;
        }
    Peer peer = Peer(nickNameStr,ownIPaddress);

    message.insertPeerObj(peer);
    QByteArray block;
   QDataStream out(&block, QIODevice::ReadWrite);
   out.setVersion(QDataStream::Qt_5_5);
//! [4] //! [6]
   out << (quint16)0;
   //out << s;
   //out << peer;
   out << message;
   out.device()->seek(0);

   out << (quint16)(block.size() - sizeof(quint16));
//! [6] //! [7]
  qDebug()<<QString(block);
  socket->write(block);
  socket->flush();

  socket->waitForBytesWritten(3000);
  qDebug()<<"LeaveRoom : sent!";

  //Need to sync local copy of Room obj
  // Need to send multicast message so that other UI will be updated
  // Multicast Will be sent from ChatRoom object it self


   /* QString responseString="leaveRoom:"+nickNameStr+":"+roomName;

    QByteArray tempByteArray = responseString.toUtf8();
    const char *tempChar = tempByteArray.data();
    socket->write(tempChar);

    socket->flush();*/


}

void MainWindow::newConnectionForUnicast(){
    while(tcpServerForUnicast->hasPendingConnections()){

        QTcpSocket *socket = tcpServerForUnicast->nextPendingConnection();
        UnicastChat *unicastChat = new UnicastChat(nickNameStr,socket);
        unicastChat->show();

    }
}

void MainWindow::connectToPeer(Peer peer){

    UnicastChat *unicastChat = new UnicastChat(nickNameStr,peer);
    unicastChat->show();

}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Chat Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Chat Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the Room Manager is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Chat Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(socket->errorString()));
    }


}


