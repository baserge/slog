// CerrThreadSink.h - CerrThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef CERRTHREADSINK_H_J26GGQKH
#define CERRTHREADSINK_H_J26GGQKH
#include "config.h"
#ifdef HAVE_KLUBOK
#include "Sink.h"
#include <Klubok/thread.h>
#include <iostream>
namespace slog
{
    using std::cerr;
    // =========================================================================
    /// @brief Thread safe logger to standard error stream.
    // =========================================================================
    class CerrThreadSink : public Sink
    {
        protected:
            virtual ostream &lockStream();
            virtual void releaseStream();
    
        private:
            static klubok::Mutex mutex;
    };
} //namespace slog
#endif
#endif /* end of include guard: CERRTHREADSINK_H_J26GGQKH */
