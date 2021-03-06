// ClientDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include <memory>

#include "CNPClientDLL.h"
#include "CnpClientFactory.h"

std::shared_ptr<cnp::client::ICnpClientFactory> createFactory()
{
	return std::move(std::make_shared<cnp::client::impl::CnpClientFactory>());
}
