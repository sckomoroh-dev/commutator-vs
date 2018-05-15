#pragma once

#include "CNPDataContracts.h"
#include "CNPMessage.h"

namespace cnp::contracts::impl
{
	class CNPMessageParser;

	class CNPRequest
		: public ICNPRequest
		, public CNPMessage
	{
	private:
		CNPVersion _version { CNPVersion::Unknown };
		std::string _command;
		std::string _data;

	public:
		CNPRequest() = default;
		CNPRequest(const CNPRequest& copy) = default;
		CNPRequest(CNPRequest&& move) = default;

		template<class TCommand>
		CNPRequest(CNPVersion version, TCommand&& command)
			: _version(version)
			, _command(std::forward<std::string>(command))
		{
		}

		template<class TCommand, class  TData>
		CNPRequest(CNPVersion version, TCommand&& command, TData&& data)
			: _version(version)
			, _command(std::forward<std::string>(command))
			, _data(std::forward<std::string>(data))
		{
		}

		~CNPRequest() override = default;

		CNPRequest& operator=(const CNPRequest& copy) = default;
		CNPRequest& operator=(CNPRequest&& copy) = default;

	public:
		CNPVersion version() const noexcept override;

		std::string command() const noexcept override;

		std::string data() const noexcept override;

		std::string toString() const override;

		friend class CNPMessageParser;
	};
}
