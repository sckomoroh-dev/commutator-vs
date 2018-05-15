#pragma once

#include <memory>

#include "SocketsDLL.h"

namespace sockets::impl
{
	class UdpSocketFactory : public IUdpSocketFactory
	{
	public:
		std::unique_ptr<udp::IUdpClientSocket> createClientSocket(const char* serverIp, int32_t port) override;

		std::unique_ptr<udp::IUdpServerSocket> createServerSocket(const char* serverIp, int32_t port) override;
	};
}
