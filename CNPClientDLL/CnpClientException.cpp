#include "stdafx.h"
#include "CnpClientException.h"

using namespace cnp::client;

const char *CnpClientException::what() const noexcept
{
	return _message.c_str();
}
