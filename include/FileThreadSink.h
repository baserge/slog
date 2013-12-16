// FileThreadSink.h - FileThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef FILETHREADSINK_H_J26GGQKH
#define FILETHREADSINK_H_J26GGQKH
#include "config.h"
#ifdef HAVE_KLUBOK
#include "Sink.h"
#include <Klubok/thread.h>
#include <fstream> 
#include <string>
namespace slog
{
    using std::string;
    using std::ofstream;
    // =========================================================================
    /// @brief Thread safe file logger.
    // =========================================================================
    class FileThreadSink : public Sink
    {
        public:
            FileThreadSink (const string &fileName);
            virtual ~FileThreadSink () {file.close();};

        protected:
            virtual ostream &lockStream();
            virtual void releaseStream();
    
        private:
            ofstream file;
            klubok::Mutex mutex;
    };
} //namespace slog
#endif
#endif /* end of include guard: FILETHREADSINK_H_J26GGQKH */
