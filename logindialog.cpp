#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(Client & c,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
    , client(c)
{
    ui->setupUi(this);
    if(!client.connectToServer()){
        QMessageBox::critical(this, "Error", "Could not connect to the server.");
        initSuccess = false;
        return;
    }
    if (!client.waitForAuthRequest())
    {
        QMessageBox::critical(this, "Error", "Server did not send an authentication request.");
        initSuccess = false;
        return;
    }
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pushButton_clicked()
{
    QString name = ui->lineEdit->text().trimmed();
    client.username = name.toStdString();
    AuthResult result = client.authenticate();
    switch(result){
    case AuthResult::Accepted:
        client.startReceiving();
        accept();
        break;

    case AuthResult::Taken:
        ui->label->setText("Username already taken");
        break;

    case AuthResult::Invalid:
        ui->label->setText("Invalid username");
        break;

    case AuthResult::Disconnected:
        //ui->label->setText("Disconnected from server");
        QMessageBox::critical(this,"Error","Connection to server lost");
        reject();
        break;
    }
}
bool LoginDialog::isInitialised(){
    return initSuccess;
}

