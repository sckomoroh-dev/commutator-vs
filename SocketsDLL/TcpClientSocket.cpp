//
// Created by sckomoroh on 05.05.18.
//
#include "stdafx.h"

#include "TcpClientSocket.h"
#include "SocketException.h"

using namespace sockets::impl::tcp;

TcpClientSocket::TcpClientSocket(const char *serverIp, int32_t port)
        : TcpSocket(serverIp, port)
{
}

void TcpClientSocket::connect()
{
    if (::connect(_socket,
                  reinterpret_cast<struct sockaddr *>(&_targetSocketAddress),
                  sizeof(_targetSocketAddress)) == -1)
    {
        throw SocketException("Unable to connect to server socket", WSAGetLastError());
    }
}

void TcpClientSocket::sendBuffer(char *buffer, size_t bufferSize) const
{
    if (send(_socket,
             buffer,
             bufferSize,
             0) < 1)
    {
        throw SocketException("Unable to send data to socket", WSAGetLastError());
    }
}

void TcpClientSocket::readBuffer(char *buffer, size_t bufferSize) const
{
    if (recv(_socket,
             buffer,
             bufferSize,
             0) < 1)
    {
        throw SocketException("Unable to receive data from socket", WSAGetLastError());
    }
}
