#ifndef CHATROOM_H
#define CHATROOM_H

#include <QWidget>
#include <QHostAddress>
#include  <QUdpSocket>
#include <room.h>
#include <peer.h>
#include <QListWidgetItem>
namespace Ui {
class chatRoom;
}

class chatRoom : public QWidget
{
    Q_OBJECT

public:
    chatRoom(QString mnickName,Room room,QWidget *parent = 0 );

    ~chatRoom();
    void closeEvent(QCloseEvent *event);
signals:
    /* This signal will be emitted when closeEvent slot on QWidget is executed. */
    void leaveChatRoom(QString chatRoomName);
    /* This signal will be emitted when on_peerList_itemDoubleClicked slot is executed. */
    void emitUnicast(Peer peer);

private slots:
    void processPendingDatagrams();


    void on_sendBtn_clicked();

    void on_peerList_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::chatRoom *ui;
    QUdpSocket *udpSocket;
    QHostAddress groupAddress;
    QString nickName;
    Room m_room;


};

#endif // CHATROOM_H
