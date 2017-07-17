#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "tcpserver.h"
#include <QDebug>
#include <QDesktopServices>
#include <QDir>

TcpServer gTcpServer;

ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);

//    std::vector<std::string> hosts = TcpServer::getLocalHosts();
//    for (int i = 0; i < hosts.size(); i ++) {
//        qDebug() << QString::fromStdString(hosts.at(i));
//    }
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::on_pushButton_start_clicked()
{
    gTcpServer.start();
}

void ServerWindow::on_pushButton_stop_clicked()
{
    gTcpServer.stop();
}

void ServerWindow::on_pushButton_openWorkDir_clicked()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath()));
}
