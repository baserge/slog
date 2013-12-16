.. overview.rst - documentation in rst format, part of Slog.
   Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
   Licensed under GPLv3 or later, see the COPYING file.
.. include:: links.rst

Overview
--------
*Slog* is a *C++* library for logging. It supports
four levels of urgency: debug, information, warning,
error. Each level can be configured to stream messages
to a different destination like terminal output, string
buffer, file. The library provides thread safe logging
by using `Klubok`_
library. 

The source code should be available via 
`Mercurial <http://mercurial.selenic.com/>`_ at:

    :file:`/afs/ipp-garching.mpg.de/u/boz/progs/cpp/Slog`

This documentation should be available
`here <http://lxpowerboz:88/documentation/cpp/Slog>`_,
or as a pdf-document
`here <http://lxpowerboz:88/downloads/documentation/cpp/Slog.pdf>`_.
If you have any bugs or ideas to report, please use
`the issue tracker <http://lxpowerboz:88/tracker/main>`_
and choose ``Slog`` as the project.
Please assign the issue either to ``administartor`` or to my name.

Examples
========
*Logger* class provides access to the library functionality.
This is a singleton class, whose the only instance is to be
accessed by *getLogger* method. Each instance has four
sinks for each level of the message urgency: debug, information,
warning, error. Sinks support normal stream operations.
To sum up, to use the library it is sufficient to do the following:


.. code-block:: cpp

    #include "Slog/Logger.h"
    using namespace slog;

    Logger &logger = Logger::getLogger();
    logger.getDebugSink()<<"my debug information a = "<<1;
    logger.getInfoSink()<<"my information a = "<<2;
    logger.getWarnSink()<<"my warning a = "<<-1;
    logger.getErrorSink()<<"an error message a = "<<0;

By default all sinks are null sinks that do not do any logging.
The four lines above are equivalent to validating an *if*-statement.
To get a useful messaging it is necessary to set a desired sink to non-null.

.. code-block:: cpp

    #include "Slog/CoutSink.h"
    using namespace slog;

    logger.setInfoSink(auto_ptr<Sink>(new CoutSink));
    logger.getInfoSink()<<"my information a = "<<2;
    logger.getInfoSink()<<"my information b = "<<3;

This results in the following message:

.. code-block:: bash

    INF my information a = 2
    INF my information b = 3

if the
`Klubok`_
library is not available. Otherwise with multithreading support
the messages include a thread id.

.. code-block:: bash

    INF TH7f4ca0aa4700: my information a = 2
    INF TH7f4ca0aa4700: my information b = 3

In this case it is however more reasonable to use *CoutThreadSink*,
which uses a mutex to avoid message overlaying. In a similar way
the other sinks can be used. There are several sink types
available for use (threaded versions are available only
if 
`Klubok`_
is installed):

**CoutSink**
    Output to the standard output.

**CerrSink**
    Output to the standard error output.

**StringSink**
    Output to a string buffer.

**FileSink**
    Output to a file.

**SharedStringSink**
    Output to a string buffer shared by several sinks.

**CoutThreadSink**
    Output to the standard output with thread safe locking (mutexing).

**CerrThreadSink**
    Output to the standard error output with thread safe locking (mutexing).

**StringThreadSink**
    Output to a string buffer with thread safe locking (mutexing).
    This provides only locking for the same sink, e.g. if every
    sink level uses own buffer.

**SharedStringThreadSink**
    Output to a shared string buffer with thread safe locking (mutexing), e.g.
    if the same buffer is used for all logging levels.
    
**FileThreadSink**
    Output to a file with thread safe locking (mutexing).
    This provides only locking for the same sink, e.g. if every
    sink level uses own buffer.

**SharedFileThreadSink**
    Output to a shared file with thread safe locking (mutexing), e.g.
    if the same file is used for all logging levels. This file sink
    also support a size limit on the log file.

Details
=======
*Logger* is a singleton class. A static instance of the class is created 
on the first call to *getLogger* method. A getting sink call
*getXSink* returns an instance of a slim temporary object *StreamBuffer*.
This instance implements stream operator and exists only inside
a single expression, i.e. a single chain of stream operations.
If a final sink is available for the desired level of urgency,
a *StreamBuffer* saves all chained calls into a single message in
a buffer. On instance destruction the buffer is redirected to the sink.
The sink locking (mutexing) happens only during this last stage.
If no sink is available, the only operation performed in the
stream operator is an *if*-statement check like *if (sink)*.
The latter is the performance penalty with a null sink, whereas
with a non-null sink the actual output would overweight the checks.

Building
========

To build the package `CMake`_ build system is necessary.
The build system detects if required libraries/components are present and
if not it disables building the affected parts of the package,
:ref:`requirements`.

With `CMake`_ it is preferable to use out-of-source build.
For that inside the top level package directory::

    mkdir build && cd build

From the build directory call the `CMake`_ with the path to the top-level
directory, which is in our case::

    cmake ..

To build all parts with the exception of documentation::

    make

To install the library using the administrator rights::

    make install

To build the documentation::

    make doc-html
    make doc-pdf

.. _requirements:

Requirements
============
The dependencies of the package components are:

    #.  Multithreading support.
        `Klubok`_
    #. Test suite for library and service
        `Boost.Test`_
    #. Documentation
        `Doxygen`_, `Sphinx`_, `breathe`_, `Latex`_,

API
===
For API please refer to `here <doxy/index.html>`_.
