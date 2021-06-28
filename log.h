/*
MIT License

Copyright (c) 2021 Bastian Jäger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <future>
#include <map>
#include <mutex>
#include <math.h>
#include <string.h>
#include <sys/time.h>

namespace SimpleLog {

const std::string LOG_TEXT_DEBUG2 = "[DEBUG2]";
const std::string LOG_TEXT_DEBUG  = "[DEBUG]";
const std::string LOG_TEXT_INFO   = "[INFO] ";
const std::string LOG_TEXT_WARN   = "[WARN] ";
const std::string LOG_TEXT_ERROR  = "[ERROR]";

const std::string LOG_COLOR_DEBUG = "\033[1;32m";
const std::string LOG_COLOR_INFO  = "\033[1;34m";
const std::string LOG_COLOR_WARN  = "\033[1;33m";
const std::string LOG_COLOR_ERROR = "\033[1;31m";
const std::string LOG_COLOR_END   = "\033[0m";

enum LOG_LEVEL {
  DEBUG2= 0,
  DEBUG = 1,
  INFO  = 2,
  WARN  = 3,
  ERROR = 4,
};

class SimpleLogger {
private:
  SimpleLogger(const std::string &_name, LOG_LEVEL _verbosity, bool _doColoring = true, bool _logToConsole = true, bool _logToFile = false) {
    __LOG_NAME = _name;
    __LOG_VERBOSITY = _verbosity;
    __DO_COLORING = _doColoring;
    __LOG_TO_CONSOLE = _logToConsole;
    __LOG_TO_FILE = _logToFile;
  }

  static std::shared_ptr<SimpleLogger> createInstance(SimpleLogger* _t = nullptr) {
    static std::shared_ptr<SimpleLogger> myInstance;
    if (_t) {
      myInstance = std::shared_ptr<SimpleLogger>(_t);
    }
    return myInstance;
  }

public:
  static std::shared_ptr<SimpleLogger> getInstance() {
    return createInstance();
  }
  static void Init(const std::string &_name = "SimpleLogger", LOG_LEVEL _verbosity = DEBUG, bool _doColoring = true, bool _logToConsole = true, bool _logToFile = false) {
    auto t = new SimpleLogger(_name, _verbosity, _doColoring, _logToConsole, _logToFile);
    createInstance(t);
  }
  static void verbosity(LOG_LEVEL _verbosity) {
    getInstance()->__LOG_VERBOSITY = _verbosity;
  }
  static LOG_LEVEL verbosity() { return getInstance()->__LOG_VERBOSITY; }

  static std::string name() { return getInstance()->__LOG_NAME; }
  static bool doColoring() { return getInstance()->__DO_COLORING; }
  static bool logToConsole() { return getInstance()->__LOG_TO_CONSOLE; }
  static bool logToFile() { return getInstance()->__LOG_TO_FILE; }

  static std::map<LOG_LEVEL, std::string> loglevel_str() {
    std::map<LOG_LEVEL, std::string> m;

    m[DEBUG2] = LOG_TEXT_DEBUG2;
    m[DEBUG] = LOG_TEXT_DEBUG;
    m[INFO] = LOG_TEXT_INFO;
    m[WARN] = LOG_TEXT_WARN;
    m[ERROR] = LOG_TEXT_ERROR;

    return m;
  }
  static std::map<LOG_LEVEL, std::string> loglevel_color() {
    std::map<LOG_LEVEL, std::string> m;

    m[DEBUG2] = LOG_COLOR_DEBUG;
    m[DEBUG] = LOG_COLOR_DEBUG;
    m[INFO] = LOG_COLOR_INFO;
    m[WARN] = LOG_COLOR_WARN;
    m[ERROR] = LOG_COLOR_ERROR;

    return m;
  }

  static std::string formattedDate() {
    time_t t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::stringstream formattedDateSs;
    formattedDateSs << std::put_time(&tm, "%Y-%m-%d");

    return formattedDateSs.str();
  }
  static std::string formattedDateTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm * tm_info = localtime(&tv.tv_sec);

    char buffer[28];
    if ( __SHOW_DATE ) {
      sprintf(buffer, "%04d-%02d-%02dT%02d:%02d:%02d",
              1900 + tm_info->tm_year,
              tm_info->tm_mon,
              tm_info->tm_mday,
              tm_info->tm_hour,
              tm_info->tm_min,
              tm_info->tm_sec);
    }
    else {
      sprintf(buffer, "%02d:%02d:%02d",
              tm_info->tm_hour,
              tm_info->tm_min,
              tm_info->tm_sec);
    }

    if ( __SHOW_MILLISECONDS ) { // add milliseconds
      int millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
      if (millisec>=1000) { // Allow for rounding up to nearest second
        millisec -=1000;
        tv.tv_sec++;
      }
      strcat(buffer, ".");
      strcat(buffer, std::to_string(millisec).c_str());
    }

    if ( __SHOW_TIMEZONE ) { // add timezone offset
      char buffer_tz[8];
      strftime(buffer_tz, 7 , "%z", tm_info);
      strcat(buffer, buffer_tz);
    }

    return std::string(buffer);
  }

private:
  std::string __LOG_NAME = "log";
  LOG_LEVEL __LOG_VERBOSITY = LOG_LEVEL::DEBUG;

  bool __DO_COLORING;
  bool __LOG_TO_CONSOLE;
  bool __LOG_TO_FILE;

  static constexpr bool __SHOW_DATE = true;
  static constexpr bool __SHOW_TIMEZONE = false;
  static constexpr bool __SHOW_MILLISECONDS = true;

private:
  static std::shared_ptr<SimpleLogger> instance_;
};

static std::mutex __LOG_MUTEX;

#define _CLOG std::cout
//#define _CLOG std::ostream(0)
#define _FLOG std::ofstream(SimpleLogger::name() + "-" + SimpleLogger::formattedDate() + ".log", std::ios::out | std::ios::app)
//#define _FLOG ostream(0)

#define LOG(level, msg) { \
  if ( !SimpleLog::SimpleLogger::getInstance() ) { \
    SimpleLog::SimpleLogger::Init(); \
  } \
  if ( level >= SimpleLog::SimpleLogger::verbosity() ) { \
    std::stringstream logSs; \
    logSs << msg; \
    auto f = std::async([](LOG_LEVEL _l, std::string _m){ \
      std::lock_guard<std::mutex> lockGuard(__LOG_MUTEX); \
      if ( SimpleLog::SimpleLogger::logToConsole() ) { \
        if ( SimpleLog::SimpleLogger::doColoring() ) { \
          _CLOG << SimpleLog::SimpleLogger::loglevel_color()[_l]; \
        } \
        _CLOG << SimpleLog::SimpleLogger::formattedDateTime() << " " << SimpleLog::SimpleLogger::loglevel_str()[_l] << " "; \
        if ( SimpleLog::SimpleLogger::doColoring() ) { \
          _CLOG << LOG_COLOR_END; \
        } \
        _CLOG << _m << std::endl; \
      } \
      if ( SimpleLog::SimpleLogger::logToFile() ) { \
        if ( SimpleLog::SimpleLogger::doColoring() ) { \
          _FLOG << SimpleLog::SimpleLogger::loglevel_color()[_l]; \
        } \
        _FLOG << SimpleLog::SimpleLogger::formattedDateTime() << " " << SimpleLog::SimpleLogger::loglevel_str()[_l] << " "; \
        if ( SimpleLog::SimpleLogger::doColoring() ) { \
          _FLOG << LOG_COLOR_END; \
        } \
        _FLOG << _m << std::endl; \
      } \
    }, level, logSs.str()); \
  } \
}

inline void log_init(const std::string &_name, LOG_LEVEL _verbosity = DEBUG, bool _doColoring = true, bool _logToConsole = false, bool _logToFile = false) {
  SimpleLogger::Init(_name, _verbosity, _doColoring, _logToConsole, _logToFile);

  LOG(INFO, "SimpleLog initialized with LOG_NAME='" << SimpleLogger::name()
            << "' and VERBOSITY=" << SimpleLogger::verbosity() << ".");
}
inline void log_verbosity(LOG_LEVEL _verbosity) {
  SimpleLogger::verbosity(_verbosity);
}

} // SimpleLog

using namespace SimpleLog;
