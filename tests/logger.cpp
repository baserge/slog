// matrix.cpp - Logger class tests, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "config.h"
#include "Logger.h"
#include "CoutSink.h"
#include "StringSink.h"
#include "SharedStringSink.h"
#include "SharedStringThreadSink.h"
#include "CerrSink.h"
#include "FileSink.h"
#include "CoutThreadSink.h"
#include "CerrThreadSink.h"
#include "StringThreadSink.h"
#include "FileThreadSink.h"
#include "SharedFileThreadSink.h"
#include <iostream>
#include <algorithm>
#include <cstdio>
#ifdef HAVE_KLUBOK
#include <Klubok/pool.h>
using namespace klubok;
#endif
#include <cstdlib>
#ifdef UNIX
#include <unistd.h>
#endif
#include <vector>

using namespace std;
using namespace slog;

BOOST_AUTO_TEST_SUITE(Logger_Tests)

BOOST_AUTO_TEST_CASE(nuller)
{
    Logger &logger = Logger::getLogger();
    BOOST_REQUIRE_EQUAL(logger.getName(), "");
    BOOST_REQUIRE_EQUAL(logger.getWriteTime(), false);
    logger.getDebugSink()<<"my message "<<1<<2<<3<<4<<5;
    logger.getInfoSink()<<"my message "<<2<<2<<3<<4<<5;
    logger.getWarnSink()<<"my message "<<3<<2<<3<<4<<5;
    logger.getErrorSink()<<"my message "<<4<<2<<3<<4<<5;
    logger.setLogLevel(Logger::NONE, unique_ptr<slog::Sink>(new CoutSink));
    logger.getDebugSink()<<"my message "<<1<<2<<3<<4<<5;
    logger.getInfoSink()<<"my message "<<2<<2<<3<<4<<5;
    logger.getWarnSink()<<"my message "<<3<<2<<3<<4<<5;
    logger.getErrorSink()<<"my message "<<4<<2<<3<<4<<5;
}

BOOST_AUTO_TEST_CASE(couter)
{
    Logger &logger = Logger::getLogger();
    logger.setDebugSink(unique_ptr<Sink>(new CoutSink));
    logger.setInfoSink(unique_ptr<Sink>(new CoutSink));
    logger.setWarnSink(unique_ptr<Sink>(new CoutSink));
    logger.setErrorSink(unique_ptr<Sink>(new CoutSink));
    logger.getDebugSink()<<"my message "<<1;
    logger.getInfoSink()<<"my message "<<2;
    logger.getWarnSink()<<"my message "<<3;
    logger.getErrorSink()<<"my message "<<4;
    logger.getDebugSink()<<"my message "<<Logger::getTimeStamp()<<"message";
    logger.setLogLevel(Logger::NONE, unique_ptr<slog::Sink>(new CoutSink));
    logger.getDebugSink()<<"my message (error if you see)"<<1<<2<<3<<4<<5;
    logger.getInfoSink()<<"my message (error if you see)"<<2<<2<<3<<4<<5;
    logger.getWarnSink()<<"my message (error if you see)"<<3<<2<<3<<4<<5;
    logger.getErrorSink()<<"my message (error if you see)"<<4<<2<<3<<4<<5;
    logger.setLogLevel(Logger::WARN, unique_ptr<slog::Sink>(new CoutSink));
    logger.getDebugSink()<<"my message (error if you see)"<<1<<2<<3<<4<<5;
    logger.getInfoSink()<<"my message (error if you see)"<<2<<2<<3<<4<<5;
    logger.getWarnSink()<<"my message "<<3<<2<<3<<4<<5;
    logger.getErrorSink()<<"my message "<<4<<2<<3<<4<<5;
}

BOOST_AUTO_TEST_CASE(cerrer)
{
    Logger &logger = Logger::getLogger();
    logger.setDebugSink(unique_ptr<Sink>(new CerrSink));
    logger.setInfoSink(unique_ptr<Sink>(new CerrSink));
    logger.setWarnSink(unique_ptr<Sink>(new CerrSink));
    logger.setErrorSink(unique_ptr<Sink>(new CerrSink));
    logger.getDebugSink()<<"my message "<<1;
    logger.getInfoSink()<<"my message "<<2;
    logger.getWarnSink()<<"my message "<<3;
    logger.getErrorSink()<<"my message "<<4;
}

BOOST_AUTO_TEST_CASE(file)
{
    Logger &logger = Logger::getLogger();
    logger.setDebugSink(unique_ptr<Sink>(new FileSink("shared.log", false)));
    logger.setInfoSink(unique_ptr<Sink>(new FileSink("shared.log")));
    logger.setWarnSink(unique_ptr<Sink>(new FileSink("shared.log")));
    logger.setErrorSink(unique_ptr<Sink>(new FileSink("shared.log")));
    logger.getDebugSink()<<"my message "<<1;
    logger.getInfoSink()<<"my message "<<2;
    logger.getWarnSink()<<"my message "<<3;
    logger.getErrorSink()<<"my message "<<4;
    std::string line;
    std::ifstream file("shared.log");
    int num = 0;
    while (std::getline(file, line))
        num++;
    BOOST_REQUIRE_EQUAL(num, 4);
}

BOOST_AUTO_TEST_CASE(strbuf_separate)
{
    Logger &logger = Logger::getLogger();
    logger.setDebugSink(unique_ptr<Sink>(new StringSink));
    logger.setInfoSink(unique_ptr<Sink>(new StringSink));
    logger.setWarnSink(unique_ptr<Sink>(new StringSink));
    logger.setErrorSink(unique_ptr<Sink>(new StringSink));
    logger.getDebugSink()<<"my message "<<1;
    logger.getInfoSink()<<"my message "<<2;
    logger.getWarnSink()<<"my message "<<3;
    logger.getErrorSink()<<"my message "<<4;
    istringstream strm(((StringSink*)logger.getDebugSink().getSink())->getString());
    std::string line;
    int num = 0;
    while (std::getline(strm, line))
        num++;
    BOOST_REQUIRE_EQUAL(num, 1);
    strm.clear();
    strm.str(((StringSink*)logger.getInfoSink().getSink())->getString());
    num = 0;
    while (std::getline(strm, line))
        num++;
    BOOST_REQUIRE_EQUAL(num, 1);
    strm.clear();
    strm.str(((StringSink*)logger.getWarnSink().getSink())->getString());
    num = 0;
    while (std::getline(strm, line))
        num++;
    BOOST_REQUIRE_EQUAL(num, 1);
    strm.clear();
    strm.str(((StringSink*)logger.getErrorSink().getSink())->getString());
    num = 0;
    while (std::getline(strm, line))
        num++;
    BOOST_REQUIRE_EQUAL(num, 1);
}

BOOST_AUTO_TEST_CASE(named_loggers)
{
    Logger &l1 = Logger::getLogger();
    l1.setDebugSink(unique_ptr<Sink>(new CoutSink));
    Logger &l2 = Logger::getLogger("");
    BOOST_REQUIRE_EQUAL(&l1, &l2);
    Logger &l3 = Logger::getLogger("test1");
    Logger &l4 = Logger::getLogger("test2");
    BOOST_REQUIRE(&l1 != &l3);
    BOOST_REQUIRE(&l3 != &l4);
    l1.dropLoggers();
    Logger &l31 = Logger::getLogger("test1");
    Logger &l41 = Logger::getLogger("test2");
    BOOST_REQUIRE(&l31 != &l3);
    BOOST_REQUIRE(&l41 != &l4);
    Logger &l32 = Logger::getLogger("test1");
    BOOST_REQUIRE(&l31 == &l32);
}

BOOST_AUTO_TEST_CASE(strbuf_shared)
{
    ostringstream buf;
    Logger &logger = Logger::getLogger();
    logger.setDebugSink(unique_ptr<Sink>(new SharedStringSink(buf)));
    logger.setInfoSink(unique_ptr<Sink>(new SharedStringSink(buf)));
    logger.setWarnSink(unique_ptr<Sink>(new SharedStringSink(buf)));
    logger.setErrorSink(unique_ptr<Sink>(new SharedStringSink(buf)));
    logger.getDebugSink()<<"my message "<<1;
    logger.getInfoSink()<<"my message "<<2;
    logger.getWarnSink()<<"my message "<<3;
    logger.getErrorSink()<<"my message "<<4;
    istringstream strm(((SharedStringSink*)logger.getDebugSink().getSink())->getString());
    std::string line;
    int num = 0;
    while (std::getline(strm, line))
        num++;
    BOOST_REQUIRE_EQUAL(num, 4);
    strm.clear();
    strm.str(((SharedStringSink*)logger.getInfoSink().getSink())->getString());
    num = 0;
    while (std::getline(strm, line))
        num++;
    BOOST_REQUIRE_EQUAL(num, 4);
    strm.clear();
    strm.str(((SharedStringSink*)logger.getWarnSink().getSink())->getString());
    num = 0;
    while (std::getline(strm, line))
        num++;
    BOOST_REQUIRE_EQUAL(num, 4);
    strm.clear();
    strm.str(((SharedStringSink*)logger.getErrorSink().getSink())->getString());
    num = 0;
    while (std::getline(strm, line))
        num++;
    BOOST_REQUIRE_EQUAL(num, 4);
}

#ifdef HAVE_KLUBOK
class DebugJob : public AbstractJob
{
    virtual void execute()
    {
        for (size_t i = 0; i < 10; i++)
        {
            Logger::getLogger().getDebugSink()<<"my message "<<i;
            float t = rand();
            t /= float(RAND_MAX);
#ifdef UNIX
            usleep(t*10.1);
#endif
        }
    
    }
};
class InfoJob : public AbstractJob
{
    virtual void execute()
    {
        for (size_t i = 0; i < 20; i++)
        {
            Logger::getLogger().getInfoSink()<<"my message "<< i;
            float t = rand();
            t /= float(RAND_MAX);
#ifdef UNIX
            usleep(t*3.0);
#endif
        }
    
    }
};
class WarnJob : public AbstractJob
{
    virtual void execute()
    {
        for (size_t i = 0; i < 20; i++)
        {
            Logger::getLogger().getWarnSink()<<"my message "<<i;
            float t = rand();
            t /= float(RAND_MAX);
#ifdef UNIX
            usleep(t*1.0);
#endif
        }
    
    }
};
class ErrorJob : public AbstractJob
{
    virtual void execute()
    {
        for (size_t i = 0; i < 20; i++)
        {
            Logger::getLogger().getErrorSink()<<"my message "<<i;
            float t = rand();
            t /= float(RAND_MAX);
#ifdef UNIX
            usleep(t*2.0);
#endif
        }
    
    }
};
BOOST_AUTO_TEST_CASE(coutThread)
{
    srand(0);
    Logger &logger = Logger::getLogger();
    logger.setDebugSink(unique_ptr<Sink>(new CoutThreadSink));
    logger.setInfoSink(unique_ptr<Sink>(new CoutThreadSink));
    logger.setWarnSink(unique_ptr<Sink>(new CoutThreadSink));
    logger.setErrorSink(unique_ptr<Sink>(new CoutThreadSink));
    vector<AbstractJob*> jobs;
    for (size_t i = 0; i < 4; i++)
    {
        jobs.push_back(new DebugJob);
        jobs.push_back(new InfoJob);
        jobs.push_back(new WarnJob);
        jobs.push_back(new ErrorJob);
    }
    ThreadPool pool(4*4);
    pool.addJob(jobs);
    pool.waitUntilEmpty();
}
BOOST_AUTO_TEST_CASE(cerrThread)
{
    srand(0);
    Logger &logger = Logger::getLogger();
    logger.setDebugSink(unique_ptr<Sink>(new CerrThreadSink));
    logger.setInfoSink(unique_ptr<Sink>(new CerrThreadSink));
    logger.setWarnSink(unique_ptr<Sink>(new CerrThreadSink));
    logger.setErrorSink(unique_ptr<Sink>(new CerrThreadSink));
    vector<AbstractJob*> jobs;
    for (size_t i = 0; i < 4; i++)
    {
        jobs.push_back(new DebugJob);
        jobs.push_back(new InfoJob);
        jobs.push_back(new WarnJob);
        jobs.push_back(new ErrorJob);
    }
    ThreadPool pool(4*4);
    pool.addJob(jobs);
    pool.waitUntilEmpty();
}
BOOST_AUTO_TEST_CASE(strbuf_shared_threaded)
{
    srand(0);
    ostringstream buf;
    Mutex m;
    Logger &logger = Logger::getLogger();
    logger.setDebugSink(unique_ptr<Sink>(new 
                                       SharedStringThreadSink(buf, m)
                                       ));
    logger.setInfoSink(unique_ptr<Sink>(new 
                                      SharedStringThreadSink(buf, m)
                                      ));
    logger.setWarnSink(unique_ptr<Sink>(new 
                                      SharedStringThreadSink(buf, m)
                                      ));
    logger.setErrorSink(unique_ptr<Sink>(new 
                                       SharedStringThreadSink(buf, m)
                                       ));
    vector<AbstractJob*> jobs;
    for (size_t i = 0; i < 4; i++)
    {
        jobs.push_back(new DebugJob);
        jobs.push_back(new InfoJob);
        jobs.push_back(new WarnJob);
        jobs.push_back(new ErrorJob);
    }
    ThreadPool pool(4*4);
    pool.addJob(jobs);
    pool.waitUntilEmpty();
    string s = buf.str();
    BOOST_REQUIRE_EQUAL(std::count(s.begin(), s.end(), '\n'), 70*4);
}
BOOST_AUTO_TEST_CASE(fileshared_threaded)
{
    srand(0);
    remove("shared_threaded.log");
    Mutex m;
    Logger &logger = Logger::getLogger();
    logger.setDebugSink(unique_ptr<Sink>(new 
                                       SharedFileThreadSink(
                                                        "shared_threaded.log",
                                                        m)
                                       ));
    logger.setInfoSink(unique_ptr<Sink>(new 
                                      SharedFileThreadSink(
                                                       "shared_threaded.log",
                                                       m)
                                      ));
    logger.setWarnSink(unique_ptr<Sink>(new 
                                      SharedFileThreadSink(
                                                       "shared_threaded.log",
                                                       m)
                                      ));
    logger.setErrorSink(unique_ptr<Sink>(new 
                                       SharedFileThreadSink(
                                                        "shared_threaded.log",
                                                        m)
                                       ));
    vector<AbstractJob*> jobs;
    for (size_t i = 0; i < 4; i++)
    {
        jobs.push_back(new DebugJob);
        jobs.push_back(new InfoJob);
        jobs.push_back(new WarnJob);
        jobs.push_back(new ErrorJob);
    }
    ThreadPool pool(4*4);
    pool.addJob(jobs);
    pool.waitUntilEmpty();
    std::string line;
    std::ifstream file("shared_threaded.log");
    int num = 0;
    while (std::getline(file, line))
        num++;
    BOOST_REQUIRE_EQUAL(num, 70*4);
}
BOOST_AUTO_TEST_CASE(fileshared_threaded_trunc)
{
    srand(0);
    remove("shared_threaded.log");
    Mutex m;
    Logger &logger = Logger::getLogger();
    logger.setDebugSink(unique_ptr<Sink>(new 
                                       SharedFileThreadSink(
                                                        "shared_threaded.log",
                                                        m, 1024)
                                       ));
    logger.setInfoSink(unique_ptr<Sink>(new 
                                      SharedFileThreadSink(
                                                       "shared_threaded.log",
                                                       m, 1024)
                                      ));
    logger.setWarnSink(unique_ptr<Sink>(new 
                                      SharedFileThreadSink(
                                                       "shared_threaded.log",
                                                       m, 1024)
                                      ));
    logger.setErrorSink(unique_ptr<Sink>(new 
                                       SharedFileThreadSink(
                                                        "shared_threaded.log",
                                                        m, 1024)
                                       ));
    vector<AbstractJob*> jobs;
    for (size_t i = 0; i < 4; i++)
    {
        jobs.push_back(new DebugJob);
        jobs.push_back(new InfoJob);
        jobs.push_back(new WarnJob);
        jobs.push_back(new ErrorJob);
    }
    ThreadPool pool(4*4);
    pool.addJob(jobs);
    pool.waitUntilEmpty();
    std::string line;
    std::ifstream file("shared_threaded.log");
    file.seekg(0, ios_base::end);
    BOOST_REQUIRE(file.tellg() <= 1024);
}
#endif

BOOST_AUTO_TEST_SUITE_END()
