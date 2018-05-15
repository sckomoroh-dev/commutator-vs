#pragma once

#include <map>
#include <functional>
#include <atomic>
#include <WinSock2.h>

#include <CNPDataContracts.h>
#include <SocketsDLL.h>

#include "CNPServerDLL.h"

namespace cnp::server::udp
{
	class CnpUdpServer : public ICnpServer
	{
	private:
		static std::shared_ptr<contracts::ICNPMessageFactory> _messageFactory;
		static std::shared_ptr<contracts::ICNPMessageParser> _messageParser;
		static std::shared_ptr<sockets::IUdpSocketFactory> _socketFactory;

		std::unique_ptr<sockets::udp::IUdpServerSocket> _socket;

		std::map<std::string, std::function<std::pair<std::string, contracts::CNPStatus>(std::string &)>> _methodsMap;
		std::atomic_bool _needStopServer { false };

	public:
		CnpUdpServer(CnpUdpServer&&) = default;
		CnpUdpServer(std::string serverIp, uint32_t port);

		~CnpUdpServer() override;

		void initializeServer() override;

		void startServer() override;

		void stopServer() override;

		void handleRequests() override;

		CnpUdpServer& operator =(const CnpUdpServer&) = delete;
		CnpUdpServer& operator =(CnpUdpServer&&) = default;

	private:
		static std::shared_ptr<contracts::ICNPMessageFactory> getMessageFactory();

		static std::shared_ptr<contracts::ICNPMessageParser> getMessageParser();

		static std::shared_ptr<sockets::IUdpSocketFactory> getSocketFactory();

		static void processClientRequest(CnpUdpServer* thisPtr, struct sockaddr_in clientSocketAddress, size_t requestLength);

		size_t readRequestLength(struct sockaddr_in* clientSocketAddress) const;
	
		std::shared_ptr<contracts::ICNPRequest> readRequest(struct sockaddr_in &targetAddress, size_t messageLen) const;
	
		std::shared_ptr<contracts::ICNPResponse> getResponse(const std::shared_ptr<contracts::ICNPRequest>& request);

		void sendResponse(std::shared_ptr<contracts::ICNPResponse>& response, struct sockaddr_in& clientSocketAddress) const;
	};
}
