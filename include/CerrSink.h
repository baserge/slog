// CerrSink.h - CerrSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef CERRSINK_H_LLYSB1YF
#define CERRSINK_H_LLYSB1YF
#include "Sink.h"
#include <iostream>
namespace slog
{
    using std::cerr;
    // =========================================================================
    /// @brief Error to the standard error stream.
    // =========================================================================
    class CerrSink : public Sink
    {
        protected:
            virtual ostream &lockStream(){return cerr;}
            virtual void releaseStream() {};
    };
} //namespace slog

#endif /* end of include guard: CERRSINK_H_LLYSB1YF */
