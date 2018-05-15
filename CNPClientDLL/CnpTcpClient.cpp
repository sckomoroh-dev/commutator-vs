#include "stdafx.h"

#include <SocketsDLL.h>
#include <CNPDataContracts.h>

#include "CnpTcpClient.h"
#include "CnpClientException.h"

using namespace cnp::client::tcp::impl;

std::shared_ptr<cnp::contracts::ICNPMessageParser> CnpTcpClient::_messageParser = nullptr;
std::shared_ptr<sockets::ITcpSocketFactory> CnpTcpClient::_socketFactory = nullptr;

CnpTcpClient::CnpTcpClient(std::string serverIp, uint32_t port)
{
	_socket = getSocketFactory()->createClientSocket(std::move(serverIp.c_str()), port);
}

CnpTcpClient::~CnpTcpClient()
{
	if (_socket != nullptr)
	{
		_socket->closeSocket();
		_socket = nullptr;
	}
}

void CnpTcpClient::connect()
{
	if (_socket == nullptr)
	{
		throw CnpClientException("Client socket was deleted");
	}

	_socket->connect();
}

void CnpTcpClient::sendRequest(std::shared_ptr<cnp::contracts::ICNPRequest>&& request)
{
	if (_socket == nullptr)
	{
		throw CnpClientException("Client socket was deleted");
	}

	const auto requestString = request->toString();
	size_t requestLen = requestString.length();

	_socket->sendBuffer(reinterpret_cast<char*>(&requestLen), sizeof size_t);
	_socket->sendBuffer(const_cast<char*>(requestString.c_str()), requestLen);
}

std::shared_ptr<cnp::contracts::ICNPResponse> CnpTcpClient::readResponse()
{
	if (_socket == nullptr)
	{
		throw CnpClientException("Client socket was deleted");
	}
	
	size_t responseLen = 0;
	_socket->readBuffer(reinterpret_cast<char*>(&responseLen), sizeof size_t);

	std::shared_ptr<char> buffer(
		[responseLen]() -> char*
		{
			char* buffer = static_cast<char*>(malloc(responseLen + 1));
			memset(buffer, 0, responseLen + 1);
			return buffer;
		}(),
		::free);

	_socket->readBuffer(buffer.get(), responseLen);

	return std::move(getMessageParser()->responseFromString(buffer.get()));
}

void CnpTcpClient::closeClient()
{
	if (_socket == nullptr)
	{
		throw CnpClientException("Client socket was deleted");
	}

	_socket->closeSocket();
	_socket = nullptr;
}

std::shared_ptr<cnp::contracts::ICNPMessageParser> CnpTcpClient::getMessageParser()
{
	if (_messageParser == nullptr)
	{
		_messageParser = createMessageParser();
	}

	return _messageParser;
}

std::shared_ptr<sockets::ITcpSocketFactory> CnpTcpClient::getSocketFactory()
{
	if (_socketFactory == nullptr)
	{
		_socketFactory = getTcpSocketFactory();
	}

	return _socketFactory;
}