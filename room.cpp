#include "room.h"

Room::Room()
{

}
//Room constructor to create a room
Room::Room( QString roomName1,  QString roomDesc1, int portNum){

    roomName = roomName1;
    port = portNum;
    roomDesc = roomDesc1;

}


QString Room::getRoomName() const
{
    return roomName;
}

void Room::setRoomName(const QString &roomName1 ){

    roomName = roomName1;
}

int Room::getPort() const
{
    return port;
}

void Room::setPort(int portNum)
{
    port = portNum;
}

QString Room::getRoomDesc() const
{
    return roomDesc;
}

void Room::setRoomDesc(const QString &roomDesc1)
{
    roomDesc = roomDesc1;
}

QSet<Peer> Room::getJoinedNickNames() const
{
    return joinedNickNames;
}

void Room::setJoinedNickNames(const QSet<Peer> &joinedNickNames1)
{
    joinedNickNames = joinedNickNames1;
}

void  Room::addNickName(const Peer &nickName) {

    joinedNickNames.insert(nickName);

}

void Room::removeNickName(const Peer &nickName){

    joinedNickNames.remove(nickName);
}


QDataStream & operator << (QDataStream & stream, const Room &room){
    stream << room.getRoomName();
    stream << room.getRoomDesc();
    stream << room.getPort();
    QSet<Peer> joinedNicks = room.getJoinedNickNames();
    int joinedNicksSize = joinedNicks.size();
    stream << joinedNicksSize;
    for(Peer peer: joinedNicks){
        stream << peer;
    }

}
QDataStream & operator >>(QDataStream & stream, Room &room){
    QString roomName;
    QString roomDesc;
    int roomPort;
    int joinedNickSize;
    stream >> roomName;
    room.setRoomName(roomName);
    stream >> roomDesc;
    room.setRoomDesc(roomDesc);
    stream >> roomPort;
    room.setPort(roomPort);

    stream >> joinedNickSize;
    Peer peerObj;
    for(int i=0;i<joinedNickSize;i++){
        stream >> peerObj;
        room.addNickName(peerObj);
    }
}
