//
// Created by sckomoroh on 4/24/18.
//

#pragma once

#include <exception>
#include <sstream>
#include <string>

namespace sockets
{
    class SocketException : public std::exception
    {
    private:
        std::string _message;

    public:
		SocketException(const SocketException&) = default;
		SocketException(SocketException&&) = default;

		template<typename TMessage>
        explicit SocketException(TMessage&& message, int32_t errorCode)
		{
			char errorBuffer[1024] = { 0 };
			strerror_s<1024>(errorBuffer, errorCode);

			std::stringstream stringStream;
			stringStream << message << " : " << errorBuffer;

			_message = stringStream.str();
		}

		template<typename TMessage>
		explicit SocketException(TMessage&& message)
			: _message(std::forward<std::string>(message))
		{
		}

        const char *what() const noexcept override;

		SocketException& operator =(const SocketException&) = default;
		SocketException& operator =(SocketException&&) = default;
	};
}
