// SocketsTestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <thread>
#include <atomic>

#include <SocketsDLL.h>
#include <CNPServerDLL.h>
#include <CNPClientDLL.h>
#include <CNPDataContracts.h>

std::atomic_bool g_isInit(false);
auto g_clientFactory = createFactory();
auto g_serverFactory = getServerFactory();
auto g_requestFactory = createMessageFactory();

std::unique_ptr<cnp::server::ICnpServer> g_server;

void client()
{
	while (!g_isInit)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	auto client = g_clientFactory->createTcpClient("127.0.0.1", 9009);
	client->connect();

	std::string command("Echo");
	std::string data("Echo data");

	auto request = g_requestFactory->createRequest(cnp::contracts::CNPVersion::Version10, command, data);
	client->sendRequest(std::move(request));
	auto response = client->readResponse();

	g_server->stopServer();
}

void server()
{
	//while (true)
	{
		try
		{
			g_server = g_serverFactory->createTcpServer("127.0.0.1", 9009);
			g_server->initializeServer();
			g_server->startServer();

			g_isInit = true;

			g_server->handleRequests();
		}
		catch (std::exception& ex)
		{
			int a = 0;
		}
	}
}

int main()
{
	initWinSocks();

	std::thread clientThread(client);
	std::thread serverThread(server);

	clientThread.join();
	serverThread.join();


	cleanupWinSock();

    return 0;
}

