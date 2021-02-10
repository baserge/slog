// SharedFileThreadSink.cpp - SharedFileThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#include "SharedFileThreadSink.h"
#include <sstream>
using namespace slog;
using namespace std;

// =========================================================================
/// @brief Opens the file with the given name for logging.
///
/// Mutex common for all sinks using the same file is to be supplied
/// as well. File is opened in append mode. Also supports size checking and
/// rolling over the log if the limit is exceeded.
///
/// @param fileName File to use.
/// @param mutex Common mutex for all sinks using the same file.
/// @param sizeLimit Limit on the file size in bytes. If the limit is
///                  reached the file is truncated.
// =========================================================================
SharedFileThreadSink::SharedFileThreadSink(const string &fileName,
                                          std::mutex &mutex,
                                          long int sizeLimit)
    :  fileName(fileName), mutex(mutex), sizeLimit(sizeLimit)
{
    file.open(fileName.c_str(), ios::app);
    checkFail();
}

SharedFileThreadSink::~SharedFileThreadSink()
{
    file.close();
}

ostream &SharedFileThreadSink::lockStream()
{
    mutex.lock();
    rollOver();
    return file;
}
void SharedFileThreadSink::releaseStream()
{
    mutex.unlock();
}

// =========================================================================
/// @brief Start writing file from the beginning if the size is exceeded.
// =========================================================================
void SharedFileThreadSink::rollOver()
{
    if (file.tellp() > sizeLimit)
    {
        // iostream mess, there is not clean way of truncating files?
        file.close();
        file.open(fileName.c_str(), ios::trunc);
        file.close();
        file.open(fileName.c_str(), ios::app );
        checkFail();
    }
}

// =========================================================================
/// @brief Check that file was openned properly
// =========================================================================
void SharedFileThreadSink::checkFail()
{
    if (file.fail())
    {
        ostringstream strm;
        strm<<"Failed to open the file for logging output: "
            <<fileName<<endl;
        throw invalid_argument(strm.str().c_str());
    }
}
