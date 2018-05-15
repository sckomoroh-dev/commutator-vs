#include "stdafx.h"

#include "CNPDataContracts.h"
#include "CNPMessage.h"
#include "CNPException.h"

using namespace cnp::contracts::impl;
using namespace cnp::contracts;

std::string CNPMessage::versionToString(CNPVersion version)
{
	switch (version)
	{
	case CNPVersion::Version10:
		return "1.0";
	default:
		break;
	}

	throw CNPException("Invalid CNP version");
}
