#include "stdafx.h"
#include "CnpServerFactory.h"
#include "CnpTcpServer.h"
#include "CnpUdpServer.h"

using namespace cnp::server;

std::unique_ptr<ICnpServer> CnpServerFactory::createTcpServer(const std::string& serverIp, uint32_t port)
{
	return std::move(std::make_unique<tcp::CnpTcpServer>(serverIp, port));
}

std::unique_ptr<ICnpServer> CnpServerFactory::createUdpServer(const std::string& serverIp, uint32_t port)
{
	return std::move(std::make_unique<udp::CnpUdpServer>(serverIp, port));
}
