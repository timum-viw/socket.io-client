#ifndef __SOCKET_IO_CLIENT_H__
#define __SOCKET_IO_CLIENT_H__

#include <Arduino.h>
#include <map>
#include <WebSocketsClient.h>

#define USE_SERIAL Serial

#define PING_INTERVAL 10000 //TODO: use socket.io server response
#ifdef SOCKETIOCLIENT_USE_SSL
	#define DEFAULT_PORT 443
#else
	#define DEFAULT_PORT 80
#endif
#define DEFAULT_URL "/socket.io-client/?transport=websocket"

class SocketIoClient {
private:
	WebSocketsClient _webSocket;
	int _lastPing;
	std::map<String, std::function<void (const char * payload, size_t length)>> _events;

	void trigger(const char* event, const char * payload, size_t length);
	void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
public:
	void begin(const char* host, const int port = DEFAULT_PORT, const char* url = DEFAULT_URL);
	void loop();
	void on(const char* event, std::function<void (const char * payload, size_t length)>);
	void emit(const char* event, const char * payload);
};

#endif