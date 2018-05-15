#include "stdafx.h"

#include "CNPException.h"

using namespace cnp::contracts;

const char *CNPException::what() const noexcept
{
	return _message.c_str();
}
