#ifndef ROOM_H
#define ROOM_H

#include <QObject>
#include <QString>
#include <QHostAddress>
#include <QSet>
#include "peer.h"
class Room
{


public:
    Room();

    Room( QString roomName1, QString roomDesc1, int  portNum);

    QString getRoomName() const;
    void setRoomName(const QString &roomName1);

    QString getRoomDesc() const;
    void setRoomDesc(const QString &roomDesc1);

    int getPort() const;
    void setPort(int portNum);

    QSet<Peer> getJoinedNickNames() const;
    void setJoinedNickNames(const QSet<Peer> &joinedNickNames1);

    void addNickName(const Peer &nickName) ;
    void removeNickName(const Peer &nickName);


private:
    // To hold room name
    QString roomName;
    // To hold room description
    QString roomDesc;
    // This variable holds multicast group port
    int port;

    // To hold list of nick names connected to this room
    // Need to change this to QSet
    QSet<Peer> joinedNickNames;

};
QDataStream & operator <<( QDataStream & stream, const Room &room);
QDataStream & operator >>(QDataStream & stream, Room & room);
#endif // ROOM_H
