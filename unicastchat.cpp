#include "unicastchat.h"
#include "ui_unicastchat.h"
#include "message.h"
UnicastChat::UnicastChat(QString nickName,Peer peer,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnicastChat)
{   m_nickName = nickName;
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    socket->connectToHost(peer.getpeerAddress(),17000);
    socket->waitForConnected();


}
UnicastChat::UnicastChat(QString nickName,QTcpSocket *socketPtr,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnicastChat)
{   socket = socketPtr;


    m_nickName = nickName;
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    ui->setupUi(this);
}

UnicastChat::~UnicastChat()
{
    delete ui;
}

void UnicastChat::connected(){

    qDebug()<<"Connected!";

}

void UnicastChat::disconnected(){

    qDebug() << "Disconnected !";
}

void UnicastChat::readyRead(){

    qDebug() << "Reading ... " <<'\n';

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
    if(mtype == MessageType::Message){

        qDebug() << "Message";
        std::vector<QString> messageStrings = message.getDataStrings();
       QString sender = messageStrings.at(1);
       QString message = messageStrings.at(0);
       ui->chatBox->append(sender + " : "+message);

    }




}




void UnicastChat::on_sendBtn_pressed()
{
    QString messageStr = ui->messgeBox->toPlainText();
    Message message = Message(MessageType::Message);
    message.insertDataString(messageStr);
    message.insertDataString(m_nickName);

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
  qDebug()<<"messageSent : sent!";

}
