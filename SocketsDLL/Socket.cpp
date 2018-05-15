//
// Created by sckomoroh on 05.05.18.
//
#include "stdafx.h"

#include <WS2tcpip.h>

#include "Socket.h"
#include "SocketException.h"

using namespace sockets::impl;

Socket::Socket()
	: _socket(INVALID_SOCKET)
	, _targetSocketAddress{ 0 }
	, _serverIp("")
	, _port(-1)
{
}

Socket::Socket(const char* serverIp, int32_t port)
    : _socket(INVALID_SOCKET)
    , _targetSocketAddress { 0 }
	, _serverIp(serverIp)
	, _port(port)
{
	printf("Creating socket\n");

    _targetSocketAddress.sin_family = AF_INET;
    _targetSocketAddress.sin_port = htons(port);
	
	if (inet_pton(AF_INET, serverIp, &_targetSocketAddress.sin_addr.s_addr) != 1)
	{
		throw SocketException("Invalid network address", WSAGetLastError());
	}
}

Socket::~Socket()
{
	if (_socket != INVALID_SOCKET)
	{
		Socket::closeSocket();
	}
}

void Socket::closeSocket()
{
    if (_socket != INVALID_SOCKET)
    {
		::closesocket(_socket);

        _socket = INVALID_SOCKET;
    }
	else
	{
		throw SocketException("Socket was not created");
	}
}

unsigned long Socket::socketOptions(int64_t command, unsigned long args)
{
	auto err = ioctlsocket(_socket, command, &args);

	return args;
}


bool Socket::readyForRead()
{
	return WSAPoll(&_pollRead, 1, 0) > 0;
}

bool Socket::readyForWrite()
{
	return WSAPoll(&_pollRead, 1, 0) > 0;
}

bool Socket::readyForRead(int timeout)
{
	return WSAPoll(&_pollRead, 1, timeout) > 0;
}

bool Socket::readyForWrite(int timeout)
{
	return WSAPoll(&_pollRead, 1, timeout) > 0;
}
