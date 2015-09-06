#ifndef CHATROOM_H
#define CHATROOM_H

#include <QWidget>
#include <QHostAddress>
#include  <QUdpSocket>
namespace Ui {
class chatRoom;
}

class chatRoom : public QWidget
{
    Q_OBJECT

public:
    chatRoom(QString mnickName,int mportNumber,QWidget *parent = 0 );

    ~chatRoom();
    void closeEvent(QCloseEvent *event);
signals:
    void leaveChatRoom(QString chatRoomName);

private slots:
    void processPendingDatagrams();


    void on_sendBtn_clicked();

private:
    Ui::chatRoom *ui;
    QUdpSocket *udpSocket;
    QHostAddress groupAddress;
    QString nickName;
    int portNumber;


};

#endif // CHATROOM_H
