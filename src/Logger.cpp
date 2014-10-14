// Logger.cpp - Logger implementation, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.

#include "config.h"
#include "Logger.h"
#include "Sink.h"
#include <ctime>
#include <cstring>
#include <vector>
#include <stdexcept>
using namespace slog;
using std::invalid_argument;

// =========================================================================
/// @brief Constructor.
//
// All sinks are set to null.
// =========================================================================
Logger::Logger()
{
    debugSink = 0;
    infoSink = 0;
    warnSink = 0;
    errorSink = 0;
}

Logger::~Logger()
{
    delete debugSink;
    delete infoSink;
    delete warnSink;
    delete errorSink;
}

// =========================================================================
/// @brief Set a new debugging sink.
///
/// The new sink will be destroyed once not required. The old sink
/// is returned.
// =========================================================================
auto_ptr<Sink> Logger::setDebugSink(auto_ptr<Sink> sink)
{
    auto_ptr<Sink> r(debugSink);
    debugSink = sink.release();
    if (debugSink)
        debugSink->setPrefix("DEB ");
    return r;
}
// =========================================================================
/// @brief Set a new info sink.
///
/// The new sink will be destroyed once not required. The old sink
/// is returned.
// =========================================================================
auto_ptr<Sink> Logger::setInfoSink(auto_ptr<Sink> sink)
{
    auto_ptr<Sink> ret(infoSink);
    infoSink = sink.release();
    if (infoSink)
       infoSink->setPrefix("INF ");
    return ret;
}
// =========================================================================
/// @brief Set a new warn sink.
///
/// The new sink will be destroyed once not required. The old sink
/// is returned.
// =========================================================================
auto_ptr<Sink> Logger::setWarnSink(auto_ptr<Sink> sink)
{
    auto_ptr<Sink> ret(warnSink);
    warnSink = sink.release();
    if (warnSink)
        warnSink->setPrefix("WAR ");
    return ret;
}
// =========================================================================
/// @brief Set a new eror sink.
///
/// The new sink will be destroyed once not required. The old sink
/// is returned.
// =========================================================================
auto_ptr<Sink> Logger::setErrorSink(auto_ptr<Sink> sink)
{
    auto_ptr<Sink> ret(errorSink);
    errorSink = sink.release();
    if (errorSink)
        errorSink->setPrefix("ERR ");
    return ret;
}

// =========================================================================
/// @brief Static method to get a single Logger instance.
// =========================================================================
Logger &Logger::getLogger()
{
    static Logger log = Logger();
    return log;
}

// =========================================================================
/// @brief Get UTC time stamp. Standard ascrime(gmtime()) with removed \n.
// =========================================================================
char *Logger::getTimeStamp()
{
    time_t now = time(0);
    char *s = asctime(gmtime(&now));
    s[strlen(s)-1] = ' ';
    return s;
}

typedef auto_ptr<Sink>(Logger::*func)(auto_ptr<Sink>);

// =========================================================================
/// @brief Set log level, with the same sink for all levels.
// =========================================================================
void Logger::setLogLevel(LogLevel level, auto_ptr<Sink> sink)
{
    func fs[4];
    fs[0] = &Logger::setErrorSink;
    fs[1] = &Logger::setWarnSink;
    fs[2] = &Logger::setInfoSink;
    fs[3] = &Logger::setDebugSink;
    if (level > 4)
        throw invalid_argument("unknown log level");
    Sink *s = sink.release();
    for (size_t i = 0; i < 4; i++)
    {
        if (i < level)
            (this->*fs[i])(auto_ptr<Sink>(s->clone()));
        else
            (this->*fs[i])(auto_ptr<Sink>(0));
    }
    delete s;
}

void Logger::setLogLevel(string level, auto_ptr<Sink> sink)
{
    LogLevel lv;
    for (size_t i = 0; i < level.size(); i++)
        level[i] = char(toupper(level[i]));
    if (level == "NONE")
        lv = NONE;
    else if (level == "ERROR")
        lv = ERROR;
    else if (level == "WARN")
        lv = WARN;
    else if (level == "INFO")
        lv = INFO;
    else if (level == "DEBUG")
        lv = DEBUG;
    else
        throw invalid_argument("unknown log level");
    setLogLevel(lv, sink);
}
