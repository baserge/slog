// SharedFileThreadSink.h - SharedFileThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef SHAREDFILETHREADSINK_H_J26GGQKH
#define SHAREDFILETHREADSINK_H_J26GGQKH
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
    using klubok::Mutex;
    // =========================================================================
    /// @brief Thread safe file logger into a shared file.
    // =========================================================================
    class SharedFileThreadSink : public Sink
    {
        public:
            SharedFileThreadSink (const string &fileName, Mutex &mutex,
                                  long int sizeLimit = 1024*1024*1024);
            virtual ~SharedFileThreadSink ();

        protected:
            virtual ostream &lockStream();
            virtual void releaseStream();
    
        private:
            string fileName;
            ofstream file;
            Mutex &mutex;
            size_t sizeLimit;
            void rollOver();
            void checkFail();
    };
} //namespace slog
#endif
#endif /* end of include guard: SHAREDFILETHREADSINK_H_J26GGQKH */
