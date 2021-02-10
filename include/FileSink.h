// Licensed under GPLv3 or later, see the COPYING file.
#ifndef FILESINK_H_2HKGDFLO
#define FILESINK_H_2HKGDFLO
#include "Sink.h"
#include <fstream>
#include <string>
namespace slog
{
    using std::string;
    using std::ofstream;
    // =========================================================================
    /// @brief File sink - saves messages to a file.
    // =========================================================================
    class FileSink : public Sink
    {
        public:
            FileSink (const string &fileName, bool append = true);
            virtual ~FileSink () {file.close();};
            virtual Sink *clone() const {return new FileSink(fileName, append);};

        protected:
            virtual std::mutex* getMutex() {return nullptr;};
            virtual ostream &getStream() {return file;};

        private:
            ofstream file;
            string fileName;
            bool append;
    };
} //namespace slog

#endif /* end of include guard: FILESINK_H_2HKGDFLO */
