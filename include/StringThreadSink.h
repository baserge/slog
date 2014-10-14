// StringThreadSink.h - StringThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef STRINGTHREADSINK_H_J26GGQKH
#define STRINGTHREADSINK_H_J26GGQKH
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
    // =========================================================================
    /// @brief Thread safe logger into a string buffer.
    // =========================================================================
    class StringThreadSink : public Sink
    {
        public:
            ostringstream &getBuffer() {return buffer;};
            string getString() const {return buffer.str();};
            virtual Sink *clone() const {return new StringThreadSink;};

        protected:
            virtual ostream &lockStream();
            virtual void releaseStream();
    
        private:
            ostringstream buffer;
            klubok::Mutex mutex;
    };
} //namespace slog
#endif
#endif /* end of include guard: STRINGTHREADSINK_H_J26GGQKH */
