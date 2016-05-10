#include <SocketIoClient.h>

void webSocketEvent(SocketIoClient* client, WStype_t type, uint8_t * payload, size_t length) {
	String msg;
	switch(type) {
		case WStype_DISCONNECTED:
			USE_SERIAL.printf("[WSc] Disconnected!\n");
			break;
		case WStype_CONNECTED:
			USE_SERIAL.printf("[WSc] Connected to url: %s\n",  payload);
			break;
		case WStype_TEXT:
			msg = String((char*)payload);
			if(msg.startsWith("42")) {
				msg.remove(0,4);
				client->trigger(msg.substring(0, msg.indexOf("\"")).c_str(), NULL, NULL);
			}
			break;
		case WStype_BIN:
			USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
			hexdump(payload, length);
		break;
	}
}

void SocketIoClient::begin(const char* host, const int port, const char* url) {
#ifdef SOCKETIOCLIENT_USE_SSL
	_webSocket.beginSSL(host, port, url); 
#else
	_webSocket.begin(host, port, url);
#endif
	_webSocket.onEvent(std::bind(webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	_lastPing = millis();
}

void SocketIoClient::loop() {
	_webSocket.loop();
	if(millis() - _lastPing > PING_INTERVAL) {
		_webSocket.sendTXT("2");
		_lastPing = millis();
	}
}

void SocketIoClient::on(const char* event, std::function<void (uint8_t * payload, size_t length)> func) {
	_events[event] = func;
}

void SocketIoClient::trigger(const char* event, uint8_t* payload, size_t length) {
	auto e = _events.find(event);
	if(e != _events.end()) {
		e->second(payload, length);
	} else {
		USE_SERIAL.printf("[WSc] event %s not found\n", event);
	}
}