//
// Created by sckomoroh on 4/24/18.
//

#pragma once

#include <memory>

#include "TcpClientSocket.h"
#include "SocketsDLL.h"

namespace sockets::impl::tcp
{
	using namespace ::sockets::tcp;

    class TcpServerSocket 
		: public TcpSocket
		, public ITcpServerSocket
    {
    public:
        TcpServerSocket(const char* serverIp, int32_t port);

        void bind() override;

        void listen() const override;

        ITcpClientSocketUPtr accept() override;
    };
}
