// CoutThreadSink.cpp - CoutThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#include "CoutThreadSink.h"
#ifdef HAVE_KLUBOK
using namespace slog;

klubok::Mutex CoutThreadSink::mutex = klubok::Mutex();

ostream &CoutThreadSink::lockStream()
{
    mutex.lock();
    return cout;
}
void CoutThreadSink::releaseStream()
{
    mutex.unlock();
}
#endif
