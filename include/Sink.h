// Sink.h - Sink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef SINK_H_KTB3DX0H
#define SINK_H_KTB3DX0H
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

            template<class T>
            Sink &operator<< (const T &value);

            virtual Sink *clone() const = 0;
        protected:
            // return mutex for sink locking, can be null if not used by sink
            virtual std::mutex* getMutex() = 0;
            // return underlying stream for writing data, called with mutex
            // locked (if present)
            virtual ostream &getStream() = 0;
    };

    template<class T>
    inline Sink &Sink::operator<< (const T &value)
    {
        std::mutex *mutex = getMutex();
        if (mutex)
        {
            const std::lock_guard<std::mutex> lock(*mutex);
            getStream()<<value<<endl;
        }
        else
            getStream()<<value<<endl;
        return *this;
    }
} //namespace slog


#endif /* end of include guard: SINK_H_KTB3DX0H */
