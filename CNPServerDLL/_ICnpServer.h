#pragma once

namespace cnp::server
{
	class ICnpServer
	{
	public:
		ICnpServer() = default;
		ICnpServer(const ICnpServer&) = default;
		ICnpServer(ICnpServer&&) = default;

		virtual ~ICnpServer() = default;

		virtual void initializeServer() = 0;

		virtual void startServer() = 0;
		
		virtual void stopServer() = 0;

		virtual void handleRequests() = 0;

		ICnpServer& operator =(const ICnpServer&) = default;
		ICnpServer& operator =(ICnpServer&&) = default;
	};
}