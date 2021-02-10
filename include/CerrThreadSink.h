// CerrThreadSink.h - CerrThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef CERRTHREADSINK_H_J26GGQKH
#define CERRTHREADSINK_H_J26GGQKH
#include "config.h"
#include "Sink.h"
#include <mutex>
#include <iostream>
namespace slog
{
    using std::cerr;
    // =========================================================================
    /// @brief Thread safe logger to standard error stream.
    // =========================================================================
    class CerrThreadSink : public Sink
    {
        public:
            virtual Sink *clone() const {return new CerrThreadSink;};
        protected:
            virtual ostream &lockStream();
            virtual void releaseStream();
    
        private:
            static std::mutex mutex;
    };
} //namespace slog
#endif /* end of include guard: CERRTHREADSINK_H_J26GGQKH */
