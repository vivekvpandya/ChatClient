#include "message.h"

Message::Message()
{

}

Message::Message(MessageType type){

    m_type = type;
}

void Message::setMessageType(const MessageType &mtype){
    m_type = mtype;
}

MessageType Message::getMessageType() const {
    return m_type;
}

std::vector<QString> Message::getDataStrings() const{
    return dataStrings;
}
std::vector<Peer> Message::getPeerVector() const{
    return peerVector;
}

void Message::insertDataString(const QString &string){

    dataStrings.push_back(string);
}

void Message::insertPeerObj(const Peer &peer){

    peerVector.push_back(peer);
}

std::vector<Room> Message::getRoomVector() const{
    return roomVector;
}
void Message::insertRoomObj(const Room &room){
    roomVector.push_back(room);
}

QDataStream & operator <<( QDataStream & stream, const MessageType &type){

    return stream <<(int) type;
}
QDataStream & operator >>(QDataStream & stream, MessageType & type){

    int enumVal;
    stream >> enumVal;
    type = MessageType(enumVal);
    return stream;
}

QDataStream & operator <<(QDataStream & stream, const Message &message){

    stream << message.getMessageType();

    std::vector<QString> dataStrings = message.getDataStrings();
    int dataStringsSize = dataStrings.size();
    stream << dataStringsSize;
    std::vector<Peer> peerObjects = message.getPeerVector();
    int peerVectorSize = peerObjects.size();
    stream << peerVectorSize;
    std::vector<Room> roomVector = message.getRoomVector();
    int roomVectorSize = roomVector.size();
    stream << roomVectorSize;
    for( QString obj : dataStrings ){
        
        stream << obj;
    }

    for(Peer peer :  peerObjects){
        stream<<peer;

    }
    for(Room room: roomVector){
        stream << room;
    }
    return stream;
}

QDataStream &  operator >>(QDataStream & stream, Message &message){
    MessageType mtype;
    QString stringObj;
    Peer peerObj;
    Room roomObj;
    int dataStringsSize;
    int peerVectorSize;
    int roomVectorSize;

    stream >> mtype;
    stream >> dataStringsSize;
    stream >> peerVectorSize;
    stream >> roomVectorSize;
    message.setMessageType(mtype);

    for(int i=0;i<dataStringsSize;i++){
        stream>>stringObj;
        message.insertDataString(stringObj);
    }
    for(int i=0;i<peerVectorSize; i++){
        stream>>peerObj;
        message.insertPeerObj(peerObj);
    }
    for(int i=0;i<roomVectorSize; i++){
        stream >> roomObj;
        message.insertRoomObj(roomObj);
    }
    return stream;
}

