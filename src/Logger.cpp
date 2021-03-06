// Licensed under GPLv3 or later, see the COPYING file.

#include "config.h"
#include "Logger.h"
#include "Sink.h"
#include <cstring>
#include <vector>
#include <stdexcept>
#include <mutex>
using namespace slog;
using std::invalid_argument;

// =========================================================================
/// @brief Constructor.
//
// All sinks are set to null.
// =========================================================================
Logger::Logger(const string &name) : name(name),
    debugHeader("DEB", name, false, false),
    infoHeader("INF", name, false, false),
    warnHeader("WAR", name, false, false),
    errorHeader("ERR", name, false, false)
{
    debugSink = nullptr;
    infoSink = nullptr;
    warnSink = nullptr;
    errorSink = nullptr;
    instances = nullptr;
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
    return ret;
}

// =========================================================================
/// @brief Set this instance sinks by cloning those of the supplied instance.
// =========================================================================
void Logger::setClonedSinks(const Logger *other)
{
    if (!other)
        return;
    if (other->debugSink)
        this->setDebugSink(unique_ptr<Sink>(other->debugSink->clone()));
    if (other->infoSink)
        this->setInfoSink(unique_ptr<Sink>(other->infoSink->clone()));
    if (other->warnSink)
        this->setWarnSink(unique_ptr<Sink>(other->warnSink->clone()));
    if (other->errorSink)
        this->setErrorSink(unique_ptr<Sink>(other->errorSink->clone()));
}

// =========================================================================
/// @brief Static method to get a single Logger instance.
// =========================================================================
Logger &Logger::getLogger(const string &name)
{
    static map<string, Logger*> instances;
    static std::mutex mutex;
    const std::lock_guard<std::mutex> lock(mutex);
    if (!instances.count("")) // first check root logger is present
    {
        instances[""] = new Logger();
        instances[""]->setInstancesMap(&instances);
    }
    if (!instances.count(name)) // instance not present -> create it
    {
        instances[name] = new Logger(name);
        instances[name]->setClonedSinks(instances[""]);
        instances[name]->setInstancesMap(&instances);
    }
    Logger *inst = instances[name];
    return *inst;
}

// =========================================================================
/// @brief Static method to delete all but root logger.
//
//  Be warned! This invalidates all references (including the calling),
//  except that to the root logger. Also no thread safety is provided here.
// =========================================================================
void Logger::dropLoggers()
{
    if (!instances)
        return;
    //std::cout<<"old size"<<instances->size()<<std::endl;
    for(auto it = instances->begin(); it != instances->end(); )
    {
        if (it->first != "")  // first is key
        {
            delete it->second;  // second is value
            it = instances->erase(it);  // iterator to the next is returned
        }
        else
            ++it;
    }
    //std::cout<<"new size"<<instances->size()<<std::endl;
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

void Logger::setWriteTime(bool v)
{
    debugHeader.setWriteTime(v);
    infoHeader.setWriteTime(v);
    warnHeader.setWriteTime(v);
    errorHeader.setWriteTime(v);
}

void Logger::setWriteThreadId(bool v)
{
    debugHeader.setWriteThread(v);
    infoHeader.setWriteThread(v);
    warnHeader.setWriteThread(v);
    errorHeader.setWriteThread(v);
}
