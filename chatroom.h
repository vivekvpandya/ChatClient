#ifndef CHATROOM_H
#define CHATROOM_H

#include <QWidget>

namespace Ui {
class chatRoom;
}

class chatRoom : public QWidget
{
    Q_OBJECT

public:
    explicit chatRoom(QWidget *parent = 0);
    ~chatRoom();
    void closeEvent(QCloseEvent *event);
signals:
    void leaveChatRoom(QString chatRoomName);

private:
    Ui::chatRoom *ui;
};

#endif // CHATROOM_H
