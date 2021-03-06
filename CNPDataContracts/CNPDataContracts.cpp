// CNPDataContracts.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include <memory>

#include "CNPDataContracts.h"
#include "CNPMessageParser.h"
#include "CNPMessageFactory.h"

std::shared_ptr<cnp::contracts::ICNPMessageParser> createMessageParser()
{
	return std::move(std::make_shared<cnp::contracts::impl::CNPMessageParser>());
}

std::shared_ptr<cnp::contracts::ICNPMessageFactory> createMessageFactory()
{
	return std::move(std::make_shared<cnp::contracts::impl::CNPMessageFactory>());
}
