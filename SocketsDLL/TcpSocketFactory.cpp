#include "stdafx.h"

#include "TcpSocketFactory.h"

#include "TcpClientSocket.h"
#include "TcpServerSocket.h"

using namespace sockets::impl;

std::unique_ptr<sockets::tcp::ITcpClientSocket> TcpSocketFactory::createClientSocket(const char * serverIp, int32_t port)
{
	return std::move(std::make_unique<sockets::impl::tcp::TcpClientSocket>(serverIp, port));
}

std::unique_ptr<sockets::tcp::ITcpServerSocket> TcpSocketFactory::createServerSocket(const char * serverIp, int32_t port)
{
	return std::move(std::make_unique<sockets::impl::tcp::TcpServerSocket>(serverIp, port));
}
