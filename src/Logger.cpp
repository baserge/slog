// Logger.cpp - Logger implementation, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.

#include "config.h"
#include "Logger.h"
#include "Sink.h"
using namespace slog;

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
    if (!sink.get())
        return auto_ptr<Sink>(0);
    auto_ptr<Sink> r(debugSink);
    debugSink = sink.release();
    debugSink->setPrefix("DEB");
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
    if (!sink.get())
        return auto_ptr<Sink>(0);
    auto_ptr<Sink> ret(infoSink);
    infoSink = sink.release();
    infoSink->setPrefix("INF");
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
    if (!sink.get())
        return auto_ptr<Sink>(0);
    auto_ptr<Sink> ret(warnSink);
    warnSink = sink.release();
    warnSink->setPrefix("WAR");
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
    if (!sink.get())
        return auto_ptr<Sink>(0);
    auto_ptr<Sink> ret(errorSink);
    errorSink = sink.release();
    errorSink->setPrefix("ERR");
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

