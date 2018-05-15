//
// Created by sckomoroh on 05.05.18.
//
#include "stdafx.h"

#include "UdpServerSocket.h"
#include "SocketException.h"

using namespace sockets::impl::udp;

UdpServerSocket::UdpServerSocket(const char *serverIp, int32_t port)
    : UdpSocket(serverIp, port)
{
	printf("Creating UDP server socket\n");
}

void UdpServerSocket::bind()
{
    if (::bind(
		_socket,
		reinterpret_cast<struct sockaddr *>(&_targetSocketAddress),
		sizeof _targetSocketAddress) < 0)
    {
        throw SocketException("Unable to bind server socket", WSAGetLastError());
    }
}

 void UdpServerSocket::readBuffer(char *buffer, size_t bufferSize, struct sockaddr_in* clientSocketAddress) const
{
    int socketAddressStructureSize = sizeof clientSocketAddress;
    if (recvfrom(
		_socket,
		buffer,
		bufferSize,
		0,
		reinterpret_cast<struct sockaddr*>(clientSocketAddress),
		&socketAddressStructureSize) < 1)
    {
        throw SocketException("Unable to receive data from socket", WSAGetLastError());
    }
}

void UdpServerSocket::sendBuffer(char *buffer, size_t bufferSize, struct sockaddr_in &targetAddress) const
{
    if (sendto(
		_socket,
		buffer,
		bufferSize,
		0,
		const_cast<const struct sockaddr*>(reinterpret_cast<struct sockaddr*>(&targetAddress)),
		sizeof(struct sockaddr)) < 1)
    {
        throw SocketException("Unable to send data to socket", WSAGetLastError());
    }
}
