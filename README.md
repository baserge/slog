# Slog
*Slog* is a simple *C++* (>=11) library for logging, which somewhat mimics ideas from
the standard Python logging and at the same time keeps it simple and uses
output stream operators.  Four levels of urgency are supported for each logger:
debug, information, warning, error.  Each level can be configured to stream
messages to a different destination like terminal output, string
buffer, file, etc. This configuration is done in terms of *Sink* objects and
some of sinks support thread safety via *std::thread*.

A simple use example (once sinks are configured) is:

    #include "Slog/Logger.h"
    using namespace slog;  // all classes are defined in this namespace

    Logger &logger = Logger::getLogger();  // root-logger, singleton
    logger.getDebugSink()<<"my debug information a = "<<1;
    logger.getInfoSink()<<"my information a = "<<2;
    logger.getWarnSink()<<"my warning a = "<<-1;
    logger.getErrorSink()<<"an error message a = "<<0;

Here each line corresponds to a single message in the log-sink, which includes
a message header and is terminated by *endl*.

# Using the library
The central concept for the library is a *Logger* class. It is used to set up
sinks (log-level), to configure message header and to send messages to sinks.
A *Logger* instance can not be created directly, instead one should use
a static method *getLogger* with a logger name:

    Logger &logger1 = Logger::getLogger();  // root logger
    Logger &logger2 = Logger::getLogger("my_logger_name");

Note, that loggers should be used via references (assignment will fail).
Loggers can be named, i.e. they can represent logging for a category,
as shown in this example with *my_logger_name*.
Each logger is a singleton, i.e. every call to
*getLogger* with the same name returns the same object. Logger objects are created
only once and remain alive for the rest of the program, unless an explicit call
to *dropLoggers* is done. The logger without a name is called root-logger. Each
logger can be configured independently, but named loggers copy their initial
configuration from the root. That is, if a root logger was set up to output to
a file and this is not desired for a newly created named logger, the latter
should be reset, otherwise the new logger will continue outputting to the same
file.

## Log-level configuration
Each logger can output four message levels: *DEBUG, INFO, WARN, ERROR*. One can
configure output sinks for each level independently, e.g.:

    #include <memory>
    #include "Logger.h"
    #include "CoutSink.h"
    using namespace slog;

    Logger &logger = Logger::getLogger();
    logger.setDebugSink(std::unique_ptr<Sink>(new CoutSink));
    logger.setErrorSink(std::unique_ptr<Sink>(new CoutSink));

Here debugging and error messages are directed to the standard output, while
information and warning messages are, by default, discarded (i.e. the
corresponding sinks are *nullptr)*. Such configuration can be performed
independently for each logger. Initially named loggers inherit their settings
from the root one. Note, that a logger takes ownership of a passed sink.

To simplify setting a log-level, i.e. to set all sinks for levels below a
certain one, there are two convenience functions:

    logger.setLogLevel(Logger::INFO, std::unique_ptr<Sink>(new CoutSink));
    logger.setLogLevel("warn", std::unique_ptr<Sink>(new CoutSink));

The first variant accepts log-level as an enum value and the second one
works with string names. In both cases, all sinks for log-levels below or
equal to the passed one are set to copies of the input sink. That is,
in the first example above *ERROR, WARN, INFO* sinks will be directed to the
standard output, while the debugging sink will remain untouched.

## Available sinks
There are several implemented sinks (each defined in a corresponding header
file). Thread safety of some of the sinks is implemented with *std::mutex*.

**CoutSink**
    Output to the standard output (not thread safe).

**CoutThreadSink**
    Output to the standard output with thread safe locking (mutexing).

**CerrSink**
    Output to the standard error output (not thread safe).

**CerrThreadSink**
    Output to the standard error output with thread safe locking (mutexing).

**StringSink**
    Output to a string buffer (not thread safe).

**StringThreadSink**
    Output to a string buffer with thread safe locking (mutexing).
    This provides only locking for the same sink, e.g. if every
    sink level uses own buffer.

**SharedStringSink**
    Output to a string buffer shared by several sinks (not thread safe).

**SharedStringThreadSink**
    Output to a shared string buffer with thread safe locking (mutexing), e.g.
    if the same buffer is used for all logging levels.

**FileSink**
    Output to a file (not thread safe).

**FileThreadSink**
    Output to a file with thread safe locking (mutexing) per instance.
    This provides only locking for the same instance.

**SharedFileThreadSink**
    Output to a shared file with thread safe locking (mutexing), e.g.
    if the same file is used for all logging levels. This file sink
    also support a size limit on the log file (file truncation).

Further sinks can be developed by inheriting *Sink* class and implementing
required *virtual* methods.

## Message header
Log-message is a single line of output directed to a sink. For example:

    logger.getDebugSink()<<"my debug information a = "<<1<<", b = "<< 2;
    logger.getInfoSink()<<"my information a = "<<2;

Here there are two log messages are sent to the output: one to a debug sink
and one to an information sink. Each message is defined by a chained call
to the *operator<<* and can consist of multiple part. Messages are prepended
with a message header that contains some context information. By default this
includes the current log-level and the current logger name (if any), e.g.:

    [ERR][my_logger_name] the actual message comes here

It is also possible to include a thread id and the current time information
into the message header. This can be achieved with the following *Logger*
methods:

    Logger &logger = Logger::getLogger("my logger");
    logger.setWriteTime(true);
    logger.setWriteThreadId(true);

After this a message will look as:

    [ERR][Thread Id][Time][my_logger_name] the actual message comes here

Here thread id is printed as hex number and time is printed as
*YYYY-MM-DD HH:MM:SS*.

If is further possible to change also the message output format, e.g to
print integers as hex by using I/O manipulators:

    logger.getInfoSink()<<"test " << std::hex<< 10;

Note however, that such changes are not persistent, i.e. cover only the current
message, because of a temporary message buffer.

## Temporary message buffer
To be able to define a message as a chained call of the *opertor<<* and to avoid
potentially costly, repeated calls to a mutex, methods of the *getXSink* family
actually return a temporary buffer object instead of a direct reference to
a sink. Consider the following call:

    logger.getDebugSink();

Here a temporary instance of class *StreamBuffer* buffer is returned. This
object collects (if the log-level is on) the whole message from a
single chained call to the *operator<<* and on destruction outputs
it to the actual sink as a single string.
Therefore, one should not store the return of this function in an lvalue,
because in this case the message will get too long and will not be send
to the output until the object goes out of scope. Although, with some care
the following can be useful (pay attention to the scope):

    {
        auto buffer = logger.getInfoSink();
        std::vector<int> vec{10, 11, 1, 2};
        for (auto &x : vec)
            buffer<<x<<" ";
    }

To get access to the underlying sink one can call *getSink* method, which
returns a pointer (can be a nullptr):

    Sink *sink = logger.getDebugSink().getSink();

*StreamBuffer* class is very simple and is completely defined in its header
file. Therefore, it can be expected that all method calls will be inlined.

## What does it cost?
In the case a log-level is off (its sink is a *nullptr)*, message output
reduces to creation of a temporary object with 1 if-statement in the
constructor and 1 if-statement in the destructor, and 1 if-statement for
every call to the *operator<<*. Because the *StreamBuffer* class is small, it
can be expected that all operations are inlined and individual
if-statements are potentially combined.

In the case a log-level is on, it takes all the same operations as above
plus the actual message outputting with a potential mutex locking for
thread safe sinks.

# Installation
It is a *cmake* project and building is standard:

    mkdir build && cd build
    cmake ..
    make
    sudo make install

There are no external dependencies for the library.

To build tests (depends on *boost.tests*):

    make tests
    tests/tests
