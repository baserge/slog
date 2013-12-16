// StringThreadSink.cpp - StringThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#include "StringThreadSink.h"
#ifdef HAVE_KLUBOK
using namespace slog;

ostream &StringThreadSink::lockStream()
{
    mutex.lock();
    return buffer;
}
void StringThreadSink::releaseStream()
{
    mutex.unlock();
}
#endif
