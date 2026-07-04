#ifndef CLIENT_H
#define CLIENT_H

#include<string>
#include<vector>
#include <QObject>

enum class AuthResult
{
    Accepted,
    Taken,
    Invalid,
    Disconnected
};

class Client: public QObject
{
    Q_OBJECT
public:
    explicit  Client(QObject *parent = nullptr);
    std::string username;
    QStringList userList;
    bool connectToServer();
    bool waitForAuthRequest();
    AuthResult authenticate();
    void startReceiving();
    bool sendMessage(const QString& message, const QStringList & receivers);
    void procesBufferedPacket();
private:
    int sock;
    std::string buffer;
    void receiveLoop();
signals:
    void serverDisconnected();
    void messageReceived(const QString &sender, const QString &time, const QString &message);
    void userlistUpdated(const QStringList &users);
};

#endif // CLIENT_H
