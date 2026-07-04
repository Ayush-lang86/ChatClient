# 💬 ChatClient – Qt C++ Real-Time Chat Application

A real-time client-server chat application built using **C++ and Qt Framework**.  
This project demonstrates socket programming, GUI development, and basic networking concepts using TCP communication.

---

## 🚀 Features

- 🔌 Real-time messaging system
- 👥 Multi-client support via server
- 🧵 Concurrent client handling (threaded communication)
- 🖥️ Qt-based graphical user interface (GUI)
- 📡 TCP/IP socket communication
- 🔐 Basic username-based authentication
- 📣 Broadcast messaging between connected users

---

## 🛠️ Tech Stack

- C++
- Qt (Qt Widgets + Qt Network)
- TCP Sockets
- Multithreading (Qt / STL depending on implementation)

---
## 📁 Project Structure
ChatClient/
│
├── client/ # Qt Client application
├── server/ # Server application
├── ui/ # Qt Designer UI files
├── core/ # Networking + logic layer
└── README.md

## ⚙️ How to Run

### 1. Start the Server

Run the server first:

```bash
cd server
./server
