// FileSink.cpp - FileSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#include "FileSink.h" 
#include <sstream>
#include <stdexcept>
using namespace slog;
using namespace std;

// =========================================================================
/// @brief Opens the file with the given name for logging.
// =========================================================================
FileSink::FileSink(const string &fileName, bool append)
    : fileName(fileName), append(append)
{
    if (append)
        file.open(fileName.c_str(), ios::app);
    else
        file.open(fileName.c_str());
    if (file.fail())
    {
        ostringstream strm;
        strm<<"Failed to open the file for logging output: "
            <<fileName<<endl;
        throw invalid_argument(strm.str().c_str());
    }
}
