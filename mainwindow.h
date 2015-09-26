#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "chatroom.h"
#include <QTcpServer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    int on_connectBtn_clicked();
    void connected();
    void disconnected();
    void readyRead();
    void newConnectionForUnicast();
    void displayError(QAbstractSocket::SocketError socketError);
    void leaveChatRoom(QString roomName);
    void on_roomList_doubleClicked(const QModelIndex &index);
    void connectToPeer(Peer peer);

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString nickNameStr;

    QTcpServer *tcpServerForUnicast;


    QHash<QString, Room> rooms;// This will be changed to QString -> Room
   // chatRoom chatRoomObj;
};

#endif // MAINWINDOW_H
