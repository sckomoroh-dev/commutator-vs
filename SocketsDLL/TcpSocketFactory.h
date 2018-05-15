#pragma once

#include "SocketsDLL.h"

namespace sockets::impl
{
	class TcpSocketFactory : public sockets::ITcpSocketFactory
	{
	public:
		std::unique_ptr<sockets::tcp::ITcpClientSocket> createClientSocket(const char* serverIp, int32_t port) override;

		std::unique_ptr<sockets::tcp::ITcpServerSocket> createServerSocket(const char* serverIp, int32_t port) override;
	};
}
