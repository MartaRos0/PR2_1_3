/*
Archivo: WsClientWrapper.h
Descripcion: Wrapper que adapta WebSocketsClient a la interfaz Client de Arduino,
permitiendo su uso con PubSubClient sobre WebSocket seguro (WSS).
*/

#pragma once
#include <WebSocketsClient.h>
#include <Client.h>

class WsClientWrapper : public Client
{
public:
  WsClientWrapper(WebSocketsClient &ws) : _ws(ws) {}

  size_t write(const uint8_t *buf, size_t size) override
  {
    _ws.sendBIN(buf, size);
    return size;
  }

  size_t write(uint8_t byte) override
  {
    _ws.sendBIN(&byte, 1);
    return 1;
  }

  int available() override { return _buffer.length(); }

  int read() override
  {
    if (_buffer.length() == 0) return -1;
    char c = _buffer[0];
    _buffer.remove(0, 1);
    return c;
  }

  int read(uint8_t *buf, size_t size) override
  {
    int i = 0;
    while (i < (int)size && _buffer.length() > 0)
    {
      buf[i++] = (uint8_t)_buffer[0];
      _buffer.remove(0, 1);
    }
    return i;
  }

  int peek() override
  {
    if (_buffer.length() == 0) return -1;
    return _buffer[0];
  }

  void flush() override {}
  int connect(IPAddress, uint16_t) override   { return 1; }
  int connect(const char *, uint16_t) override { return 1; }
  void stop() override {}
  uint8_t connected() override { return 1; }
  operator bool() override { return true; }

  void injectData(uint8_t *payload, size_t length)
  {
    for (size_t i = 0; i < length; i++) _buffer += (char)payload[i];
  }

private:
  WebSocketsClient &_ws;
  String _buffer;
};
