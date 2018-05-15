#pragma once

#include <memory>
#include <string>

#include <SocketsDLL.h>
#include <CNPDataContracts.h>

#include "CNPClientDLL.h"

namespace cnp::client::tcp::impl
{
	class CnpTcpClient : public ICnpTcpClient
	{
	private:
		static std::shared_ptr<cnp::contracts::ICNPMessageParser> _messageParser;
		static std::shared_ptr<sockets::ITcpSocketFactory> _socketFactory;

		std::unique_ptr<sockets::tcp::ITcpClientSocket> _socket;

	public:
		CnpTcpClient() = delete;
		CnpTcpClient(const CnpTcpClient&) = delete;
		CnpTcpClient(CnpTcpClient&& move) = default;
		CnpTcpClient(std::string serverIp, uint32_t port);

		~CnpTcpClient() override;

		void connect() override;

		void sendRequest(std::shared_ptr<contracts::ICNPRequest>&& request) override;

		std::shared_ptr<contracts::ICNPResponse> readResponse() override;

		void closeClient() override;

		CnpTcpClient& operator=(const CnpTcpClient&) = delete;
		CnpTcpClient& operator=(CnpTcpClient&&) = default;

	private:
		static std::shared_ptr<contracts::ICNPMessageParser> getMessageParser();

		static std::shared_ptr<sockets::ITcpSocketFactory> getSocketFactory();
	};
}
