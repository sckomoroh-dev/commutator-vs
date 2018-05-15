#pragma once

#include <string>
#include "CNPClientDLL.h"

namespace cnp::client::impl
{
	class CnpClientFactory : public ICnpClientFactory
	{
	public:
		CnpClientFactory() = default;
		CnpClientFactory(const CnpClientFactory&) = default;
		CnpClientFactory(CnpClientFactory&&) = default;

		std::unique_ptr<tcp::ICnpTcpClient> createTcpClient(const std::string& serverIp, uint32_t port) override;

		std::unique_ptr<udp::ICnpUdpClient> createUdpClient(const std::string& serverIp, uint32_t port) override;

		~CnpClientFactory() override = default;

		CnpClientFactory& operator =(const CnpClientFactory&) = default;
		CnpClientFactory& operator =(CnpClientFactory&&) = default;
	};
}
