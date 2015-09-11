#ifndef UNICASTCHAT_H
#define UNICASTCHAT_H

#include <QWidget>
#include <QTcpSocket>
#include "peer.h"

namespace Ui {
class UnicastChat;
}

class UnicastChat : public QWidget
{
    Q_OBJECT

public:
    explicit UnicastChat(QString nickName,Peer peer,QWidget *parent = 0);
    UnicastChat(QString nickName,QTcpSocket *socketPtr,QWidget *parent = 0);

    ~UnicastChat();

private slots:

    void connected();
    void disconnected();
    void readyRead();





    void on_sendBtn_pressed();

private:
    Ui::UnicastChat *ui;
    QTcpSocket *socket;
    QString m_nickName;
};

#endif // UNICASTCHAT_H
