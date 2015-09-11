#include "peer.h"

Peer::Peer()
{

}
Peer::Peer(QString nickName, QHostAddress peerAddress){

    m_nickName = nickName;
    m_peerAddress = peerAddress;
}

QString Peer::getNickName() const{

    return m_nickName;
}

void Peer::setNickName(const QString &nickName){

    m_nickName = nickName;
}

QHostAddress Peer::getpeerAddress() const{

    return m_peerAddress;
}

void Peer::setPeerAddress(const QHostAddress &peerAddress){

    m_peerAddress = peerAddress;
}

QDataStream & operator << (QDataStream & stream, const Peer &peer){
    stream<<peer.getNickName()<<peer.getpeerAddress();

}
QDataStream & operator >>(QDataStream & stream, Peer &peer){
    QString nickName;
    QHostAddress peerAddress;

    stream>>nickName>>peerAddress;
    peer.setNickName(nickName);
    peer.setPeerAddress(peerAddress);
    return stream;
}
