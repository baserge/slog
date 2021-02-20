// Licensed under GPLv3 or later, see the COPYING file.

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <thread>
#include <vector>
#include <chrono>
#include <algorithm>

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

using namespace std;
using namespace slog;

BOOST_AUTO_TEST_SUITE(Logger_Tests)

BOOST_AUTO_TEST_CASE(nuller)
{
    Logger &logger = Logger::getLogger();
    BOOST_REQUIRE_EQUAL(logger.getName(), "");
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
    logger.getDebugSink()<<"my message "<<"message";
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
    l2.getDebugSink()<<"test";
    Logger &l3 = Logger::getLogger("test1");
    Logger &l4 = Logger::getLogger("test2");
    l3.setWriteTime(true);
    l3.getDebugSink()<<"test";
    l4.setWriteTime(true);
    l4.getDebugSink()<<"test 4";
    BOOST_REQUIRE(&l1 != &l3);
    BOOST_REQUIRE(&l3 != &l4);
    BOOST_REQUIRE(l3.getDebugHeader().getWriteTime() == true);
    BOOST_REQUIRE(l4.getDebugHeader().getWriteTime() == true);
    l1.dropLoggers();
    Logger &l31 = Logger::getLogger("test1");
    Logger &l41 = Logger::getLogger("test2");
    BOOST_REQUIRE(l31.getDebugHeader().getWriteTime() == false);
    BOOST_REQUIRE(l41.getDebugHeader().getWriteTime() == false);
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
    istringstream strm(buf.str());
    std::string line;
    int num = 0;
    while (std::getline(strm, line))
        num++;
    BOOST_REQUIRE_EQUAL(num, 4);
}

void printDebug()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    for (size_t i = 0; i < 30; i++)
    {
        float t = rand();
        t /= float(RAND_MAX);
        Logger::getLogger().getDebugSink()<<"my message "<<i<<", sleep "<<int(t*3)<<" ms";
        std::this_thread::sleep_for(std::chrono::milliseconds(int(t*3)));
    }
}

void printInfo()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(70));
    for (size_t i = 0; i < 20; i++)
    {
        float t = rand();
        t /= float(RAND_MAX);
        Logger::getLogger().getInfoSink()<<"my message "<<i<<", sleep "<<int(t*7)<<" ms";
        std::this_thread::sleep_for(std::chrono::milliseconds(int(t*7)));
    }
}

void printWarn()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(110));
    for (size_t i = 0; i < 20; i++)
    {
        float t = rand();
        t /= float(RAND_MAX);
        Logger::getLogger().getWarnSink()<<"my message "<<i<<", sleep "<<int(t*6)<<" ms";
        std::this_thread::sleep_for(std::chrono::milliseconds(int(t*6)));
    }
}

void printError()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(80));
    for (size_t i = 0; i < 15; i++)
    {
        float t = rand();
        t /= float(RAND_MAX);
        Logger::getLogger().getErrorSink()<<"my message "<<i<<", sleep "<<int(t*5)<<" ms";
        std::this_thread::sleep_for(std::chrono::milliseconds(int(t*5)));
    }
}

BOOST_AUTO_TEST_CASE(coutThread)
{
    srand(0);
    Logger &logger = Logger::getLogger();
    logger.setWriteThreadId(true);
    logger.setDebugSink(unique_ptr<Sink>(new CoutThreadSink));
    logger.setInfoSink(unique_ptr<Sink>(new CoutThreadSink));
    logger.setWarnSink(unique_ptr<Sink>(new CoutThreadSink));
    logger.setErrorSink(unique_ptr<Sink>(new CoutThreadSink));
    std::vector<std::thread> jobs;
    for (size_t i = 0; i < 4; i++)
    {
        jobs.push_back(std::thread(printDebug));
        jobs.push_back(std::thread(printInfo));
        jobs.push_back(std::thread(printWarn));
        jobs.push_back(std::thread(printError));
    }
    for (auto &v : jobs)
        v.join();
}

BOOST_AUTO_TEST_CASE(cerrThread)
{
    srand(0);
    Logger &logger = Logger::getLogger();
    logger.setWriteThreadId(true);
    logger.setDebugSink(unique_ptr<Sink>(new CerrThreadSink));
    logger.setInfoSink(unique_ptr<Sink>(new CerrThreadSink));
    logger.setWarnSink(unique_ptr<Sink>(new CerrThreadSink));
    logger.setErrorSink(unique_ptr<Sink>(new CerrThreadSink));

    std::vector<std::thread> jobs;
    for (size_t i = 0; i < 4; i++)
    {
        jobs.push_back(std::thread(printDebug));
        jobs.push_back(std::thread(printInfo));
        jobs.push_back(std::thread(printWarn));
        jobs.push_back(std::thread(printError));
    }
    for (auto &v : jobs)
        v.join();
}

BOOST_AUTO_TEST_CASE(strbuf_shared_threaded)
{
    srand(0);
    ostringstream buf;
    std::mutex m;
    Logger &logger = Logger::getLogger();
    logger.setWriteThreadId(true);
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
    std::vector<std::thread> jobs;
    for (size_t i = 0; i < 4; i++)
    {
        jobs.push_back(std::thread(printDebug));
        jobs.push_back(std::thread(printInfo));
        jobs.push_back(std::thread(printWarn));
        jobs.push_back(std::thread(printError));
    }
    for (auto &v : jobs)
        v.join();
    string s = buf.str();
    BOOST_REQUIRE_EQUAL(std::count(s.begin(), s.end(), '\n'), 85*4);
}

BOOST_AUTO_TEST_CASE(fileshared_threaded)
{
    srand(0);
    remove("shared_threaded.log");
    std::mutex m;
    Logger &logger = Logger::getLogger();
    logger.setWriteThreadId(true);
    logger.setWriteTime(true);
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
    std::vector<std::thread> jobs;
    for (size_t i = 0; i < 4; i++)
    {
        jobs.push_back(std::thread(printDebug));
        jobs.push_back(std::thread(printInfo));
        jobs.push_back(std::thread(printWarn));
        jobs.push_back(std::thread(printError));
    }
    for (auto &v : jobs)
        v.join();
    std::string line;
    std::ifstream file("shared_threaded.log");
    int num = 0;
    while (std::getline(file, line))
        num++;
    BOOST_REQUIRE_EQUAL(num, 85*4);
}

BOOST_AUTO_TEST_CASE(fileshared_threaded_trunc)
{
    srand(0);
    remove("shared_threaded_trunc.log");
    std::mutex m;
    Logger &logger = Logger::getLogger();
    logger.setWriteThreadId(true);
    logger.setDebugSink(unique_ptr<Sink>(new 
                                       SharedFileThreadSink(
                                                        "shared_threaded_trunc.log",
                                                        m, 1024)
                                       ));
    logger.setInfoSink(unique_ptr<Sink>(new 
                                      SharedFileThreadSink(
                                                       "shared_threaded_trunc.log",
                                                       m, 1024)
                                      ));
    logger.setWarnSink(unique_ptr<Sink>(new 
                                      SharedFileThreadSink(
                                                       "shared_threaded_trunc.log",
                                                       m, 1024)
                                      ));
    logger.setErrorSink(unique_ptr<Sink>(new 
                                       SharedFileThreadSink(
                                                        "shared_threaded_trunc.log",
                                                        m, 1024)
                                       ));
    std::vector<std::thread> jobs;
    for (size_t i = 0; i < 4; i++)
    {
        jobs.push_back(std::thread(printDebug));
        jobs.push_back(std::thread(printInfo));
        jobs.push_back(std::thread(printWarn));
        jobs.push_back(std::thread(printError));
    }
    for (auto &v : jobs)
        v.join();
    std::string line;
    std::ifstream file("shared_threaded_trunc.log");
    file.seekg(0, ios_base::end);
    BOOST_REQUIRE(file.tellg() <= 1024);  // it is a bit of a race and can fail sometimes
}

BOOST_AUTO_TEST_SUITE_END()
