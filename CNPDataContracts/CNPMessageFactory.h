#pragma once

#include <string>

#include "CNPDataContracts.h"

namespace cnp::contracts::impl
{
	class CNPMessageFactory : public ICNPMessageFactory
	{
	public:
		CNPMessageFactory() = default;
		CNPMessageFactory(const CNPMessageFactory&) = default;
		CNPMessageFactory(CNPMessageFactory&&) = default;

		~CNPMessageFactory() override = default;

		std::shared_ptr<ICNPRequest> createRequest(CNPVersion version, std::string& command) override;

		std::shared_ptr<ICNPRequest> createRequest(CNPVersion version, std::string& command, std::string& data) override;

		std::shared_ptr<ICNPResponse> createResponse(CNPVersion version, CNPStatus status, std::string& command) override;

		std::shared_ptr<ICNPResponse> createResponse(CNPVersion version, CNPStatus status, std::string& command, std::string& data) override;

		CNPMessageFactory& operator=(const CNPMessageFactory&) = default;
		CNPMessageFactory& operator=(CNPMessageFactory&&) = default;
	};
}
