#include "stdafx.h"
#include "CNPMessageParser.h"
#include "CNPException.h"
#include "CNPRequest.h"
#include "CNPResponse.h"

using namespace cnp::contracts::impl;
using namespace cnp::contracts;

std::shared_ptr<ICNPRequest> CNPMessageParser::requestFromString(const std::string & sourceString)
{
	return requestFromString(std::move(sourceString));
}

std::shared_ptr<ICNPRequest> CNPMessageParser::requestFromString(std::string && sourceString)
{
	auto mapping = requestToMap(sourceString);
	if (mapping.find("Version") == mapping.end())
	{
		throw CNPException("Protocol version was not specified");
	}

	if (mapping.find("Command") == mapping.end())
	{
		throw CNPException("A command was not specified");
	}

	auto request = std::make_shared<CNPRequest>();

	request->_version = stringToVersion(mapping["Version"]);
	request->_command = mapping["Command"];

	if (mapping.find("Data") != mapping.end())
	{
		request->_data = mapping["Data"];
	}

	return request;
}

std::shared_ptr<ICNPResponse> CNPMessageParser::responseFromString(const std::string & sourceString)
{
	return responseFromString(std::move(sourceString));
}

std::shared_ptr<ICNPResponse> CNPMessageParser::responseFromString(std::string && sourceString)
{
	auto mapping = requestToMap(std::forward<std::string>(sourceString));
	if (mapping.find("Version") == mapping.end())
	{
		throw CNPException("Protocol version was not specified");
	}

	if (mapping.find("Command") == mapping.end())
	{
		throw CNPException("A command was not specified");
	}

	if (mapping.find("Status") == mapping.end())
	{
		throw CNPException("A status was not specified");
	}

	auto request = std::make_shared<CNPResponse>();
	request->_version = stringToVersion(mapping["Version"]);
	request->_command = mapping["Command"];
	request->_status = stringToStatus(mapping["Status"]);

	if (mapping.find("Data") != mapping.end())
	{
		request->_data = mapping["Data"];
	}

	return request;
}

CNPVersion CNPMessageParser::stringToVersion(const std::string &strVersion)
{
	if (strVersion == "1.0")
	{
		return ::cnp::contracts::CNPVersion::Version10;
	}

	throw CNPException("Invalid CNP version");
}

std::map<std::string, std::string> CNPMessageParser::requestToMap(const std::string &request)
{
	std::map<std::string, std::string> result;
	auto requestFields = splitMessageString(request, '\n', false, [](std::string item) { return item; });

	for (const auto &requestField : requestFields)
	{
		if (requestField.substr(0, 3) == "CNP")
		{
			auto messageVersion = getMessageVersion(requestField);
			if (!messageVersion.empty())
			{
				result.insert(std::make_pair("Version", messageVersion));
			}

			continue;
		}

		auto keyValue = parseMessageItem(requestField);

		result.insert(keyValue);
	}

	return result;
}

std::list<std::string> CNPMessageParser::splitMessageString(const std::string &inputString,
	char delimiter,
	bool single,
	std::function<std::string(std::string)> itemProcessingFunc)
{
	std::list<std::string> result;
	auto pos = inputString.find(delimiter, 0);
	decltype(pos) nextPos = 0;

	do
	{
		if (pos != std::string::npos)
		{
			auto requestItem = inputString.substr(nextPos, pos - nextPos);
			requestItem = itemProcessingFunc(requestItem);
			if (!requestItem.empty())
			{
				result.push_back(requestItem);
			}

			nextPos = pos + 1;
		}

		pos = inputString.find(delimiter, nextPos);
	} while (pos != std::string::npos && !single);

	auto requestItem = inputString.substr(nextPos);
	requestItem = itemProcessingFunc(requestItem);
	if (!requestItem.empty())
	{
		result.push_back(requestItem);
	}

	return result;
}

std::string CNPMessageParser::getMessageVersion(const std::string &requestString)
{
	const auto separateIndex = requestString.find(' ');
	if (separateIndex == std::string::npos)
	{
		return "";
	}

	auto versionString = requestString.substr(separateIndex + 1);
	if (versionString[0] != 'v')
	{
		return "";
	}

	return versionString.substr(1);
}

std::pair<std::string, std::string> CNPMessageParser::parseMessageItem(const std::string &requestField)
{
	auto requestKeyValue = splitMessageString(requestField, ':', true, [](std::string item)
	{
		// Trim spaces on start and end of the string

		auto start = 0;
		while (item[start] == ' ')
		{
			start++;
		}

		auto end = item.length();
		while (item[end - 1] == ' ')
		{
			end--;
		}

		return item.substr(start, end - start);
	});

	auto key = *requestKeyValue.begin();
	auto value = *(++requestKeyValue.begin());

	return std::make_pair<std::string, std::string>(std::move(key), std::move(value));
}
