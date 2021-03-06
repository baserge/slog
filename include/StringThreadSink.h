// Licensed under GPLv3 or later, see the COPYING file.
#ifndef STRINGTHREADSINK_H_J26GGQKH
#define STRINGTHREADSINK_H_J26GGQKH
#include "config.h"
#include "Sink.h"
#include <mutex>
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
            virtual std::mutex* getMutex() {return &mutex;};
            virtual ostream &getStream() {return buffer;};

        private:
            ostringstream buffer;
            std::mutex mutex;
    };
} //namespace slog
#endif /* end of include guard: STRINGTHREADSINK_H_J26GGQKH */
