#pragma once

#include <memory>
#include <string>

#include <SocketsDLL.h>
#include <CNPDataContracts.h>

#include "CNPClientDLL.h"

namespace cnp::client::udp::impl
{
	class CnpUdpClient : public ICnpUdpClient
	{
	private:
		static std::shared_ptr<contracts::ICNPMessageParser> _messageParser;
		static std::shared_ptr<sockets::IUdpSocketFactory> _socketFactory;

		std::unique_ptr<sockets::udp::IUdpClientSocket> _socket;

	public:
		CnpUdpClient() = delete;
		CnpUdpClient(const CnpUdpClient&) = delete;
		CnpUdpClient(CnpUdpClient&& move) = default;
		CnpUdpClient(std::string serverIp, uint32_t port);

		~CnpUdpClient() override;

		void sendRequest(std::shared_ptr<contracts::ICNPRequest>&& request) override;

		std::shared_ptr<contracts::ICNPResponse> readResponse() override;

		void closeClient() override;

		CnpUdpClient& operator=(const CnpUdpClient&) = delete;
		CnpUdpClient& operator=(CnpUdpClient&&) = default;

	private:
		static std::shared_ptr<contracts::ICNPMessageParser> getMessageParser();
	
		static std::shared_ptr<sockets::IUdpSocketFactory> getSocketFactory();
	};
}
