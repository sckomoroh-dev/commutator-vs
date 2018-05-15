#include "stdafx.h"
#include "CnpTcpServer.h"

#include <WinSock2.h>
#include <thread>

#include "CnpServerException.h"

using namespace cnp::server::tcp;

std::shared_ptr<cnp::contracts::ICNPMessageFactory> CnpTcpServer::_messageFactory = nullptr;
std::shared_ptr<cnp::contracts::ICNPMessageParser> CnpTcpServer::_messageParser = nullptr;
std::shared_ptr<sockets::ITcpSocketFactory> CnpTcpServer::_socketFactory = nullptr;

CnpTcpServer::CnpTcpServer(std::string serverIp, uint32_t port)
{
	_socket = getSocketFactory()->createServerSocket(std::move(serverIp.c_str()), port);
	_socket->socketOptions(FIONBIO, 1); // Set non-block
}

CnpTcpServer::~CnpTcpServer()
{
	if (_socket != nullptr)
	{
		_socket->closeSocket();
		_socket = nullptr;
	}
}

void CnpTcpServer::initializeServer()
{
	auto getVersionFunc = [](std::string& input) -> std::pair<std::string, contracts::CNPStatus>
	{
		return std::make_pair<const std::string, contracts::CNPStatus>(std::move("v1.0"), contracts::CNPStatus::StatusOk);
	};

	_methodsMap["GetVersion"] = getVersionFunc;

	auto echoFunc = [](std::string& input) -> std::pair<std::string, contracts::CNPStatus>
	{
		return std::make_pair<const std::string, contracts::CNPStatus>(std::move(input), contracts::CNPStatus::StatusOk);
	};

	_methodsMap["Echo"] = echoFunc;
}

void CnpTcpServer::startServer()
{
	_socket->bind();
	_socket->listen();
}

void CnpTcpServer::stopServer()
{
	_needStopServer = true;
}

void CnpTcpServer::handleRequests()
{
	while (!_needStopServer)
	{
		if (!_socket->readyForRead(250))
		{
			continue;
		}

		auto clientSocket = _socket->accept();
		auto thread = std::thread(processClientRequest, this, std::move(clientSocket));
		thread.detach();
	}
}

std::shared_ptr<cnp::contracts::ICNPMessageFactory> CnpTcpServer::getMessageFactory()
{
	if (_messageFactory == nullptr)
	{
		_messageFactory = createMessageFactory();
	}

	return _messageFactory;
}

std::shared_ptr<cnp::contracts::ICNPMessageParser> CnpTcpServer::getMessageParser()
{
	if (_messageParser == nullptr)
	{
		_messageParser = createMessageParser();
	}

	return _messageParser;
}

std::shared_ptr<::sockets::ITcpSocketFactory> CnpTcpServer::getSocketFactory()
{
	if (_socketFactory == nullptr)
	{
		_socketFactory = getTcpSocketFactory();
	}

	return _socketFactory;
}


void CnpTcpServer::processClientRequest(CnpTcpServer* thisPtr, std::unique_ptr<sockets::tcp::ITcpClientSocket>&& clientSocket)
{
	const auto messageLen = readRequestLength(clientSocket);
	if (messageLen < 1)
	{
		return;
	}

	const auto request = readRequest(clientSocket, messageLen);

	auto response = thisPtr->getResponse(request);

	thisPtr->sendResponse(clientSocket, response);
}

size_t CnpTcpServer::readRequestLength(std::unique_ptr<sockets::tcp::ITcpClientSocket>& clientSocket)
{
	size_t messageHeaderLen = 0;

	clientSocket->readBuffer(reinterpret_cast<char*>(&messageHeaderLen), sizeof(size_t));

	return messageHeaderLen;
}

std::shared_ptr<cnp::contracts::ICNPRequest> CnpTcpServer::readRequest(std::unique_ptr<sockets::tcp::ITcpClientSocket>& clientSocket, size_t messageLen)
{
	std::shared_ptr<char> buffer([messageLen]() -> char*
	{
		auto buffer = static_cast<char *>(malloc(messageLen + 1));
		memset(buffer, 0, messageLen + 1);

		return buffer;
	}(),
		::free);

	clientSocket->readBuffer(static_cast<char *>(buffer.get()), messageLen);

	auto request = getMessageParser()->requestFromString(buffer.get());

	return request;
}

std::shared_ptr<cnp::contracts::ICNPResponse> CnpTcpServer::getResponse(const std::shared_ptr<cnp::contracts::ICNPRequest>& request)
{
	if (request->command().empty())
	{
		printf("ERROR: The operation was not specified\n");

		throw CnpServerException("ERROR: The operation was not specified\n");
	}

	if (request->data().empty())
	{
		printf("The data was not specified\n");
	}

	const auto operationMethod = _methodsMap.find(request->command());
	if (operationMethod == _methodsMap.end())
	{
		printf("ERROR: The method implementation was not found\n");

		throw CnpServerException("The method implementation was not found");
	}
	auto requestData = request->data();
	const auto operationResultFunc = operationMethod->second;
	const auto operationResult = operationResultFunc(requestData);

	auto responseCommand = request->command();
	auto responseData = operationResult.first;

	return getMessageFactory()->createResponse(contracts::CNPVersion::Version10, operationResult.second, responseCommand, responseData);
}
