// ServerDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CnpServerFactory.h"

std::shared_ptr<cnp::server::ICnpServerFactory> getServerFactory()
{
	return std::move(std::make_shared<cnp::server::CnpServerFactory>());
}
