#include "chatroom.h"
#include "ui_chatroom.h"
#include <QDebug>

chatRoom::chatRoom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatRoom)
{
    ui->setupUi(this);
}

chatRoom::~chatRoom()
{
    delete ui;
}

void chatRoom::closeEvent(QCloseEvent *event){
    qDebug() << windowTitle();
    emit leaveChatRoom(windowTitle());


}
