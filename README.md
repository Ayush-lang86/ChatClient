# ChatClient

A lightweight LAN chat client built with **Qt6 (Widgets)** and raw TCP sockets. It connects to a chat server over a socket, authenticates with a username, and lets you send messages to selected users on the network, all through a simple desktop UI.

## Features

- 🔐 Username-based authentication with server-side validation (invalid / taken / accepted)
- 💬 Real-time messaging over TCP, using newline-delimited JSON packets
- 👥 Live user list with join/leave events
- 🎨 Simple, styled chat window (custom message bubbles, timestamps, sender highlighting)
- ✅ Basic client-side message validation (length limits, disallowed characters)

## Tech Stack

- **C++17**
- **Qt6** (`Core`, `Widgets`)
- **[nlohmann/json](https://github.com/nlohmann/json)** (bundled as `json.hpp`) for packet serialization
- POSIX sockets (`sys/socket.h`, `arpa/inet.h`) for networking
- **CMake** (>= 3.19) build system

## Project Structure

```
ChatClient/
├── CMakeLists.txt        # Build configuration
├── main.cpp               # Application entry point
├── client.h / client.cpp  # Networking layer (socket, auth, send/receive loop)
├── mainwindow.h/.cpp/.ui  # Main chat window
├── logindialog.h/.cpp/.ui # Username/login dialog
├── json.hpp                # Bundled nlohmann::json single-header library
└── resources.qrc           # Qt resources
```

## Requirements

- CMake 3.19+
- Qt 6.5+ (Core, Widgets)
- A C++17-capable compiler (g++/clang)
- A Linux/POSIX environment (the networking code uses POSIX sockets, e.g. `arpa/inet.h`, `unistd.h`)

## Building

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

## Running

The client expects a compatible chat server listening on `127.0.0.1:8080`. Start the server first, then run:

```bash
./ChatClient
```

On launch you'll be prompted for a username. Once authenticated, select one or more users from the list and start chatting.

## Server Protocol (expected)

The client communicates with the server using newline-delimited JSON messages. Example packet types:

| Type       | Direction       | Purpose                                  |
|------------|-----------------|-------------------------------------------|
| `auth`     | client ⇄ server | Username submission / auth status         |
| `message`  | client ⇄ server | Chat message with sender/recipients        |
| `event`    | server → client | User joined/left notifications             |
| `userlist` | server → client | Full list of currently connected users      |

## Known Limitations

- Server address/port are currently hardcoded (`127.0.0.1:8080`) in `client.cpp`.
- No message encryption/TLS — intended for trusted LAN use only.
- No persistent message history.

## License

*(Add a license here, e.g. MIT, before publishing.)*
