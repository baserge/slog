// Licensed under GPLv3 or later, see the COPYING file.
#ifndef STREAMBUFFER_H_D1JCYNSW
#define STREAMBUFFER_H_D1JCYNSW
#include "Sink.h"
#include "Header.h"
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
    // Such an object exists only as long as a stream chain proceeds,
    // on destruction the concatenated string is flushed to the real sink.
    //
    // The main purposes of this object is to concatenate the whole message
    // before passing it to the real sink, thus avoiding unnecessary mutex
    // locks/unlocks for outputting every variable. This class is not intended
    // for direct use, but will be involved in a typical situation:
    // logger.getDebugSink()<<"trying to do something with x = "<< x
    // << ", y = "<< y << ", vector size"<< z.size();
    // Stream buffer will create a whole string with message header as required
    // and will pass it in one go to the Sink. Such a line is considered as a
    // single message that should be logged on a separate line. This involves
    // copying the message, but this seems to be preferable instead of
    // potentially multiple locks/unlocks.
    //
    // Note that message forming and passing are performed only if the current
    // log level is active (Sink is set). If not, the execution is equivalent
    // to estimating a few if-statements (one per message part + 2 in addition).
    // =========================================================================
    class StreamBuffer
    {
        public:
            StreamBuffer(Sink *sink, const Header &header) :
                 sink(sink), header(header)
            {
                if (sink)
                    strm << header;
            };
            StreamBuffer(const StreamBuffer &inst) :
                sink(inst.sink), header(inst.header)
            {
                if (sink)
                    strm << header;
            }
            ~StreamBuffer ()
            {
                if (sink)
                    *sink<<strm.str();
            };

            Sink *getSink() {return sink;};

            template <class T>
            StreamBuffer &operator<<(const T &value);


        private:
            Sink *sink;
            const Header &header;
            ostringstream strm;
    };
    template <class T>
    StreamBuffer &StreamBuffer::operator<<(const T &value)
    {
        if (sink)
            strm << value;
        return *this;
    }
} //namespace slog



#endif /* end of include guard: STREAMBUFFER_H_D1JCYNSW */
