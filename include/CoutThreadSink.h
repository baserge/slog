// CoutThreadSink.h - CoutThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef COUTTHREADSINK_H_J26GGQKH
#define COUTTHREADSINK_H_J26GGQKH
#include "config.h"
#ifdef HAVE_KLUBOK
#include "Sink.h"
#include <Klubok/thread.h>
#include <iostream>
namespace slog
{
    using std::cout;
    // =========================================================================
    /// @brief Thread safe logger to standard output.
    // =========================================================================
    class CoutThreadSink : public Sink
    {
        protected:
            virtual ostream &lockStream();
            virtual void releaseStream();
    
        private:
            static klubok::Mutex mutex;
    };
} //namespace slog
#endif
#endif /* end of include guard: COUTTHREADSINK_H_J26GGQKH */
