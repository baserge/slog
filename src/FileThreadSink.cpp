// FileThreadSink.cpp - FileThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#include "FileThreadSink.h"
#include <sstream>
#ifdef HAVE_KLUBOK
using namespace slog;
using namespace std;

// =========================================================================
/// @brief Opens the file with the given name for logging.
// =========================================================================
FileThreadSink::FileThreadSink(const string &fileName) : file(fileName.c_str()),
    fileName(fileName)
{
    if (file.fail())
    {
        ostringstream strm;
        strm<<"Failed to open the file for logging output: "
            <<fileName<<endl;
        throw invalid_argument(strm.str().c_str());
    }
}

ostream &FileThreadSink::lockStream()
{
    mutex.lock();
    return file;
}
void FileThreadSink::releaseStream()
{
    mutex.unlock();
}
#endif
