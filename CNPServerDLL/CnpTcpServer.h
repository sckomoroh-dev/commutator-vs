#pragma once
#include "CNPServerDLL.h"

#include <atomic>
#include <memory>
#include <string>
#include <functional>
#include <map>

#include <SocketsDLL.h>
#include <CNPDataContracts.h>

namespace cnp::server::tcp
{
	class CnpTcpServer : public ICnpServer
	{
	private:
		static std::shared_ptr<contracts::ICNPMessageFactory> _messageFactory;
		static std::shared_ptr<contracts::ICNPMessageParser> _messageParser;
		static std::shared_ptr<sockets::ITcpSocketFactory> _socketFactory;
		
		std::unique_ptr<sockets::tcp::ITcpServerSocket> _socket;
		std::map<std::string, std::function<std::pair<std::string, contracts::CNPStatus>(std::string &)>> _methodsMap;
		std::atomic_bool _needStopServer { false };

	public:
		CnpTcpServer(CnpTcpServer&&) = default;
		CnpTcpServer(std::string serverIp, uint32_t port);

		~CnpTcpServer() override;

		void initializeServer() override;

		void startServer() override;

		void stopServer() override;

		void handleRequests() override;

		CnpTcpServer& operator =(const CnpTcpServer&) = delete;
		CnpTcpServer& operator =(CnpTcpServer&&) = default;

	private:
		static std::shared_ptr<contracts::ICNPMessageFactory> getMessageFactory();

		static std::shared_ptr<contracts::ICNPMessageParser> getMessageParser();

		static std::shared_ptr<sockets::ITcpSocketFactory> getSocketFactory();

		static void processClientRequest(CnpTcpServer* thisPtr, std::unique_ptr<sockets::tcp::ITcpClientSocket>&& clientSocket);
	
		static size_t readRequestLength(std::unique_ptr<sockets::tcp::ITcpClientSocket>& clientSocket);

		static std::shared_ptr<contracts::ICNPRequest> readRequest(std::unique_ptr<sockets::tcp::ITcpClientSocket>& clientSocket, size_t messageLen);

		std::shared_ptr<contracts::ICNPResponse> getResponse(const std::shared_ptr<contracts::ICNPRequest>& request);

		template<typename TResponse>
		void sendResponse(std::unique_ptr<sockets::tcp::ITcpClientSocket>& clientSocket, TResponse&& response)
		{
			const auto responseString = response->toString();

			size_t responseLength = responseString.length();
			clientSocket->sendBuffer(reinterpret_cast<char*>(&responseLength), sizeof(size_t));
			clientSocket->sendBuffer(static_cast<char*>(const_cast<char*>(responseString.c_str())), responseLength);
		}
	};
}
