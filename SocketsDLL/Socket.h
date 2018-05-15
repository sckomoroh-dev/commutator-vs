//
// Created by sckomoroh on 4/25/18.
//

#pragma once

#include <WinSock2.h>

#include <cstdint>
#include <memory>
#include <string>

#include "SocketsDLL.h"

namespace sockets::impl
{
    class Socket : public virtual ISocket
    {
    protected:
		SOCKET _socket;
        struct sockaddr_in _targetSocketAddress;
		std::string _serverIp;
		int32_t _port;
		struct pollfd _pollRead;
		struct pollfd _pollWrite;

    public:
        void closeSocket() override;
		
		unsigned long socketOptions(int64_t command, unsigned long option) override;

    	bool readyForRead() override;

		bool readyForWrite() override;

		bool readyForRead(int timeout) override;

		bool readyForWrite(int timeout) override;

    protected:
        Socket();

        Socket(const char* serverIp, int32_t port);

		~Socket() override;

    protected:
        template<typename TTargetSocketType>
        std::unique_ptr<TTargetSocketType> createSocket(SOCKET& socketDescriptor, const struct sockaddr_in& socketAddress)
        {
            auto targetSocket = std::make_unique<TTargetSocketType>();
            targetSocket->_socket = socketDescriptor;
            targetSocket->_targetSocketAddress = socketAddress;

            return targetSocket;
        }
    };
}
