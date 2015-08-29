#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "chatroom.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_connectBtn_clicked();
    void connected();
    void disconnected();
    void readyRead();

    void leaveChatRoom(QString roomName);
    void on_roomList_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString nickNameStr;
   // chatRoom chatRoomObj;
};

#endif // MAINWINDOW_H
