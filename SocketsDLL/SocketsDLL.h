#pragma once

#include <cstdint>
#include <memory>

#ifdef EXPORT_SYMBOLS
#define SYMBOL_ATTRIBUTE	__declspec(dllexport)
#else
#define SYMBOL_ATTRIBUTE	__declspec(dllimport)
#endif

namespace sockets
{
	class SocketException;

	class ISocket
	{
	public:
		ISocket(const ISocket& copy) = delete;

		ISocket(ISocket&& move) = default;

		virtual ~ISocket() = default;

		virtual void closeSocket() = 0;

		virtual unsigned long socketOptions(int64_t command, unsigned long option) = 0;

		virtual bool readyForRead() = 0;

		virtual bool readyForWrite() = 0;

		virtual bool readyForRead(int timeout) = 0;

		virtual bool readyForWrite(int timeout) = 0;

		ISocket& operator=(ISocket&) = delete;

		ISocket& operator=(ISocket&&) = default;

	protected:
		ISocket() = default;
	};

	namespace tcp
	{
		class ITcpClientSocket : public virtual ISocket
		{
		public:
			ITcpClientSocket(const ITcpClientSocket& copy) = delete;

			ITcpClientSocket(ITcpClientSocket&& move) = default;

			virtual ~ITcpClientSocket() = default;

			virtual void connect() = 0;

			virtual void sendBuffer(char *buffer, size_t bufferSize) const = 0;

			virtual void readBuffer(char *buffer, size_t bufferSize) const = 0;

			ITcpClientSocket& operator=(ITcpClientSocket&) = delete;

			ITcpClientSocket& operator=(ITcpClientSocket&&) = default;

		protected:
			ITcpClientSocket() = default;
		};

		using ITcpClientSocketUPtr = std::unique_ptr<ITcpClientSocket>;

		class ITcpServerSocket : public virtual ISocket
		{
		public:
			ITcpServerSocket(const ITcpServerSocket& copy) = delete;

			ITcpServerSocket(ITcpServerSocket&& move) = default;

			virtual ~ITcpServerSocket() = default;

			virtual void bind() = 0;

			virtual void listen() const = 0;

			virtual ITcpClientSocketUPtr accept() = 0;

			ITcpServerSocket& operator=(ITcpServerSocket&) = delete;

			ITcpServerSocket& operator=(ITcpServerSocket&&) = default;

		protected:
			ITcpServerSocket() = default;
		};
	}

	namespace udp
	{
		class IUdpClientSocket : public virtual ISocket
		{
		public:
			IUdpClientSocket(const IUdpClientSocket& copy) = delete;

			IUdpClientSocket(IUdpClientSocket&& move) = default;

			virtual ~IUdpClientSocket() = default;

			virtual void sendBuffer(char *buffer, size_t bufferSize) = 0;

			virtual void readBuffer(char *buffer, size_t bufferSize) = 0;

			IUdpClientSocket& operator=(IUdpClientSocket&) = delete;

			IUdpClientSocket& operator=(IUdpClientSocket&&) = default;

		protected:
			IUdpClientSocket() = default;
		};

		class IUdpServerSocket : public virtual ISocket
		{
		public:
			IUdpServerSocket(const IUdpServerSocket& copy) = delete;

			IUdpServerSocket(IUdpServerSocket&& move) = default;

			virtual ~IUdpServerSocket() = default;

			virtual void bind() = 0;

			virtual void sendBuffer(char *buffer, size_t bufferSize, struct sockaddr_in& targetAddress) const = 0;

			virtual void readBuffer(char *buffer, size_t bufferSize, struct sockaddr_in* clientSocketAddress) const = 0;

			IUdpServerSocket& operator=(IUdpServerSocket&) = delete;

			IUdpServerSocket& operator=(IUdpServerSocket&&) = default;

		protected:
			IUdpServerSocket() = default;
		};
	}

	class ITcpSocketFactory
	{
	public:
		virtual std::unique_ptr<tcp::ITcpClientSocket> createClientSocket(const char* serverIp, int32_t port) = 0;

		virtual std::unique_ptr<tcp::ITcpServerSocket> createServerSocket(const char* serverIp, int32_t port) = 0;
	};

	class IUdpSocketFactory
	{
	public:
		virtual std::unique_ptr<udp::IUdpClientSocket> createClientSocket(const char* serverIp, int32_t port) = 0;

		virtual std::unique_ptr<udp::IUdpServerSocket> createServerSocket(const char* serverIp, int32_t port) = 0;
	};
}

SYMBOL_ATTRIBUTE
void initWinSocks();

SYMBOL_ATTRIBUTE
void cleanupWinSock();

SYMBOL_ATTRIBUTE
std::shared_ptr<sockets::ITcpSocketFactory> getTcpSocketFactory();

SYMBOL_ATTRIBUTE
std::shared_ptr<sockets::IUdpSocketFactory> getUdpSocketFactory();