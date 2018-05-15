#pragma once

#include <string>
#include <memory>

#ifdef EXPORT_SYMBOLS
#define SYMBOL_ATTRIBUTE	__declspec(dllexport)
#else
#define SYMBOL_ATTRIBUTE	__declspec(dllimport)
#endif

namespace cnp::server
{
	class CnpServerException;

	class ICnpServer
	{
	public:
		ICnpServer() = default;

		ICnpServer(ICnpServer&&) = default;

		virtual ~ICnpServer() = default;

		virtual void initializeServer() = 0;

		virtual void startServer() = 0;

		virtual void stopServer() = 0;

		virtual void handleRequests() = 0;

		ICnpServer& operator =(const ICnpServer&) = delete;
		ICnpServer& operator =(ICnpServer&&) = default;
	};

	class ICnpServerFactory
	{
	public:
		ICnpServerFactory() = default;
		ICnpServerFactory(const ICnpServerFactory&) = default;
		ICnpServerFactory(ICnpServerFactory&&) = default;

		virtual ~ICnpServerFactory() = default;

		virtual std::unique_ptr<ICnpServer> createTcpServer(const std::string& serverIp, uint32_t port) = 0;

		virtual std::unique_ptr<ICnpServer> createUdpServer(const std::string& serverIp, uint32_t port) = 0;

		ICnpServerFactory& operator =(const ICnpServerFactory&) = default;
		ICnpServerFactory& operator =(ICnpServerFactory&&) = default;
	};
}

SYMBOL_ATTRIBUTE
std::shared_ptr<cnp::server::ICnpServerFactory> getServerFactory();