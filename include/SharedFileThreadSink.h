// Licensed under GPLv3 or later, see the COPYING file.
#ifndef SHAREDFILETHREADSINK_H_J26GGQKH
#define SHAREDFILETHREADSINK_H_J26GGQKH
#include "Sink.h"
#include <mutex>
#include <fstream>
#include <string>
namespace slog
{
    using std::string;
    using std::ofstream;
    // =========================================================================
    /// @brief Thread safe file logger into a shared file.
    // =========================================================================
    class SharedFileThreadSink : public Sink
    {
        public:
            SharedFileThreadSink (const string &fileName, std::mutex &mutex,
                                  long int sizeLimit = 1024*1024*1024);
            virtual ~SharedFileThreadSink ();
            virtual Sink *clone() const {return new SharedFileThreadSink(fileName,
                                                                         mutex,
                                                                         sizeLimit);};

        protected:
            virtual std::mutex* getMutex() {return &mutex;};
            virtual ostream &getStream() {rollOver(); return file;};

        private:
            string fileName;
            ofstream file;
            std::mutex &mutex;
            size_t sizeLimit;
            void rollOver();
            void checkFail();
    };
} //namespace slog
#endif /* end of include guard: SHAREDFILETHREADSINK_H_J26GGQKH */
