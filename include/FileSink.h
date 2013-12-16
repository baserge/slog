// FileSink.h - FileSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef FILESINK_H_2HKGDFLO
#define FILESINK_H_2HKGDFLO
#include "Sink.h"
#include <fstream> 
#include <string>
namespace slog
{
    using std::string;
    using std::ofstream;
    // =========================================================================
    /// @brief File sink - saves messages to a file.
    // =========================================================================
    class FileSink : public Sink
    {
        public:
            FileSink (const string &fileName, bool append = true);
            virtual ~FileSink () {file.close();};

        protected:
            virtual ostream &lockStream() {return file;};
            virtual void releaseStream() {};
    
        private:
            ofstream file;
    };
} //namespace slog

#endif /* end of include guard: FILESINK_H_2HKGDFLO */
