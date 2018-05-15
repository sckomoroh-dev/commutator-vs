#pragma once

#include <memory>
#include <CNPDataContracts.h>

#ifdef EXPORT_SYMBOLS
#define SYMBOL_ATTRIBUTE	__declspec(dllexport)
#else
#define SYMBOL_ATTRIBUTE	__declspec(dllimport)
#endif

namespace cnp::client
{
	class CnpClientException;

	class ICnpClient
	{
	public:
		ICnpClient() = default;
		ICnpClient(ICnpClient&&) = default;

		virtual ~ICnpClient() = default;

		virtual void sendRequest(std::shared_ptr<contracts::ICNPRequest>&& request) = 0;

		virtual std::shared_ptr<contracts::ICNPResponse> readResponse() = 0;

		virtual void closeClient() = 0;

		ICnpClient& operator =(const ICnpClient&) = default;
		ICnpClient& operator =(ICnpClient&&) = default;
	};

	namespace tcp
	{
		class ICnpTcpClient : public ICnpClient
		{
		public:
			ICnpTcpClient() = default;
			ICnpTcpClient(const ICnpTcpClient&) = default;
			ICnpTcpClient(ICnpTcpClient&&) = default;

			virtual ~ICnpTcpClient() = default;

			virtual void connect() = 0;

			ICnpTcpClient& operator=(const ICnpTcpClient&) = default;
			ICnpTcpClient& operator=(ICnpTcpClient&&) = default;
		};
	}

	namespace udp
	{
		class ICnpUdpClient : public ICnpClient
		{
		public:
			ICnpUdpClient() = default;
			ICnpUdpClient(const ICnpUdpClient&) = default;
			ICnpUdpClient(ICnpUdpClient&&) = default;

			virtual ~ICnpUdpClient() = default;

			ICnpUdpClient& operator=(const ICnpUdpClient&) = default;
			ICnpUdpClient& operator=(ICnpUdpClient&&) = default;
		};
	}

	class ICnpClientFactory
	{
	public:
		ICnpClientFactory() = default;
		ICnpClientFactory(const ICnpClientFactory&) = default;
		ICnpClientFactory(ICnpClientFactory&&) = default;

		virtual ~ICnpClientFactory() = default;

		virtual std::unique_ptr<tcp::ICnpTcpClient> createTcpClient(const std::string& serverIp, uint32_t port) = 0;

		virtual std::unique_ptr<udp::ICnpUdpClient> createUdpClient(const std::string& serverIp, uint32_t port) = 0;

		ICnpClientFactory& operator =(const ICnpClientFactory&) = default;
		ICnpClientFactory& operator =(ICnpClientFactory&&) = default;
	};
}

SYMBOL_ATTRIBUTE
std::shared_ptr<cnp::client::ICnpClientFactory> createFactory();
