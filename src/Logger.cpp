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
#include <map>
using namespace slog;
using std::invalid_argument;
using std::map;

// =========================================================================
/// @brief Constructor.
//
// All sinks are set to null.
// =========================================================================
Logger::Logger()
{
    debugSink = nullptr;
    infoSink = nullptr;
    warnSink = nullptr;
    errorSink = nullptr;
}

Logger::~Logger()
{
    // first check that pointers are indeed unique, otherwise we may double
    // delete
    if (debugSink == infoSink || debugSink == warnSink ||
        debugSink == errorSink)
        debugSink = nullptr;
    if (infoSink == warnSink || infoSink == errorSink)
        infoSink = nullptr;
    if (warnSink == errorSink)
        warnSink = nullptr;
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
unique_ptr<Sink> Logger::setDebugSink(unique_ptr<Sink> sink)
{
    if (sink.get() == debugSink)
        return unique_ptr<Sink>();
    unique_ptr<Sink> r(debugSink);
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
unique_ptr<Sink> Logger::setInfoSink(unique_ptr<Sink> sink)
{
    if (sink.get() == infoSink)
        return unique_ptr<Sink>();
    unique_ptr<Sink> ret(infoSink);
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
unique_ptr<Sink> Logger::setWarnSink(unique_ptr<Sink> sink)
{
    if (sink.get() == warnSink)
        return unique_ptr<Sink>();
    unique_ptr<Sink> ret(warnSink);
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
unique_ptr<Sink> Logger::setErrorSink(unique_ptr<Sink> sink)
{
    if (sink.get() == errorSink)
        return unique_ptr<Sink>();
    unique_ptr<Sink> ret(errorSink);
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
    static map<string, Logger*> instances;
    if (!instances.count("")) // instance not present -> create it
        instances[""] = new Logger;
    return *instances[""];
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

typedef unique_ptr<Sink>(Logger::*func)(unique_ptr<Sink>);

// =========================================================================
/// @brief Set log level, with the same sink for all levels.
// =========================================================================
void Logger::setLogLevel(LogLevel level, unique_ptr<Sink> sink)
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
            (this->*fs[i])(unique_ptr<Sink>(s->clone()));
        else
            (this->*fs[i])(unique_ptr<Sink>());
    }
    delete s;
}

void Logger::setLogLevel(string level, unique_ptr<Sink> sink)
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
    setLogLevel(lv, std::move(sink));
}
