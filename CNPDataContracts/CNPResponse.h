#pragma once

#include "CNPDataContracts.h"
#include "CNPMessage.h"

namespace cnp::contracts::impl
{
	class CNPMessageParser;

	class CNPResponse 
		: public ICNPResponse
		, public CNPMessage
	{
	private:
		CNPVersion _version { CNPVersion::Unknown };
		std::string _command;
		std::string _data;
		CNPStatus _status { CNPStatus::Unknown };

	public:
		CNPResponse() = default;
		CNPResponse(const CNPResponse& copy) = default;
		CNPResponse(CNPResponse&& move) = default;

		template<class TCommand>
		CNPResponse(CNPVersion version, CNPStatus status, TCommand&& command)
			: _version(version)
			, _command(std::forward<std::string>(command))
			, _status(status)
		{
		}

		template<class TCommand, class TData>
		CNPResponse(CNPVersion version, CNPStatus status, TCommand&& command, TData&& data)
			: _version(version)
			, _command(std::forward<std::string>(command))
			, _data(data)
			, _status(status)
		{
		}

		~CNPResponse() override = default;
	
		CNPVersion version() const noexcept override;

		std::string command() const noexcept override;

		std::string data() const noexcept override;

		CNPStatus status() const noexcept override;

		std::string toString() const final override;

		CNPResponse& operator=(const CNPResponse& copy) = default;
		CNPResponse& operator=(CNPResponse&& copy) = default;

	private:
		static std::string statusToString(CNPStatus status);

		friend class CNPMessageParser;
	};
}
