// SharedStringThreadSink.h - SharedStringThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef SHAREDSTRINGTHREADSINK_H_HBOURB8S
#define SHAREDSTRINGTHREADSINK_H_HBOURB8S
#include "config.h"
#ifdef HAVE_KLUBOK
#include "Sink.h"
#include <Klubok/thread.h>
#include <sstream>
#include <string>
namespace slog
{
    using std::ostringstream;
    using std::string;
    using klubok::Mutex;
    // =========================================================================
    /// @brief String buffer - messages into common thread safe buffer.
    // =========================================================================
    class SharedStringThreadSink : public Sink
    {
        public:
            SharedStringThreadSink(ostringstream &buffer, Mutex &mutex);
            const ostringstream &getBuffer() const {return buffer;};
            string getString() const {return buffer.str();};
    
        protected:
            virtual ostream &lockStream();
            virtual void releaseStream();

        private:
            ostringstream &buffer;
            Mutex &mutex;
    };
} //namespace slog
#endif
#endif /* end of include guard: SHAREDSTRINGTHREADSINK_H_HBOURB8S */
