// Licensed under GPLv3 or later, see the COPYING file.
#ifndef SHAREDSTRINGTHREADSINK_H_HBOURB8S
#define SHAREDSTRINGTHREADSINK_H_HBOURB8S
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
    /// @brief String buffer - messages into common thread safe buffer.
    // =========================================================================
    class SharedStringThreadSink : public Sink
    {
        public:
            SharedStringThreadSink(ostringstream &buffer, std::mutex &mutex) :
            buffer(buffer), mutex(mutex) {};
            const ostringstream &getBuffer() const {return buffer;};
            string getString() const {return buffer.str();};

            virtual Sink *clone() const {return new SharedStringThreadSink(buffer, mutex);};

        protected:
            virtual std::mutex* getMutex() {return &mutex;};
            virtual ostream &getStream() {return buffer;};

        private:
            ostringstream &buffer;
            std::mutex &mutex;
    };
} //namespace slog
#endif /* end of include guard: SHAREDSTRINGTHREADSINK_H_HBOURB8S */
