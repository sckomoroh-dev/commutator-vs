//
// Created by sckomoroh on 4/25/18.
//

#pragma once

#include "UdpSocket.h"
#include "SocketsDLL.h"

namespace sockets::impl::udp
{
	using namespace ::sockets::udp;

    class UdpServerSocket 
		: public IUdpServerSocket
		, public UdpSocket
    {
    public:
        UdpServerSocket(const char* serverIp, int32_t port);

        void bind() override;

        void sendBuffer(char *buffer, size_t bufferSize, struct sockaddr_in& targetAddress) const override;

        void readBuffer(char *buffer, size_t bufferSize, struct sockaddr_in* clientSocketAddress) const override;
    };
}
