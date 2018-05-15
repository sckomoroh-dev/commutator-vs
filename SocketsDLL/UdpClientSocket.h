//
// Created by sckomoroh on 4/25/18.
//

#pragma once

#include <cstdint>

#include "UdpSocket.h"
#include "SocketsDLL.h"

namespace sockets::impl::udp
{
	using namespace ::sockets::udp;

    class UdpClientSocket 
		: public IUdpClientSocket
		, public UdpSocket
    {
    public:
        UdpClientSocket(const char* serverIp, int32_t port);

        UdpClientSocket() = default;

        void sendBuffer(char *buffer, size_t bufferSize) override;

        void readBuffer(char *buffer, size_t bufferSize) override;
    };
}
