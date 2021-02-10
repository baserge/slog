// Licensed under GPLv3 or later, see the COPYING file.
#ifndef COUTTHREADSINK_H_J26GGQKH
#define COUTTHREADSINK_H_J26GGQKH
#include "Sink.h"
#include <mutex>
#include <iostream>
namespace slog
{
    using std::cout;
    // =========================================================================
    /// @brief Thread safe logger to standard output.
    // =========================================================================
    class CoutThreadSink : public Sink
    {
        public:
            virtual Sink *clone() const {return new CoutThreadSink;};
        protected:
            virtual std::mutex* getMutex() {return &mutex;};
            virtual ostream &getStream() {return cout;};

        private:
            static std::mutex mutex;
    };
} //namespace slog
#endif /* end of include guard: COUTTHREADSINK_H_J26GGQKH */
