#include "mainwindow.h"

#include <QApplication>
#include "client.h"
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client client;
    LoginDialog lg(client);
    if(!lg.isInitialised())
        return 0;
    lg.setModal(true);
    if(lg.exec() != QDialog::Accepted)
        return 0;
    MainWindow w(client);
    w.show();
    client.procesBufferedPacket();
    return a.exec();
}
