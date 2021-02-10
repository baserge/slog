// Sink.h - Sink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef SINK_H_KTB3DX0H
#define SINK_H_KTB3DX0H
#include "config.h"
#include <ostream>
#include <string>
#include <thread>
#include <mutex>

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
            // return mutex for sink locking, can be null if not used by sink
            virtual std::mutex* getMutex() = 0;
            // return underlying stream for writing data
            virtual ostream &getStream() = 0;
        private:
            string prefix;
            template<class T>
            void output(const T &value, ostream &strm);
    };

    template<class T>
    void Sink::output(const T &value, ostream &strm)
    {
        if (!prefix.empty())
            strm<<"["<<prefix<<"]";
        strm<<"[TH"<<std::hex<<std::this_thread::get_id()<<"]";
        strm << value;
        strm<<endl;
    }

    template<class T>
    inline Sink &Sink::operator<< (const T &value)
    {
        std::mutex *mutex = getMutex();
        ostream &strm = getStream();
        if (mutex)
        {
            const std::lock_guard<std::mutex> lock(*mutex);
            output(value, strm);
        }
        else
            output(value, strm);
        return *this;
    }
} //namespace slog


#endif /* end of include guard: SINK_H_KTB3DX0H */
