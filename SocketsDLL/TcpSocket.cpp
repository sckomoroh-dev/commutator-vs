//
// Created by sckomoroh on 05.05.18.
//
#include "stdafx.h"

#include "TcpSocket.h"
#include "SocketException.h"

using namespace sockets::impl::tcp;

TcpSocket::TcpSocket(const char *serverIp, int32_t port)
    : Socket(serverIp, port)
{
    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == INVALID_SOCKET)
    {
        throw SocketException("Cannot to create the socket", WSAGetLastError());
    }

	_pollRead.fd = _socket;
	_pollRead.events = POLLIN;

	_pollWrite.fd = _socket;
	_pollWrite.events = POLLOUT;
}
