// Header.cpp - Header class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.

#include "Header.h"
#include <thread>
#include <ctime>
using namespace std;

namespace slog
{
    std::ostream& operator<<(std::ostream& strm, const Header& inst)
    {
        if (!inst.logLevel.empty())
            strm<<"["<<inst.logLevel<<"]";
        if (inst.writeThread)
            strm<<"[TH"<<std::hex<<std::this_thread::get_id()<<"]"<<std::dec;
        if (inst.writeTime)
        {
            time_t now = time(0);
            char buffer[20];
            strftime (buffer, 20, "%Y-%m-%d %H:%M:%S", gmtime(&now));
            strm << "[" << buffer<<"]";
        }
        if (!inst.logName.empty())
            strm<<"["<<inst.logName<<"]";
        strm<<" ";
        return strm;
    }
} //namespace slog
