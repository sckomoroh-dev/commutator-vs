#include "stdafx.h"

#include <SocketsDLL.h>

#include "CnpUdpClient.h"
#include "CnpClientException.h"

using namespace cnp::client::udp::impl;

std::shared_ptr<cnp::contracts::ICNPMessageParser> CnpUdpClient::_messageParser = nullptr;
std::shared_ptr<sockets::IUdpSocketFactory> CnpUdpClient::_socketFactory = nullptr;

CnpUdpClient::CnpUdpClient(std::string serverIp, uint32_t port)
{
	_socket = getSocketFactory()->createClientSocket(std::move(serverIp.c_str()), port);
}

CnpUdpClient::~CnpUdpClient()
{
	if (_socket != nullptr) 
	{
		_socket->closeSocket();
		_socket = nullptr;
	}
}

void CnpUdpClient::sendRequest(std::shared_ptr<cnp::contracts::ICNPRequest>&& request)
{
	if (_socket == nullptr)
	{
		throw CnpClientException("Client socket was deleted");
	}

	const auto requestString = request->toString();
	uint32_t requestLen = requestString.length();

	_socket->sendBuffer(reinterpret_cast<char*>(&requestLen), sizeof uint32_t);
	_socket->sendBuffer(const_cast<char*>(requestString.c_str()), requestLen);
}

std::shared_ptr<cnp::contracts::ICNPResponse> CnpUdpClient::readResponse()
{
	if (_socket == nullptr)
	{
		throw CnpClientException("Client socket was deleted");
	}

	uint32_t responseLen = 0;
	_socket->readBuffer(reinterpret_cast<char*>(&responseLen), sizeof uint32_t);

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

void CnpUdpClient::closeClient()
{
	if (_socket == nullptr)
	{
		throw CnpClientException("Client socket was deleted");
	}

	_socket->closeSocket();
	_socket = nullptr;
}

std::shared_ptr<cnp::contracts::ICNPMessageParser> CnpUdpClient::getMessageParser()
{
	if (_messageParser == nullptr)
	{
		_messageParser = createMessageParser();
	}

	return _messageParser;
}

std::shared_ptr<sockets::IUdpSocketFactory> cnp::client::udp::impl::CnpUdpClient::getSocketFactory()
{
	if (_socketFactory == nullptr)
	{
		_socketFactory = getUdpSocketFactory();
	}

	return _socketFactory;
}
