// Sink.h - Sink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef SINK_H_KTB3DX0H
#define SINK_H_KTB3DX0H
#include "config.h"
#include <ostream>
#include <string>
#include <thread>

namespace slog
{
    using std::ostream;
    using std::string;
    using std::endl;
    // =========================================================================
    /// @brief Interface definition for sink classes.
    // =========================================================================
    class Sink
    {
        public:
            Sink () {};
            virtual ~Sink () {};
            void setPrefix(const string &pref) {prefix = pref;};
            const string &getPrefix() const {return prefix;};
            template<class T>
            Sink &operator<< (const T &value);
            virtual Sink *clone() const = 0;
        protected:
            // =========================================================================
            /// @brief Lock the stream and if required and return it.
            // =========================================================================
            virtual ostream &lockStream() = 0;
            // =========================================================================
            /// @brief Unlock stream, if applicable.
            // =========================================================================
            virtual void releaseStream() = 0;
        private:
            string prefix;
    };

    template<class T>
    inline Sink &Sink::operator<< (const T &value)
    {
        ostream &strm = lockStream();
        if (!prefix.empty())
            strm<<"["<<prefix<<"]";
        strm<<"[TH"<<std::hex<<std::this_thread::get_id()<<"]";
        strm << value;
        strm<<endl;
        releaseStream();
        return *this;
    }
} //namespace slog


#endif /* end of include guard: SINK_H_KTB3DX0H */
