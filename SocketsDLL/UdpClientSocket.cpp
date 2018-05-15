//
// Created by sckomoroh on 05.05.18.
//
#include "stdafx.h"

#include "UdpClientSocket.h"
#include "SocketException.h"

using namespace sockets::impl::udp;

UdpClientSocket::UdpClientSocket(const char *serverIp, int32_t port)
    : UdpSocket(serverIp, port)
{
}

void UdpClientSocket::sendBuffer(char *buffer, size_t bufferSize)
{
	const auto sendtoResult = sendto(
		_socket,
		buffer,
		bufferSize,
		0,
		const_cast<const struct sockaddr*>(reinterpret_cast<struct sockaddr*>(&_targetSocketAddress)),
		sizeof (struct sockaddr_in));

    if (sendtoResult < 1)
    {
        throw SocketException("Unable to send data to socket", WSAGetLastError());
    }
}

void UdpClientSocket::readBuffer(char *buffer, size_t bufferSize)
{
    int socketAddressStructureSize = sizeof _targetSocketAddress ;
    if (recvfrom(_socket,
                 buffer,
                 bufferSize,
                 0,
				 reinterpret_cast<struct sockaddr*>(&_targetSocketAddress),
                 &socketAddressStructureSize) < 1)
    {
        throw SocketException("Unable to receive data from socket", WSAGetLastError());
    }
}
