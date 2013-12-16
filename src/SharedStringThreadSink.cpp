// SharedStringThreadSink.cpp - SharedStringThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#include "SharedStringThreadSink.h"
#ifdef HAVE_KLUBOK
using namespace slog;
SharedStringThreadSink::SharedStringThreadSink(ostringstream &buffer,
                                               Mutex &mutex) :
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
#endif  /* end of HAVE_KLUBOK */
