//
// Created by sckomoroh on 4/24/18.
//

#pragma once

#include "TcpSocket.h"
#include "SocketsDLL.h"

namespace sockets::impl::tcp
{
	class TcpClientSocket 
		: public sockets::tcp::ITcpClientSocket
		, public TcpSocket
	{
	public:
		TcpClientSocket(const char* serverIp, int32_t port);

		TcpClientSocket() = default;

		void connect() override;

		void sendBuffer(char *buffer, size_t bufferSize) const override;

		void readBuffer(char *buffer, size_t bufferSize) const override;
	};
}
