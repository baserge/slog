// StreamBuffer.cpp - StreamBuffer implementation, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.

#include "StreamBuffer.h"
#include <ctime>
using namespace slog;

// =========================================================================
/// @brief Get UTC time stamp. Standard ascrime(gmtime()) with removed \n.
// =========================================================================
void StreamBuffer::writeTimeStamp(ostringstream *strm)
{
    if (!strm)
        return;
    time_t now = time(0);
    char buffer[20];
    strftime (buffer, 20, "%Y-%m-%d %H:%M:%S", gmtime(&now));
    *strm << "[" << buffer<<"]";
}
