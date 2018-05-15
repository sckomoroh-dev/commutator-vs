#include "stdafx.h"

#include "UdpSocketFactory.h"

#include "UdpServerSocket.h"
#include "UdpClientSocket.h"

using namespace sockets::impl;

std::unique_ptr<udp::IUdpClientSocket> UdpSocketFactory::createClientSocket(const char* serverIp, int32_t port)
{
	return std::move(std::make_unique<sockets::impl::udp::UdpClientSocket>(serverIp, port));
}

std::unique_ptr<udp::IUdpServerSocket> UdpSocketFactory::createServerSocket(const char* serverIp, int32_t port)
{
	return std::move(std::make_unique<sockets::impl::udp::UdpServerSocket>(serverIp, port));
}
