#pragma once

#include "CNPServerDLL.h"

namespace cnp::server
{
	class CnpServerFactory : public ICnpServerFactory
	{
	public:
		CnpServerFactory() = default;
		CnpServerFactory(const CnpServerFactory&) = default;
		CnpServerFactory(CnpServerFactory&&) = default;

		~CnpServerFactory() override = default;

		std::unique_ptr<ICnpServer> createTcpServer(const std::string& serverIp, uint32_t port) override;

		std::unique_ptr<ICnpServer> createUdpServer(const std::string& serverIp, uint32_t port) override;

		CnpServerFactory& operator =(const CnpServerFactory&) = default;
		CnpServerFactory& operator =(CnpServerFactory&&) = default;
	};
}
