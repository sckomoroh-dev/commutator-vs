//
// Created by sckomoroh on 05.05.18.
//
#include "stdafx.h"

#include "TcpServerSocket.h"
#include "SocketException.h"

using namespace sockets::impl::tcp;

TcpServerSocket::TcpServerSocket(const char *serverIp, int32_t port)
    : TcpSocket(serverIp, port)
{
}

void TcpServerSocket::bind()
{
    if (::bind(
		_socket,
		reinterpret_cast<struct sockaddr *>(&_targetSocketAddress),
		sizeof(_targetSocketAddress)) < 0)
    {
        throw SocketException("Unable to bind server socket", WSAGetLastError());
    }
}

void TcpServerSocket::listen() const
{
    if (::listen(_socket, 1) == -1)
    {
        throw SocketException("Unable to start listen", WSAGetLastError());
    }
}

ITcpClientSocketUPtr TcpServerSocket::accept()
{
    struct sockaddr_in clientSocketAddress = {0};
    auto clientSocketAddressSize = sizeof clientSocketAddress;

    SOCKET clientSocketDescriptor = ::accept(
		_socket,
		reinterpret_cast<struct sockaddr *>(&clientSocketAddress),
		reinterpret_cast<int*>(&clientSocketAddressSize));

    if (clientSocketDescriptor == INVALID_SOCKET)
    {
        throw SocketException("Unable to accept client socket", WSAGetLastError());
    }

    return createSocket<TcpClientSocket>(clientSocketDescriptor, clientSocketAddress);
}
