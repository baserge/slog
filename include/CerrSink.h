// Licensed under GPLv3 or later, see the COPYING file.
#ifndef CERRSINK_H_LLYSB1YF
#define CERRSINK_H_LLYSB1YF
#include "Sink.h"
#include <iostream>
namespace slog
{
    using std::cerr;
    // =========================================================================
    /// @brief Message to the standard error stream.
    // =========================================================================
    class CerrSink : public Sink
    {
        public:
            virtual Sink *clone() const {return new CerrSink;};
        protected:
            virtual std::mutex* getMutex() {return nullptr;};
            virtual ostream &getStream() {return cerr;};
    };
} //namespace slog

#endif /* end of include guard: CERRSINK_H_LLYSB1YF */
