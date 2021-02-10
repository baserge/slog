// FileThreadSink.h - FileThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef FILETHREADSINK_H_J26GGQKH
#define FILETHREADSINK_H_J26GGQKH
#include "config.h"
#include "Sink.h"
#include <mutex>
#include <fstream>
#include <string>
namespace slog
{
    using std::string;
    using std::ofstream;
    // =========================================================================
    /// @brief Thread safe file logger.
    //
    // This sinks has independent mutex per instance, that means that calls to
    // the same sink are thread safe. But if the same file is used for different
    // sinks or loggers, one should use sharedfile instead (which keeps references
    // to an exernal mutex).
    // =========================================================================
    class FileThreadSink : public Sink
    {
        public:
            FileThreadSink (const string &fileName);
            virtual ~FileThreadSink () {file.close();};
            virtual Sink *clone() const {return new FileThreadSink(fileName);};

        protected:
            virtual ostream &lockStream();
            virtual void releaseStream();

        private:
            ofstream file;
            std::mutex mutex;
            string fileName;
    };
} //namespace slog
#endif /* end of include guard: FILETHREADSINK_H_J26GGQKH */
