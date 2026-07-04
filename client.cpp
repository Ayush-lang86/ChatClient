#include "client.h"
#include<unistd.h>
#include<arpa/inet.h>
#include<iostream>
#include "json.hpp"
#include <string>
#include <cstring>
#include<thread>
#include<vector>
using json = nlohmann::json;

Client::Client(QObject * parent): QObject(parent) {
    sock = -1;
    buffer = {};
    username = "";
    userList = {};
}
bool Client::connectToServer(){

    sock = socket(AF_INET,SOCK_STREAM,0);
    if (sock < 0)
    {
        perror("socket");
        return false;
    }
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET,"192.168.0.108",&server_addr.sin_addr);

    if(::connect(sock,(sockaddr*)&server_addr,sizeof(server_addr))<0){
        perror("error: ");
        return false;
    }
    return true;
}
bool Client::waitForAuthRequest(){
    char temp[1024];
    while(true){
        int bytes = recv(sock, temp, sizeof(temp), 0);
        if (bytes <= 0){
            return false;
        }
        buffer.append(temp,bytes);
        size_t pos;
        while((pos = buffer.find('\n')) != std::string::npos){
            std::string packet = buffer.substr(0,pos);
            buffer.erase(0,pos+1);
            json j;
            j = json::parse(packet);//i am assuming that always correct json packet received from server ellse do try catch
            if(j.value("type","") != "auth")
                continue;
            if(j.value("status","") == "required")
                return true;
        }
    }
}
AuthResult Client::authenticate(){
    json packet;
    packet["type"] = "auth";
    packet["username"] = username;
    std::string stream = packet.dump() + "\n";
    send(sock,stream.c_str(),stream.size(),0);//i think i will need to handle the userlist + users packet also
    char temp[1024];
    while(true){
        int bytes = recv(sock, temp, sizeof(temp), 0);
        if (bytes <= 0){
            return AuthResult::Disconnected;
        }
        buffer.append(temp,bytes);
        size_t pos;
        while((pos = buffer.find('\n')) != std::string::npos){
            std::string p = buffer.substr(0,pos);
            buffer.erase(0,pos+1);
            json j;
            j = json::parse(p);//i am assuming that always correct json packet received from server ellse do try catch
            if(j.value("type","") != "auth")
                continue;
            std::string k = j.value("status","");
            if(k == "invalid")
                return AuthResult::Invalid;
            else if(k == "taken")
                return AuthResult::Taken;
            else if(k == "accepted")
                return AuthResult::Accepted;
            else
                return AuthResult::Disconnected;
        }
    }
}
void Client::startReceiving(){
    std::thread(&Client::receiveLoop, this).detach();
}
void Client::receiveLoop()
{
    char temp[2048]{};
    while(true){
        memset(temp,0,sizeof(temp));
        int bytes = recv(sock,temp,sizeof(temp),0);
        if(bytes > 0){
            buffer.append(temp,bytes);
            size_t pos;
            while((pos = buffer.find('\n')) != std::string::npos){
                std::string packet = buffer.substr(0,pos);
                buffer.erase(0,pos+1);
                json j;
                try {
                    j = json::parse(packet);
                } catch (...) {
                    std::cout << "Bad packet: " << packet << "\n";
                    continue;
                }
                std::string type = j.value("type","");
                if(type == "message"){
                    std::string sender = j.value("from", "");
                    std::string time   = j.value("time", "");
                    std::string msg    = j.value("msg", "");

                    emit messageReceived(QString::fromStdString(sender), QString::fromStdString(time), QString::fromStdString(msg));
                }else if(type == "event"){
                    std::string event = j.value("event","");
                    std::string user = j.value("user","");
                    if (event == "joined") {
                        userList.push_back(QString::fromStdString(user));
                    }else if (event == "left") {
                        userList.removeAll(QString::fromStdString(user));
                    }

                    emit userlistUpdated(userList);
                }else if(type == "userlist"){
                    userList.clear();
                    for (const auto &u : j["users"])
                        userList.append(QString::fromStdString(u));
                    emit userlistUpdated(userList);
                }
            }           
        }else{
            if(bytes < 0){
                perror("ERROR:");
            }
            emit serverDisconnected();
            break;
        }
    }
}
bool Client::sendMessage(const QString & message,const QStringList & receivers){
    json packet;
    packet["type"] = "message";
    packet["msg"] = message.toStdString();
    for (const auto &user : receivers)
    {
        packet["to"].push_back(user.toStdString());
    }
    std::string stream = packet.dump()+"\n";
    return send(sock,stream.c_str(),stream.size(),0);
}
void Client::procesBufferedPacket(){
    char temp[1024];
    memset(temp,0,sizeof(temp));;
    size_t pos;
    while((pos = buffer.find('\n')) != std::string::npos){
        std::string packet = buffer.substr(0,pos);
        buffer.erase(0,pos+1);
        json j;
        try {
            j = json::parse(packet);
        } catch (...) {
            std::cout << "Bad packet: " << packet << "\n";
            continue;
        }
        if(j.value("type","") == "userlist"){
            userList.clear();
            for(const auto &u : j["users"])
                userList.append(QString::fromStdString(u));
            emit userlistUpdated(userList);
        }

    }
}
