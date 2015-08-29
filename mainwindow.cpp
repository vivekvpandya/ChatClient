#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    //connect(socket,SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectBtn_clicked()
{
    QString hostAddress = ui->hostAddress->text();
    QString portNumber = ui->portNum->text();
    qint64 portNum = portNumber.toLongLong();
    nickNameStr = ui->nickName->text();
    qDebug() << nickNameStr;

    socket->connectToHost(hostAddress,portNum);
    socket->waitForConnected();
    QString responseString="getRoomDetails: ";

    QByteArray tempByteArray = responseString.toUtf8();
    const char *tempChar = tempByteArray.data();
    socket->write(tempChar);

    socket->flush();

    //socket->waitForBytesWritten(3000);
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
    QString dataString =  QString::fromLatin1(socket->readAll());
    qDebug() << dataString;
    QStringList roomsList = dataString.split(":",QString::SkipEmptyParts);
    for (QStringList::iterator it = roomsList.begin();
             it != roomsList.end(); ++it) {
        qDebug() << *it;
      int distance =  it - roomsList.begin();
      if(distance % 2 == 0)
        ui->roomList->addItem(*it);

        }


}

void MainWindow::on_roomList_doubleClicked(const QModelIndex &index)
{
    qDebug() << index.row();
    chatRoom *chatRoomObj = new chatRoom();
    connect(chatRoomObj, SIGNAL(leaveChatRoom(QString)), this,SLOT(leaveChatRoom(QString)));
    chatRoomObj->setWindowTitle(ui->roomList->currentItem()->text());
    chatRoomObj->show();
    QString responseString="joinRoom:"+nickNameStr+":"+ui->roomList->currentItem()->text();

    QByteArray tempByteArray = responseString.toUtf8();
    const char *tempChar = tempByteArray.data();
    socket->write(tempChar);

    socket->flush();

}

void MainWindow::leaveChatRoom(QString roomName){

    QString responseString="leaveRoom:"+nickNameStr+":"+roomName;

    QByteArray tempByteArray = responseString.toUtf8();
    const char *tempChar = tempByteArray.data();
    socket->write(tempChar);

    socket->flush();

}
