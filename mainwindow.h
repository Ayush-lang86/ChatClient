#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QMessageBox>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Client &c,QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_sendButton_clicked();
    void onMessageReceived(const QString &sender, const QString &time, const QString &message);
    void onUserlistUpdated(const QStringList &users);
    void onServerDisconnected();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Client & client;
    bool isValidMessage(const QString& msg);
};
#endif // MAINWINDOW_H
