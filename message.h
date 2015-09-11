#ifndef MESSAGE_H
#define MESSAGE_H
#include <vector>
#include <QDataStream>
#include <peer.h>
#include "room.h"


enum class MessageType{
    GetRoomDetails,
    RoomDetails,
    JoinRoom,
    LeaveRoom,
    Message,
};

QDataStream & operator <<( QDataStream & stream, const MessageType &type);
QDataStream & operator >>(QDataStream & stream, MessageType & types);


class Message
{
public:
    Message();

    MessageType getMessageType() const;
    void setMessageType(const MessageType &mtype);
    Message(MessageType type);
    std::vector<QString> getDataStrings() const;
    void insertDataString(const QString &string);
    std::vector<Peer> getPeerVector() const;
    void insertPeerObj(const Peer &peer);
    std::vector<Room> getRoomVector() const;
    void insertRoomObj(const Room &room);


private:
    MessageType m_type;
    std::vector<QString> dataStrings;
    std::vector<Peer> peerVector;
    std::vector<Room> roomVector;

};

QDataStream & operator <<( QDataStream & stream, const Message &message);
QDataStream & operator >>(QDataStream & stream, Message & message);
#endif // MESSAGE_H
