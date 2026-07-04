#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Client &c,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client(c)
{
    ui->setupUi(this);
    ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    connect(&client,&Client::messageReceived,this, &MainWindow::onMessageReceived);
    connect(&client,&Client::userlistUpdated,this,&MainWindow::onUserlistUpdated);
    connect(&client,&Client::serverDisconnected,this,&MainWindow::onServerDisconnected);
    ui->header->setText("💬 LAN Chat");
    ui->lineEdit->setPlaceholderText("Type a message...");
    setWindowTitle("LAN Chat");
    statusBar()->showMessage("🟢 Connected");
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_sendButton_clicked(){
    //qDebug() << "Send button clicked";
    QString text = ui->lineEdit->text();
    //send message then to networking code
    if (!isValidMessage(text))
    {
        QMessageBox::warning(this,"Warning","Invalid messsage.");
        return;
    }
    QStringList receivers;
    for (QListWidgetItem *item : ui->listWidget->selectedItems())
    {
        receivers.append(item->text());
    }
    if(receivers.isEmpty())
        QMessageBox::warning(this,"Warning","Select atleast one receiver");
    if (!client.sendMessage(text, receivers))
    {
        QMessageBox::critical(this,"Error","Failed to send message.");
        return;
    }
    ui->lineEdit->clear();
}

bool MainWindow::isValidMessage(const QString &msg) {
    QString m = msg.trimmed();

    if (m.isEmpty()) return false;
    if (m.length() > 1000) return false;
    if (m.contains('|') || m.contains('\\')) return false;

    return true;
}
void MainWindow::onMessageReceived(const QString &sender, const QString &time, const QString &message)
{
    bool mine = (sender.toStdString() == client.username)?true : false;
    QString senderColor = mine?"#FFD700" : "#66CCFF";
    QString html = QString(
                       "<div style='margin-bottom:10px;'>"
                       "<span style='color:gray; font-size:9pt;'>[%1]</span> "
                       "<span style='color:%2; font-weight:bold;'>%3</span><br>"
                       "<span style='color:white;'>%4</span>"
                       "</div>")
                       .arg(time.toHtmlEscaped())
                       .arg(senderColor)
                       .arg(sender.toHtmlEscaped())
                       .arg(message.toHtmlEscaped());

    ui->textBrowser->append(html);
    //ui->textBrowser->append("[" + time + "] " + sender + ": " + message);
}
void MainWindow::onServerDisconnected(){
    QMessageBox::critical(this,"Error","Server Disconnected");
    close();
    return;
}
void MainWindow::onUserlistUpdated(const QStringList& users){
    ui->listWidget->clear();

    for (const auto &user : users)
    {
        ui->listWidget->addItem(user);
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->listWidget->selectAll();
}

