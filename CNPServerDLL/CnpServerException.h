#pragma once

#include <exception>
#include <string>

namespace cnp::server
{
	class CnpServerException : std::exception
	{
	private:
		std::string _message;

	public:
		template<typename TMessage>
		explicit CnpServerException(TMessage&& message, int32_t errorCode)
		{
			char errorBuffer[1024] = { 0 };
			strerror_s<1024>(errorBuffer, errorCode);

			std::stringstream stringStream;
			stringStream << message << " : " << errorBuffer;

			_message = stringStream.str();
		}

		template<typename TMessage>
		explicit CnpServerException(TMessage&& message)
			: _message(std::forward<std::string>(message))
		{
		}

		const char *what() const noexcept override;
	};
}
