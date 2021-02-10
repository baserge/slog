// CoutThreadSink.cpp - CoutThreadSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#include "CoutThreadSink.h"
using namespace slog;

std::mutex CoutThreadSink::mutex;
