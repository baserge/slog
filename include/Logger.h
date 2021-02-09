// Logger.h - Logger class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.

#ifndef LOGGER_H_C68YIU4S
#define LOGGER_H_C68YIU4S

#include "Sink.h"
#include "StreamBuffer.h"
#include <memory>
#include <string>

namespace slog
{
    using std::unique_ptr;
    using std::string;

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
            StreamBuffer getDebugSink() const
            {return StreamBuffer(name, debugSink);};
            StreamBuffer getInfoSink() const
            {return StreamBuffer(name, infoSink);};
            StreamBuffer getWarnSink() const
            {return StreamBuffer(name, warnSink);};
            StreamBuffer getErrorSink() const
            {return StreamBuffer(name, errorSink);};
            
            unique_ptr<Sink> setDebugSink(unique_ptr<Sink> sink);
            unique_ptr<Sink> setInfoSink(unique_ptr<Sink> sink);
            unique_ptr<Sink> setWarnSink(unique_ptr<Sink> sink);
            unique_ptr<Sink> setErrorSink(unique_ptr<Sink> sink);

            enum LogLevel
            {
                NONE = 0,
                ERROR = 1, // only error
                WARN = 2, // error, warn
                INFO = 3, // error, warn, info
                DEBUG = 4 // error, warn, info, debug
            };
            void setLogLevel(LogLevel level, unique_ptr<Sink> sink);
            void setLogLevel(string level, unique_ptr<Sink> sink);

            const string &getName() const {return name;};

            static char *getTimeStamp();

        private:
            string name;
            Sink *debugSink;
            Sink *infoSink;
            Sink *warnSink;
            Sink *errorSink;
            Logger(const string &name = "");
            ~Logger();
    };
} //namespace slog

#endif /* end of include guard: LOGGER_H_C68YIU4S */
