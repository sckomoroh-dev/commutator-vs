#pragma once

#include "CNPDataContracts.h"

namespace cnp::contracts::impl
{
	class CNPMessage : public ICNPMessage
	{
	public:
		CNPMessage() = default;
		CNPMessage(const CNPMessage& copy) = default;
		CNPMessage(CNPMessage&& move) = default;

		virtual ~CNPMessage() = default;

		CNPMessage& operator=(const CNPMessage& copy) = default;
		CNPMessage& operator=(CNPMessage&& copy) = default;

	protected:
		static std::string versionToString(CNPVersion version);
	};
}
