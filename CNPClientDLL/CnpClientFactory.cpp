#include "stdafx.h"

#include "CnpClientFactory.h"

#include "CnpTcpClient.h"
#include "CnpUdpClient.h"

using namespace cnp::client::impl;

std::unique_ptr<cnp::client::tcp::ICnpTcpClient> CnpClientFactory::createTcpClient(const std::string & serverIp, uint32_t port)
{
	return std::move(std::make_unique<tcp::impl::CnpTcpClient>(serverIp, port));
}

std::unique_ptr<cnp::client::udp::ICnpUdpClient> CnpClientFactory::createUdpClient(const std::string & serverIp, uint32_t port)
{
	return std::move(std::make_unique<udp::impl::CnpUdpClient>(serverIp, port));
}
