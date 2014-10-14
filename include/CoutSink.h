// CoutSink.h - CoutSink class, part of Slog.
// Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
// Licensed under GPLv3 or later, see the COPYING file.
#ifndef COUTSINK_H_LHCHBLUO
#define COUTSINK_H_LHCHBLUO
#include <iostream>
namespace slog
{
    using std::cout;
    // =========================================================================
    /// @brief Wraps standard cout output, no threading support.
    // =========================================================================
    class CoutSink : public Sink
    {
        public:
            virtual Sink *clone() const {return new CoutSink;};
        protected:
            virtual ostream &lockStream(){return cout;}
            virtual void releaseStream() {};
    
    };
} //namespace slog

#endif /* end of include guard: COUTSINK_H_LHCHBLUO */
