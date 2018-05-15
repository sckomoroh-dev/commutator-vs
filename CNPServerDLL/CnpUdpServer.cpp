#include "stdafx.h"
#include "CnpUdpServer.h"

#include <thread>
#include <WinSock2.h>

#include "CnpServerException.h"

using namespace cnp::server::udp;

std::shared_ptr<cnp::contracts::ICNPMessageFactory> CnpUdpServer::_messageFactory = nullptr;
std::shared_ptr<cnp::contracts::ICNPMessageParser> CnpUdpServer::_messageParser = nullptr;
std::shared_ptr<sockets::IUdpSocketFactory> CnpUdpServer::_socketFactory = nullptr;

CnpUdpServer::CnpUdpServer(std::string serverIp, uint32_t port)
{
	_socket = getSocketFactory()->createServerSocket(std::move(serverIp.c_str()), port);
}

CnpUdpServer::~CnpUdpServer()
{
	if (_socket != nullptr)
	{
		_socket->closeSocket();
		_socket = nullptr;
	}
}

void CnpUdpServer::initializeServer()
{
	auto getVersionFunc = [](std::string& input) -> std::pair<std::string, contracts::CNPStatus>
	{
		return std::make_pair<const std::string, contracts::CNPStatus>("v1.0", contracts::CNPStatus::StatusOk);
	};

	_methodsMap["GetVersion"] = getVersionFunc;

	auto echoFunc = [](std::string& input) -> std::pair<std::string, contracts::CNPStatus>
	{
		return std::make_pair<const std::string, contracts::CNPStatus>(std::move(input), contracts::CNPStatus::StatusOk);
	};

	_methodsMap["Echo"] = echoFunc;
}

void CnpUdpServer::startServer()
{
	_socket->bind();
}

void CnpUdpServer::stopServer()
{
	_needStopServer = true;
}

void CnpUdpServer::handleRequests()
{
	while (!_needStopServer)
	{
		struct ::sockaddr_in clientSocketAddress = { 0 };
		auto requestLength = readRequestLength(&clientSocketAddress);
		if (requestLength < 0)
		{
			continue;
		}

		auto clientThread = std::thread(processClientRequest, this, clientSocketAddress, requestLength);
		clientThread.detach();
	}
}

std::shared_ptr<cnp::contracts::ICNPMessageFactory> CnpUdpServer::getMessageFactory()
{
	if (_messageFactory == nullptr)
	{
		_messageFactory = createMessageFactory();
	}

	return _messageFactory;
}

std::shared_ptr<cnp::contracts::ICNPMessageParser> CnpUdpServer::getMessageParser()
{
	if (_messageParser == nullptr)
	{
		_messageParser = createMessageParser();
	}

	return _messageParser;
}

std::shared_ptr<::sockets::IUdpSocketFactory> CnpUdpServer::getSocketFactory()
{
	if (_socketFactory == nullptr)
	{
		_socketFactory = getUdpSocketFactory();
	}

	return _socketFactory;
}


void CnpUdpServer::processClientRequest(CnpUdpServer* thisPtr, struct sockaddr_in clientSocketAddress, size_t requestLength)
{
	const auto request = thisPtr->readRequest(clientSocketAddress, requestLength);
	auto response = thisPtr->getResponse(request);

	thisPtr->sendResponse(response, clientSocketAddress);
}

size_t CnpUdpServer::readRequestLength(struct sockaddr_in* clientSocketAddress) const
{
	size_t messageHeaderLen = 0;

	_socket->readBuffer(reinterpret_cast<char*>(&messageHeaderLen), sizeof messageHeaderLen, clientSocketAddress);

	return messageHeaderLen;
}

std::shared_ptr<cnp::contracts::ICNPRequest> CnpUdpServer::readRequest(struct sockaddr_in &targetAddress, size_t messageLen) const
{
	std::shared_ptr<char> buffer([messageLen]() -> char*
	{
		auto buffer = static_cast<char *>(malloc(messageLen + 1));
		memset(buffer, 0, messageLen + 1);

		return buffer;
	}(),
		::free);

	_socket->readBuffer(static_cast<char *>(buffer.get()), messageLen, &targetAddress);

	auto request = getMessageParser()->requestFromString(buffer.get());

	return request;
}

std::shared_ptr<cnp::contracts::ICNPResponse> CnpUdpServer::getResponse(const std::shared_ptr<cnp::contracts::ICNPRequest>& request)
{
	if (request->command().empty())
	{
		printf("ERROR: The operation was not specified\n");

		throw CnpServerException("The operation was not specified");
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

	const auto operationResultFunc = operationMethod->second;
	
	auto requestData = request->data();
	const auto operationResult = operationResultFunc(requestData);

	auto command = request->command();
	auto responseData = operationResult.first;

	return getMessageFactory()->createResponse(contracts::CNPVersion::Version10, operationResult.second, command, responseData);
}

void CnpUdpServer::sendResponse(std::shared_ptr<cnp::contracts::ICNPResponse>& response, struct sockaddr_in& clientSocketAddress) const
{
	auto responseString = response->toString();

	size_t responseLength = responseString.length();
	_socket->sendBuffer(reinterpret_cast<char*>(&responseLength), sizeof(size_t), clientSocketAddress);
	_socket->sendBuffer(const_cast<char*>(responseString.c_str()), responseLength, clientSocketAddress);
}
