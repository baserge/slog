// Licensed under GPLv3 or later, see the COPYING file.
#ifndef HEADER_H_UPCEAWJT
#define HEADER_H_UPCEAWJT
#include <string>
#include <iostream>

namespace slog
{
    // =========================================================================
    /// @brief Class to print log message header, containing log level,
    // logger name, thread, time, etc.
    // =========================================================================
    class Header
    {
        public:
            Header (const std::string &level, const std::string &name,
                    bool writeThread, bool writeTime) :
                logLevel(level), logName(name), writeThread(writeThread),
                writeTime(writeTime) {};
            ~Header () {};

            const std::string &getLogLevel() const {return logLevel;};
            const std::string &getLogName() const {return logName;};
            bool getWriteThread() const {return writeThread;};
            bool getWriteTime() const {return writeTime;};

            void setLogLevel(const std::string &v) {logLevel = v;};
            void setLogName(const std::string &v) {logName = v;};
            void setWriteThread(bool v) {writeThread = v;};
            void setWriteTime(bool v) {writeTime = v;};

        private:
            std::string logLevel;
            std::string logName;
            bool writeThread;
            bool writeTime;
            friend std::ostream& operator<<(std::ostream& strm,
                                            const Header& inst);
    };
} //namespace slog
#endif /* end of include guard: HEADER_H_UPCEAWJT */
