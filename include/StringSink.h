// StringSink.h - StringSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef STRINGSINK_H_HBOURB8S
#define STRINGSINK_H_HBOURB8S
#include "Sink.h"
#include <sstream>
#include <string>
namespace slog
{
    using std::ostringstream;
    using std::string;
    // =========================================================================
    /// @brief String buffer - saves messages into ostringstream buffer.
    // =========================================================================
    class StringSink : public Sink
    {
        public:
            ostringstream &getBuffer() {return buffer;};
            string getString() const {return buffer.str();};
    
        protected:
            virtual ostream &lockStream() {return buffer;};
            virtual void releaseStream() {};

        private:
            ostringstream buffer;
    };
} //namespace slog

#endif /* end of include guard: STRINGSINK_H_HBOURB8S */
