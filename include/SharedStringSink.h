// SharedStringSink.h - SharedStringSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef SHAREDSTRINGSINK_H_HBOURB8S
#define SHAREDSTRINGSINK_H_HBOURB8S
#include "Sink.h"
#include <sstream>
#include <string>
namespace slog
{
    using std::ostringstream;
    using std::string;
    // =========================================================================
    /// @brief String buffer - saves messages into common ostringstream buffer.
    // =========================================================================
    class SharedStringSink : public Sink
    {
        public:
            SharedStringSink(ostringstream &buffer) : buffer(buffer) {};
            ostringstream &getBuffer() {return buffer;};
            string getString() const {return buffer.str();};

            virtual Sink *clone() const {return new SharedStringSink(buffer);};

        protected:
            virtual std::mutex* getMutex() {return nullptr;};
            virtual ostream &getStream() {return buffer;};

        private:
            ostringstream &buffer;
    };
} //namespace slog

#endif /* end of include guard: SHAREDSTRINGSINK_H_HBOURB8S */
