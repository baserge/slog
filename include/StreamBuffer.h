// StreamBuffer.h - StreamBuffer class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef STREAMBUFFER_H_D1JCYNSW
#define STREAMBUFFER_H_D1JCYNSW
#include "Sink.h"
#include <sstream>
#include <string>
namespace slog
{
    using std::ostringstream;
    using std::string;
    // =========================================================================
    /// @brief Temporary object to combine messages.
    //
    // A temporary wrapping object is returned by a logger instance.
    // Such an object exists only as long as the stream chain proceeds,
    // on destruction the concatenated string is flushed to the real sink.
    // =========================================================================
    class StreamBuffer
    {
        public:
            StreamBuffer(const string &name, Sink *sink, bool writeTime) :
                name(name), sink(sink), writeTime(writeTime), strm(nullptr) {};
            ~StreamBuffer ()
            {
                if (strm)
                    *sink<<strm->str();
                delete strm;
            };

            Sink *getSink() {return sink;};

            template <class T>
            StreamBuffer &operator<<(const T &value);


        private:
            const string &name;
            Sink *sink;
            bool writeTime;
            ostringstream *strm;
            void writeTimeStamp(ostringstream *strm);
    };
    template <class T>
    StreamBuffer &StreamBuffer::operator<<(const T &value)
    {
        if (sink)
        {
            if (!strm)
            {
                strm = new ostringstream;
                if (!name.empty())
                    *strm << "["<<name<<"]";
                if (writeTime)
                    writeTimeStamp(strm);
                *strm << " ";
            }
            *strm << value;
        }
        return *this;
    }
} //namespace slog



#endif /* end of include guard: STREAMBUFFER_H_D1JCYNSW */
