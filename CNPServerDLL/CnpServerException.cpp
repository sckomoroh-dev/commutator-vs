#include "stdafx.h"
#include "CnpServerException.h"

using namespace cnp::server;

const char *CnpServerException::what() const noexcept
{
	return _message.c_str();
}
