#include <SocketIoClient.h>

const String getEventName(const String msg) {
	int firstQuote = msg.indexOf("\"");
	int secondQuote = msg.indexOf("\"", firstQuote+1);
	if (secondQuote>firstQuote) {
		return msg.substring(firstQuote+1, secondQuote);
	} else {
		return "UNKNOWN";
	}
}

const String getEventPayload(const String msg) {
	String result = msg.substring(msg.indexOf(",")+1,msg.length()-1);
	if(result.startsWith("\"")) {
		result.remove(0,1);
	}
	if(result.endsWith("\"")) {
		result.remove(result.length()-1);
	}
	return result;
}

const String getId(const String msg) {
	index = msg.indexOf("[");
	if(msg.indexOf("[") == 2){
		return "";
	}
	return msg.substring(2, msg.indexOf("["));
}

void SocketIoClient::webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
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
				trigger(getEventName(msg).c_str(), getEventPayload(msg).c_str(), length, getId(msg).c_str());
			} else if(msg.startsWith("2")) {
				_webSocket.sendTXT("3");
			} else if(msg.startsWith("40")) {
				trigger("connect", NULL, 0);
			} else if(msg.startsWith("41")) {
				trigger("disconnect", NULL, 0);
			}
			break;
		case WStype_BIN:
			SOCKETIOCLIENT_DEBUG("[SIoC] get binary length: %u\n", length);
			hexdump(payload, length);
		break;
	}
}

void SocketIoClient::beginSSL(const char* host, const int port, const char* url) {
	_webSocket.beginSocketIOSSL(host, port, url);
	initialize();
}

void SocketIoClient::begin(const char* host, const int port, const char* url) {
	_webSocket.beginSocketIO(host, port, url);
	initialize();
}

void SocketIoClient::initialize() {
	_webSocket.onEvent(std::bind(&SocketIoClient::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	_lastPing = millis();
}

void SocketIoClient::loop() {
	_webSocket.loop();
	for(auto packet=_packets.begin(); packet != _packets.end();) {
		if(_webSocket.sendTXT(*packet)) {
			SOCKETIOCLIENT_DEBUG("[SIoC] packet \"%s\" emitted\n", packet->c_str());
			packet = _packets.erase(packet);
		} else {
			++packet;
		}
	}

	if(millis() - _lastPing > PING_INTERVAL) {
		_webSocket.sendTXT("2");
		_lastPing = millis();
	}
}

void SocketIoClient::on(const char* event, std::function<void (const char* payload, size_t length)> func) {
	_events[event] = func;
}

void SocketIoClient::emit(const char* event, const char* payload) {
	String msg = String("42[\"");
	msg += event;
	msg += "\"";
	if(payload) {
		msg += ",";
		msg += payload;
	}
	msg += "]";
	SOCKETIOCLIENT_DEBUG("[SIoC] add packet %s\n", msg.c_str());
	_packets.push_back(msg);
}

void SocketIoClient::trigger(const char* event, const char* payload, size_t length, const char* id) {
	auto e = _events.find(event);
	if(e != _events.end()) {
		SOCKETIOCLIENT_DEBUG("[SIoC] trigger event %s\n", event);
		e->second(payload, length, id);
	} else {
		SOCKETIOCLIENT_DEBUG("[SIoC] event %s not found. %d events available\n", event, _events.size());
	}
}

void SocketIoClient::callback(const char* id, const char* payload) {
	if(id == "") {
		return;
	}

	String msg = String("43") + id + "[";
	if(payload) {
		msg += payload;
	}
	msg += "]";
	SOCKETIOCLIENT_DEBUG("[SIoC] callback packet %s\n", msg.c_str());
	_packets.push_back(msg);
}

void SocketIoClient::disconnect() {
	_webSocket.disconnect();
	trigger("disconnect", NULL, 0);
}