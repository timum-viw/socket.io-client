#include <SocketIoClient.h>

const String getEventName(const String msg) {
	return msg.substring(4, msg.indexOf("\"",4));
}

const String getEventPayload(const String msg) {
	String result = msg.substring(msg.indexOf("\"",4)+2,msg.length()-1);
	if(result.startsWith("\"")) {
		result.remove(0,1);
	}
	if(result.endsWith("\"")) {
		result.remove(result.length()-1);
	}
	return result;
}

void SocketIoClient::webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
	String msg;
	switch(type) {
		case WStype_DISCONNECTED:
			SOCKETIOCLIENT_DEBUG("[SIoC] Disconnected!\n");
			break;
		case WStype_CONNECTED:
			SOCKETIOCLIENT_DEBUG("[SIoC] Connected to url: %s\n",  payload);
			break;
		case WStype_TEXT:
			msg = String((char*)payload);
			if(msg.startsWith("42")) {
				trigger(getEventName(msg).c_str(), getEventPayload(msg).c_str(), length);
			} else if(msg.startsWith("2")) {
				_webSocket.sendTXT("3");
			}
			break;
		case WStype_BIN:
			SOCKETIOCLIENT_DEBUG("[SIoC] get binary length: %u\n", length);
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
	_webSocket.onEvent(std::bind(&SocketIoClient::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	_lastPing = millis();
}

void SocketIoClient::loop() {
	_webSocket.loop();
	if(millis() - _lastPing > PING_INTERVAL) {
		_webSocket.sendTXT("2");
		_lastPing = millis();
	}
}

void SocketIoClient::on(const char* event, std::function<void (const char * payload, size_t length)> func) {
	_events[event] = func;
}

void SocketIoClient::emit(const char* event, const char * payload) {
	String msg = String("42[\"");
	msg += event;
	msg += "\",";
	msg += payload;
	msg += "]";
	_webSocket.sendTXT(msg);
}

void SocketIoClient::trigger(const char* event, const char * payload, size_t length) {
	auto e = _events.find(event);
	if(e != _events.end()) {
		e->second(payload, length);
	} else {
		SOCKETIOCLIENT_DEBUG("[SIoC] event %s not found. %d events available\n", event, _events.size());
	}
}