<h1 align="center">socket.io-client</h1>

<p align = "center">A socket.io-client implementation for ESP8266 and Arduino.</p>

## Index
- [Install](#install)
- [Functions](#functions)
- [Miscellaneous](#miscellaneous)
- [Example](#example)
- [Contribution](#contribution)

## Install <a name = "install"></a>

### 💡 This library uses lstdc++
To compile you have to add a reference to the linker. 
To do so edit platform.txt in $ARDUINO_IDE/hardware/esp8266com/esp8266 and add '-lstdc++' to the line 
```
compiler.c.elf.libs= ...
```
💡 **Note:** See [ESP8266 for Arduino IDE (xtensa-lx106-elf-gcc) and std::map linking error](http://stackoverflow.com/questions/33450946/esp8266-for-arduino-ide-xtensa-lx106-elf-gcc-and-stdmap-linking-error).

### Add library
Best thing is to use the Arduino Library Manager.
1. Go to Sketch > Include Library > Manage Libraries.
2. Install WebSockets by Markus Sattler
3. Install SocketIoClient
4. Select Sketch > Include Library > SocketIoClient

## Functions <a name = "functions"></a>
1. [SocketIoClient::begin()](#begin)
2. [SocketIoClient::beginSSL()](#beginSSL)
3. [SocketIoClient::disconnect()](#disconnect)
4. [SocketIoClient::emit()](#emit)
5. [SocketIoClient::loop()](#loop)
6. [SocketIoClient::on()](#on)
7. [SocketIoClient::remove()](#remove)
8. [SocketIoClient::setAuthorization()](#setAuthorization)

### SocketIoClient::begin(host[, port, path]) <a name = "begin"></a>
Open connection to socket.io server.

##### Parameter
```host``` url to socket.io server
```port``` port to connect on. Defaults to 80 or 443 (SSL)
```path``` path to connect to on server. Defaults to "/socket.io/?transport=websocket"

##### Example
```c
socket.begin("my.socket-io.server", 443, "/socket.io/?transport=websocket");
```

### SocketIoClient::beginSSL(host[, port, path, fingerprint]) <a name = "beginSSL"></a>
Open SSL connection to socket.io server.

##### Parameter
```host``` url to socket.io server
```port``` port to connect on. Defaults to 80 or 443 (SSL)
```path``` path to connect to on server. Defaults to "/socket.io/?transport=websocket"
```fingerprint``` the SSL fingerprint. Defaults to ""

##### Example
```c
socket.beginSSL("my.socket-io.server", 443, "/socket.io/?transport=websocket", "26 96 1C 2A 51 07 FD 15 80 96 93 AE F7 32 CE B9 0D 01 55 C4");
```

### SocketIoClient::disconnect() <a name = "disconnect"></a>
Disconnect from the server.

##### Example
```c
socket.disconnect();
```

### SocketIoClient::emit(event, payload) <a name = "emit"></a>
Emits an event to the server.

##### Parameter
```event``` name of the event to be emitted
```payload``` string of the payload to be sent with the event. Plain strings and object property names should be encapsulated in quotes.

##### Example
```c
socket.emit("plainString", "\"this is a plain string\"");
socket.emit("jsonObject", "{\"foo\":\"bar\"}");
```

### SocketIoClient::loop() <a name = "loop"></a>
Processes the websocket. Should be called in Arduino main loop.

### SocketIoClient::on(event, callback) <a name = "on"></a>
Binds a function to an event.

##### Parameter
```event``` name of the event to bind to
```callback``` callback function to call when the event is triggered
Function signature must be
```c
void (const char * payload, size_t length)
```

##### Example
```c
void event(const char * payload, size_t length) {
	// do stuff
}
socket.on("event", event);
```

##### Supported default events:
* `connect` - when user is connected to server
* `disconnect` - when user is disconnected from the server

### SocketIoClient::remove(event) <a name = "remove"></a>
Removes the previously added event.

##### Parameter
```event``` name of the event binded

##### Example
```c
socket.remove("event");
```

### SocketIoClient::setAuthorization(username, password) <a name = "setAuthorization"></a>
Set HTTP Basic auth username and password.

##### Example
```c
socket.setAuthorization("username", "password");
```

## Miscellaneous <a name = "miscellaneous"></a>
To go along with the socket.io-client implementation of socket.io the ```connect``` event is triggered upon successfully opened connection to server. To utilize simply add
```
socket.on("connect", handler)
```
Likewise, ```disconnect``` event is triggered upon terminated connection.

##  Example <a name = "example"></a>
See [Example](examples/BasicExample/BasicExample.ino)

## Contribution <a name = "contribution"></a>
Based on the great work of [Links2004 / arduinoWebSockets](https://github.com/Links2004/arduinoWebSockets).
