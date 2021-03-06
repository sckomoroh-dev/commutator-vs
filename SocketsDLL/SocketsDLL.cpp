// SocketsDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include <memory>
#include <WinSock2.h>

#include "SocketsDLL.h"

#include "TcpSocketFactory.h"
#include "UdpSocketFactory.h"

static WSAData g_wsaData;

void initWinSocks()
{
	WSAStartup(MAKEWORD(2, 2), &g_wsaData);
}

void cleanupWinSock()
{
	WSACleanup();
}

std::shared_ptr<sockets::ITcpSocketFactory> getTcpSocketFactory()
{
	return std::make_shared<sockets::impl::TcpSocketFactory>();
}

std::shared_ptr<sockets::IUdpSocketFactory> getUdpSocketFactory()
{
	return std::make_shared<sockets::impl::UdpSocketFactory>();
}
