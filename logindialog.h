#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "client.h"
#include <QMessageBox>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(Client &c,QWidget *parent = nullptr);
    bool isInitialised();
    ~LoginDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::LoginDialog *ui;
    Client &client;
    bool initSuccess = true;
};

#endif // LOGINDIALOG_H
