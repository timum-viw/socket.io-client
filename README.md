# socket.io-client

## Install

### This library uses lstdc++!!
To compile you have to add a reference to the linker. 
To do so edit platform.txt in $ARDUINO_IDE/hardware/esp8266com/esp8266 and add '-lstdc++' to the line 
```
compiler.c.elf.libs= ...
```
(see http://stackoverflow.com/questions/33450946/esp8266-for-arduino-ide-xtensa-lx106-elf-gcc-and-stdmap-linking-error)

### Add library
Best thing is to use the Arduino Library Manager.
* Go to Sketch > Include Library > Manage Libraries.
* Install WebSockets by Markus Sattler
* Install SocketIoClient
* Select Sketch > Include Library > SocketIoClient

## Functions

### SocketIoClient::begin(host[, port, path])
open connection to socket.io server.
##### Parameter
```host``` url to socket.io server
```port``` port to connect on. Defaults to 80 or 443 (SSL)
```path``` path to connect to on server. Defaults to "/socket.io/?transport=websocket"
##### Example
```c
socket.begin("my.socket-io.server", 443, "/socket.io/?transport=websocket");
```

### SocketIoClient::beginSSL(host[, port, path, fingerprint])
open SSL connection to socket.io server.
##### Parameter
```host``` url to socket.io server
```port``` port to connect on. Defaults to 80 or 443 (SSL)
```path``` path to connect to on server. Defaults to "/socket.io/?transport=websocket"
```fingerprint``` the SSL fingerprint. Defaults to ""
##### Example
```c
socket.beginSSL("my.socket-io.server", 443, "/socket.io/?transport=websocket", "26 96 1C 2A 51 07 FD 15 80 96 93 AE F7 32 CE B9 0D 01 55 C4");
```

### SocketIoClient::disconnect()
disconnect from the server
##### Example
```c
socket.disconnect();
```

### SocketIoClient::on(event, callback)
binds a function to an event. 
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
	//do stuff
}
socket.on("event", event);
```
##### Supported default events:
* `connect` - when user is connected to server
* `disconnect` - when user is disconnected from the server

### SocketIoClient::emit(event, payload)
emits an event to the server.
##### Parameter
```event``` name of the event to be emitted
```payload``` string of the payload to be sent with the event. Plain strings and object property names should be encapsulated in quotes.
##### Example
```c
socket.emit("plainString", "\"this is a plain string\"");
socket.emit("jsonObject", "{\"foo\":\"bar\"}");
```

### SocketIoClient::loop()
processes the websocket. Should be called in Arduino main loop.

### SocketIoClient::setAuthorization(username, password)
set HTTP Basic auth username and password.
##### Example
```c
socket.setAuthorization("username", "password");
```

## Misc
To go along with the socket.io-client implementation of socket.io the ```connect``` event is triggered upon successfully opened connection to server. To utilize simply add
```
socket.on("connect", handler)
```
likewise ```disconnect``` event is triggered upon terminated connection.

##  Example
see [Example](examples/BasicExample/BasicExample.ino)

## contribution
Based on the great work of [Links2004 / arduinoWebSockets](https://github.com/Links2004/arduinoWebSockets).
