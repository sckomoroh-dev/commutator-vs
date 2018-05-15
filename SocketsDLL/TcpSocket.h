//
// Created by sckomoroh on 4/25/18.
//

#pragma once

#include <cstdint>
#include "Socket.h"

namespace sockets::impl::tcp
{
	class TcpSocket : public Socket
	{
	protected:
		TcpSocket(const char* serverIp, int32_t port);

		TcpSocket() = default;
	};
}
