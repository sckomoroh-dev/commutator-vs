#include "stdafx.h"

#include "CNPDataContracts.h"

#include "CNPMessageFactory.h"
#include "CNPRequest.h"
#include "CNPResponse.h"

using namespace cnp::contracts::impl;
using namespace cnp::contracts;

std::shared_ptr<cnp::contracts::ICNPRequest> CNPMessageFactory::createRequest(CNPVersion version, std::string & command)
{
	return std::move(std::make_shared<CNPRequest>(version, command));
}

std::shared_ptr<cnp::contracts::ICNPRequest> CNPMessageFactory::createRequest(CNPVersion version, std::string & command, std::string & data)
{
	return std::move(std::make_shared<CNPRequest>(version, command, data));
}

std::shared_ptr<cnp::contracts::ICNPResponse> CNPMessageFactory::createResponse(CNPVersion version, CNPStatus status, std::string & command)
{
	return std::move(std::make_shared<CNPResponse>(version, status, command));
}

std::shared_ptr<cnp::contracts::ICNPResponse> CNPMessageFactory::createResponse(CNPVersion version, CNPStatus status, std::string & command, std::string & data)
{
	return std::move(std::make_shared<CNPResponse>(version, status, command, data));
}
