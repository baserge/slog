// SharedStringThreadSink.cpp - SharedStringThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#include "SharedStringThreadSink.h"
using namespace slog;
SharedStringThreadSink::SharedStringThreadSink(ostringstream &buffer,
                                               std::mutex &mutex) :
    buffer(buffer), mutex(mutex)
{}

ostream &SharedStringThreadSink::lockStream()
{
    mutex.lock();
    return buffer;
}

void SharedStringThreadSink::releaseStream()
{
    mutex.unlock();
}
