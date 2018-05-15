#include "stdafx.h"

#include <sstream>

#include "CNPResponse.h"
#include "CNPException.h"

using namespace cnp::contracts::impl;
using namespace cnp::contracts;

CNPVersion CNPResponse::version() const noexcept
{
	return _version;
}

std::string CNPResponse::command() const noexcept
{
	return _command;
}

std::string CNPResponse::data() const noexcept
{
	return _data;
}

CNPStatus CNPResponse::status() const noexcept
{
	return _status;
}

std::string CNPResponse::toString() const
{
	std::ostringstream stringStream;
	stringStream << "CNP v" << versionToString(_version) << std::endl;
	stringStream << "Command: " << _command << std::endl;
	stringStream << "Status: " << statusToString(_status) << std::endl;

	if (!_data.empty())
	{
		stringStream << "Data: " << _data << std::endl;
	}

	stringStream << std::endl;

	return stringStream.str();
}

std::string CNPResponse::statusToString(CNPStatus status)
{
	switch (status)
	{
	case CNPStatus::StatusError:
		return "ERROR";

	case CNPStatus::StatusOk:
		return "OK";

	default:
		break;
	}

	throw CNPException("Invalid status");
}
