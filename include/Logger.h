// Logger.h - Logger class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.

#ifndef LOGGER_H_C68YIU4S
#define LOGGER_H_C68YIU4S

#include "Sink.h"
#include "StreamBuffer.h"
#include <memory>

namespace slog
{
    using std::auto_ptr;

    // =========================================================================
    /// @brief Logger class.
    //
    // Logger class is a singleton created at the first call to getLogget().
    // With a Logger instance one can log messages to 4 sinks: debug, information,
    // warning and error. By default they all are set to a null sink, i.e. do not
    // log anything. To change this use a corresponding setSink method.
    // Afterwards to log a debug message simply do:
    // Logget::getLogger().getDebugSink()<<"my messages". It is up to a sink
    // implementation to provide support of terminal output, strings, files,
    // thread safety etc.
    //
    // Note that the logger will try to delete the set sinks.
    // =========================================================================
    class Logger
    {
        public:
            static Logger &getLogger();
            StreamBuffer getDebugSink() const {return StreamBuffer(debugSink);};
            StreamBuffer getInfoSink() const {return StreamBuffer(infoSink);};
            StreamBuffer getWarnSink() const {return StreamBuffer(warnSink);};
            StreamBuffer getErrorSink() const {return StreamBuffer(errorSink);};
            
            auto_ptr<Sink> setDebugSink(auto_ptr<Sink> sink);
            auto_ptr<Sink> setInfoSink(auto_ptr<Sink> sink);
            auto_ptr<Sink> setWarnSink(auto_ptr<Sink> sink);
            auto_ptr<Sink> setErrorSink(auto_ptr<Sink> sink);

            static char *getTimeStamp();

        private:
            Sink *debugSink;
            Sink *infoSink;
            Sink *warnSink;
            Sink *errorSink;
            Logger();
            ~Logger();
    };
} //namespace slog

#endif /* end of include guard: LOGGER_H_C68YIU4S */
