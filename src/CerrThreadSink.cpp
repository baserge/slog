// CerrThreadSink.cpp - CerrThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#include "CerrThreadSink.h"
using namespace slog;

std::mutex CerrThreadSink::mutex;

ostream &CerrThreadSink::lockStream()
{
    mutex.lock();
    return cerr;
}
void CerrThreadSink::releaseStream()
{
    mutex.unlock();
}
