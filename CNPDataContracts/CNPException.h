#pragma once

#include <string>
#include <sstream>

namespace cnp::contracts
{
	class CNPException : public std::exception
	{
	private:
		std::string _message;

	public:
		template<typename TMessage>
		explicit CNPException(TMessage&& message)
			: _message(std::forward<std::string>(message))
		{
		}

		template<typename TMessage>
		explicit CNPException(TMessage&& message, int32_t errorCode)
		{
			char errorBuffer[1024] = { 0 };
			strerror_s<1024>(errorBuffer, errorCode);

			std::stringstream stringStream;
			stringStream << std::forward<std::string>(message) << " : " << errorBuffer;

			_message = stringStream.str();
		}

		const char *what() const noexcept override;
	};
}
