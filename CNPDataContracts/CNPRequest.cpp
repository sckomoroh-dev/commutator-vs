#include "stdafx.h"

#include "CNPRequest.h"

#include <sstream>

using namespace cnp::contracts::impl;
using namespace cnp::contracts;

CNPVersion CNPRequest::version() const noexcept
{
	return _version;
}

std::string CNPRequest::command() const noexcept
{
	return _command;
}

std::string CNPRequest::data() const noexcept
{
	return _data;
}

std::string CNPRequest::toString() const
{
	std::ostringstream stringStream;
	stringStream << "CNP v" << versionToString(_version) << std::endl;
	stringStream << "Command: " << _command << std::endl;

	if (!_data.empty())
	{
		stringStream << "Data: " << _data << std::endl;
	}

	stringStream << std::endl;

	return stringStream.str();
}
