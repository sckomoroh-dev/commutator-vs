#pragma once

#include <map>

#include "CNPDataContracts.h"
#include <functional>

namespace cnp::contracts::impl
{
	class CNPMessageParser : public ICNPMessageParser
	{
	public:
		CNPMessageParser() = default;
		CNPMessageParser(const CNPMessageParser&) = default;
		CNPMessageParser(CNPMessageParser&&) = default;

		~CNPMessageParser() override = default;
	
		std::shared_ptr<ICNPRequest> requestFromString(const std::string& sourceString) override;

		std::shared_ptr<ICNPRequest> requestFromString(std::string&& sourceString) override;

		std::shared_ptr<ICNPResponse> responseFromString(const std::string& sourceString) override;

		std::shared_ptr<ICNPResponse> responseFromString(std::string&& sourceString) override;

		CNPMessageParser& operator=(const CNPMessageParser&) = default;
		CNPMessageParser& operator=(CNPMessageParser&&) = default;

	private:
		static CNPVersion stringToVersion(const std::string &strVersion);

		static std::map<std::string, std::string> requestToMap(const std::string &request);
	
		static std::list<std::string> splitMessageString(const std::string &inputString,
			char delimiter,
			bool single,
			std::function<std::string(std::string)> itemProcessingFunc);

		static std::string getMessageVersion(const std::string &requestString);

		static std::pair<std::string, std::string> parseMessageItem(const std::string &requestField);;

		template<typename TStatusString>
		static CNPStatus stringToStatus(TStatusString &&statusString)
		{
			const std::string statusStr(std::forward<std::string>(statusString));
			if (statusStr == "OK")
			{
				return CNPStatus::StatusOk;
			}

			if (statusStr == "ERROR")
			{
				return CNPStatus::StatusError;
			}

			throw CNPException("Unknown status");
		}
	};
}
