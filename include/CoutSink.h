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
            virtual std::mutex* getMutex() {return nullptr;};
            virtual ostream &getStream() {return cout;};
    };
} //namespace slog

#endif /* end of include guard: COUTSINK_H_LHCHBLUO */
