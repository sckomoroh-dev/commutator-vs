//
// Created by sckomoroh on 4/25/18.
//

#pragma once

#include <cstdint>

#include "Socket.h"

namespace sockets::impl::udp
{
    class UdpSocket : public Socket
    {
    protected:
        UdpSocket(const char* serverIp, int32_t port);

        UdpSocket() = default;
    };
}
