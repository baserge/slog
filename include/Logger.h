// Logger.h - Logger class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.

#ifndef LOGGER_H_C68YIU4S
#define LOGGER_H_C68YIU4S

#include "Sink.h"
#include "StreamBuffer.h"
#include <memory>
#include <string>
#include <map>

namespace slog
{
    using std::unique_ptr;
    using std::string;
    using std::map;

    // =========================================================================
    /// @brief Logger class.
    //
    // Loggers are named and can be created with the static method
    // getLogger(). They are created only once (on the first request).
    // If no name is supplied, this method returns the root logger.
    //
    // With a Logger instance one can log messages to 4 sinks: debug, info,
    // warn and error. By default they all are set to a null sink, i.e. do not
    // log anything. To change this use a corresponding setSink method.
    // Afterwards to log a debug message simply do:
    // Logger::getLogger().getDebugSink()<<"my messages". It is up to a sink
    // implementation to provide support of terminal output, strings, files,
    // thread safety etc.
    //
    // It is possible to set different sinks to different loggers. Note, that
    // initially sinks for named loggers are copied from the root logger. But
    // after the first use, named loggers will not be affected by changes to the
    // root logger.
    //
    // Note that Loggers will delete their sinks.
    // =========================================================================
    class Logger
    {
        public:
            static Logger &getLogger(const string &name = "");
            StreamBuffer getDebugSink() const
            {return StreamBuffer(name, debugSink, writeTime);};
            StreamBuffer getInfoSink() const
            {return StreamBuffer(name, infoSink, writeTime);};
            StreamBuffer getWarnSink() const
            {return StreamBuffer(name, warnSink, writeTime);};
            StreamBuffer getErrorSink() const
            {return StreamBuffer(name, errorSink, writeTime);};
            
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
            void setWriteTime(bool v) {writeTime = v;};
            bool getWriteTime() const {return writeTime;};

            const string &getName() const {return name;};

            static string getTimeStamp();

            void dropLoggers();
        private:
            string name;
            bool writeTime;
            Sink *debugSink;
            Sink *infoSink;
            Sink *warnSink;
            Sink *errorSink;
            map<string, Logger*> *instances;
            Logger(const string &name = "");
            ~Logger();
            void setClonedSinks(const Logger *other);
            void setInstancesMap(map<string, Logger*> *v) {instances = v;};
    };
} //namespace slog

#endif /* end of include guard: LOGGER_H_C68YIU4S */
