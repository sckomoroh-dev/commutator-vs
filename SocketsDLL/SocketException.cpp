//
// Created by sckomoroh on 05.05.18.
//
#include "stdafx.h"

#include "SocketException.h"

using namespace sockets;

const char *SocketException::what() const noexcept
{
    return _message.c_str();
}
