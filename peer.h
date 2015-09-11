#ifndef PEER_H
#define PEER_H

#include <QDataStream>
#include <QHostAddress>
class Peer
{
public:
    Peer();
    Peer(QString nickName, QHostAddress peerAddress);
    QString getNickName() const;
    void setNickName(const QString &nickName);
    QHostAddress getpeerAddress() const;
    void setPeerAddress(const QHostAddress &peerAddress);


private:
    QString m_nickName;
    QHostAddress m_peerAddress;

};
QDataStream & operator <<( QDataStream & stream, const Peer &peer);
QDataStream & operator >>(QDataStream & stream, Peer & peer);

inline bool operator==(const Peer &e1, const Peer &e2)
{
    return e1.getNickName() == e2.getNickName() ; // Every name of peer is unique

}

inline uint qHash(const Peer &key, uint seed)
{
    return qHash(key.getNickName(), seed) ^ key.getpeerAddress().LocalHost;
}


#endif // PEER_H
