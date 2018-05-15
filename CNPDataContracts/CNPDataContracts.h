#pragma once

#include <memory>
#include <string>

#ifdef EXPORT_SYMBOLS
#define SYMBOL_ATTRIBUTE	__declspec(dllexport)
#else
#define SYMBOL_ATTRIBUTE	__declspec(dllimport)
#endif

namespace cnp::contracts
{
	class CNPException;

	class ICNPMessage
	{
	public:
		virtual ~ICNPMessage() = default;

		virtual std::string toString() const = 0;
	};

	enum class CNPStatus
	{
		Unknown,
		StatusOk,
		StatusError
	};

	enum class CNPVersion
	{
		Unknown,
		Version10
	};

	class ICNPRequest : public ICNPMessage
	{
	public:
		virtual CNPVersion version() const noexcept = 0;

		virtual std::string command() const noexcept = 0;

		virtual std::string data() const noexcept = 0;
	};

	class ICNPResponse : public ICNPMessage
	{
	public:
		virtual ~ICNPResponse() = default;

		virtual CNPVersion version() const noexcept = 0;

		virtual std::string command() const noexcept = 0;

		virtual std::string data() const noexcept = 0;

		virtual CNPStatus status() const noexcept = 0;
	};

	class ICNPMessageParser
	{
	public:
		ICNPMessageParser() = default;
		ICNPMessageParser(const ICNPMessageParser&) = default;
		ICNPMessageParser(ICNPMessageParser&&) = default;

		virtual ~ICNPMessageParser() = default;

		virtual std::shared_ptr<ICNPRequest> requestFromString(const std::string& sourceString) = 0;

		virtual std::shared_ptr<ICNPRequest> requestFromString(std::string&& sourceString) = 0;

		virtual std::shared_ptr<ICNPResponse> responseFromString(const std::string& sourceString) = 0;

		virtual std::shared_ptr<ICNPResponse> responseFromString(std::string&& sourceString) = 0;

		ICNPMessageParser& operator=(const ICNPMessageParser&) = default;
		ICNPMessageParser& operator=(ICNPMessageParser&&) = default;
	};

	class ICNPMessageFactory
	{
	public:
		ICNPMessageFactory() = default;
		ICNPMessageFactory(const ICNPMessageFactory&) = default;
		ICNPMessageFactory(ICNPMessageFactory&&) = default;

		virtual ~ICNPMessageFactory() = default;

		virtual std::shared_ptr<ICNPRequest> createRequest(CNPVersion version, std::string& commnad) = 0;

		virtual std::shared_ptr<ICNPRequest> createRequest(CNPVersion version, std::string& commnad, std::string& data) = 0;

		virtual std::shared_ptr<ICNPResponse> createResponse(CNPVersion version, CNPStatus status, std::string& commnad) = 0;

		virtual std::shared_ptr<ICNPResponse> createResponse(CNPVersion version, CNPStatus status, std::string& commnad, std::string& data) = 0;

		ICNPMessageFactory& operator=(const ICNPMessageFactory&) = default;
		ICNPMessageFactory& operator=(ICNPMessageFactory&&) = default;
	};
}

SYMBOL_ATTRIBUTE
std::shared_ptr<cnp::contracts::ICNPMessageParser> createMessageParser();

SYMBOL_ATTRIBUTE
std::shared_ptr<cnp::contracts::ICNPMessageFactory> createMessageFactory();
