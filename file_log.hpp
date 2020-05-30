//MIT License
//
//Copyright (c) 2020 MrDanikus
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#ifndef FILE_LOG_HPP
#define FILE_LOG_HPP

#include <cstddef>          // size_t
#include <utility>          // std::move
#include <string>           // std::string, std::to_string
#include <queue>            // std::queue
#include <fstream>          // std::ofstream
#include <time.h>           // time, localtime
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>




// Part of log.hpp file that is needed to proper
// work of file log

#ifndef LOG_HPP

#include <exception>        // std::exception
#include <stack>            // std::stack
#include <sstream>          // std::stringstream

namespace logger {
    
    // @struct error
    //
    //
    // @member message - std::string : string containing information about the error
    //
    // @constructor error(const char*) : construct object from const C-string
    // @constructor error(std::string) : construct object from std::string
    //
    //
    // @method what()
    //     @return const char*
    //
    //     return null terminating string containing information about the error
    //
    //
    // structure for holding errors that ConsoleLog throws
    
    struct error : std::exception {
        
        error(){}
        error(const char* s)    : message_(s) {}
        error(std::string s)  : message_(std::move(s)) {}
        
        virtual const char* what() const noexcept override { return message_.c_str(); }
        
        void push(const char* path, const char* func, int line) {
            error_stack_.push(std::string(path) + ':' + std::string(func) + ':' + std::to_string(line));
        }
        
        std::stack<std::string> error_stack_;
        std::string             message_;
        
    };
    
    
    
    
    // @function StrToLen(a,b,c)
    //
    //
    // @param a - std::string&& : target string
    // @param b - size_t        : target length
    // @param c - char          : fill character
    //
    // @return std::string
    //
    //
    // move string @a and add (@b - Length(@a)) symbols that equals @c to the front
    // if Length(@a) is at least @b, return @a without changes
    
    std::string StrToLen(std::string&&, size_t, char);
    
    
    
    
    // @function ProcessVars(queue)
    //
    //
    // @param queue - std::queue<std::string>   : queue with processed variables
    //
    // @return void
    //
    // Does nothing
    
    void ProcessVars(std::queue<std::string>&);
    
    
    
    
    
    // template<T>
    // @function ProcessVars(queue,var)
    //
    //
    // @param queue - std::queue<std::string>   : queue with processed variables
    // @param var   - T                         : current processed value
    //
    // @return void
    //
    // create a stringstream and pass @var to it
    // then push string to the @queue
    
    template <class T>
    void ProcessVars(std::queue<std::string>&, T);
    
    
    
    
    
    // template<T>
    // @function ProcessVars(queue,var,args)
    //
    //
    // @param queue - std::queue<std::string>   : queue with processed variables
    // @param var   - T                         : current processed value
    // @param args  - pack                      : variables
    //
    // @return void
    //
    // create a stringstream and pass @var to it
    // then push string to the @queue
    // pass args recursively
    
    template <class T, class ...Args>
    void ProcessVars(std::queue<std::string>&, T, Args...);
    
}




// @Implementation of
//  logger::StrToLen

std::string logger::StrToLen(std::string &&s, size_t len, char fill) {
    
    if(s.length() >= len) return std::move(s);
    
    s.insert(0,len - s.length(),fill);
    
    return std::move(s);
    
}




// @Implementation of
//  logger::ProcessVars

void logger::ProcessVars(std::queue<std::string>& queue) {
    
}




// @Implementation of
//  logger::ProcessVars

template <class T>
void logger::ProcessVars(std::queue<std::string>& queue, T var) {
    
    std::stringstream ss;
    
    ss << var;
    
    queue.push(ss.str()); // copy elision
    
}




// @Implementation of
//  logger::ProcessVars

template <class T, class ...Args>
void logger::ProcessVars(std::queue<std::string>& queue, T var, Args... args) {
    
    std::stringstream ss;
    
    ss << var;
    
    queue.push(ss.str()); // copy elision
    
    ProcessVars(queue,args...);
    
}

#endif










#include "log_message_types.hpp"



namespace logger {
    
    // @member log_directory_
    //
    // string with path to logging directory
    
    std::string log_directory_;
    
    
    
    
    // @function BindLogDirectory
    //
    //
    // @param s - const char* : null terminated string, relative or full path to log directory
    //
    // @return void
    //
    //
    // Set the logging directory to @s
    
    void BindLogDirectory(const char*);
    
    
    
    
    // @function FileLog(default args, s, args)
    //
    //
    // @param default args                              : set of arguments that define macro information
    // @param TYPE          - logger::log_message_type  : type of message
    // @param args          - pack                      : variables that will be logged
    //
    // @return bool
    //
    // @throw logger::error
    //
    //
    // Put every arg in new line of log file
    // if some error occurs, throws logger::error with information about error
    // return true if everything ok and false if there were errors with console output
    
    template <class ...Args>
    bool FileLog(const char*, const char*, int, const char*, log_message_type, Args...);
    
    
    
    
    // @function FileLog(error)
    //
    //
    // @param error - const logger::error& : error
    //
    // @return bool
    //
    // @throw logger::error
    //
    //
    // output error's stack to the log file
    // if some error occurs, throws logger::error with information about error
    // return true if everything ok and false if there were errors with file output
    
    bool FileLog(const char*, const char*, int, const char*, error&);
    
}




// @Implementation of
//  logger::BindLogFile

void logger::BindLogDirectory(const char* s) {
    
    struct stat st = {0};   // structure for holding stat
    
    if (stat(s, &st) == -1) {
        throw logger::error("path is invalid");
    }
    
    logger::log_directory_ = s;
}




// @Implementation of
//  logger::FileLog

template <class ...Args>
bool logger::FileLog(const char* PATH, const char* FILENAME, int LINE, const char* FUNC, logger::log_message_type TYPE, Args... args) {
    
    static const char* month[] = {
        "january",
        "february",
        "march",
        "april",
        "may",
        "june",
        "july",
        "august",
        "september",
        "october",
        "november",
        "december"
    };
    
    static time_t               the_time  = time(NULL);             // static time object
    struct tm                   *cur_time = localtime(&the_time);   // current time object
    struct stat st                        = {0};                    // structure for holding stat
    
    
    std::string directory       = logger::log_directory_ + "logs";
    std::string year_directory  = directory + '/' + std::to_string(cur_time->tm_year + 1900);
    std::string month_directory = year_directory + '/' + month[cur_time->tm_mon];
    std::string filename        =  month_directory + '/' +
                                   StrToLen(std::to_string(cur_time->tm_mday),2,'0') +
                                   StrToLen(std::to_string(cur_time->tm_mon + 1),2,'0') +
                                   std::to_string(cur_time->tm_year + 1900) + ".log";
    
    
    std::queue<std::string> queue;  // queue with variable converted to string
    
    
    int res;
    
    if (stat(directory.c_str(), &st) == -1) {
        res = mkdir(directory.c_str(), 0700);
        if(res) {
            throw logger::error("cannot create directory");
        }
    }
    
    if (stat(year_directory.c_str(), &st) == -1) {
        res = mkdir(year_directory.c_str(), 0700);
        if(res) {
            throw logger::error("cannot create directory");
        }
    }
    
    if (stat(month_directory.c_str(), &st) == -1) {
        res = mkdir(month_directory.c_str(), 0700);
        if(res) {
            throw logger::error("cannot create directory");
        }
    }
    
    
    std::ofstream file_out(filename, std::ios::app);
    
    if(!file_out.is_open()) {
        throw logger::error("cannot open file");
    }
    
    
    std::string log_type;
    
    switch (TYPE) {
        case logger::T_INFO:
            log_type = "INFO";
            break;
        case logger::T_DEBUG:
            log_type = "DEBUG";
            break;
        case logger::T_ERROR:
            log_type = "ERROR";
            break;
        case logger::T_WARNING:
            log_type = "WARNING";
            break;
        case logger::T_CRITICAL:
            log_type = "CRITICAL";
            break;
        default:
            log_type = "INFO";
            break;
    }
    
    
    ProcessVars(queue, args...);    // convert vars to string and push them to the queue
    
    
    while(!queue.empty()) {
        
        std::string cur = queue.front();
        
        file_out << std::to_string(cur_time->tm_year + 1900) << '-' << StrToLen(std::to_string(cur_time->tm_mon + 1),2,'0') << '-' << StrToLen(std::to_string(cur_time->tm_mday),2,'0') << ' '; // date
        file_out << StrToLen(std::to_string(cur_time->tm_hour),2,'0') << ':' << StrToLen(std::to_string(cur_time->tm_min),2,'0') << ':' << StrToLen(std::to_string(cur_time->tm_sec),2,'0') << ' '; // time
        
        file_out << '[' << log_type << "] "; // message type
        
        file_out << FILENAME << ':' << LINE << ' ' << FUNC << " -> "; // info
        
        
        
        file_out << cur << '\n';
        
        queue.pop();
    }
    
    
    
    return file_out.good();
    
}




// @Implementation of
//  logger::FileLog

bool logger::FileLog(const char* PATH, const char* FILENAME, int LINE, const char* FUNC, logger::error& error) {
    
    static const char* month[] = {
        "january",
        "february",
        "march",
        "april",
        "may",
        "june",
        "july",
        "august",
        "september",
        "october",
        "november",
        "december"
    };
    
    static time_t               the_time  = time(NULL);             // static time object
    struct tm                   *cur_time = localtime(&the_time);   // current time object
    struct stat st                        = {0};                    // structure for holding stat
    
    
    std::string directory       = logger::log_directory_ + "logs";
    std::string year_directory  = directory + '/' + std::to_string(cur_time->tm_year + 1900);
    std::string month_directory = year_directory + '/' + month[cur_time->tm_mon];
    std::string filename        =  month_directory + '/' +
    StrToLen(std::to_string(cur_time->tm_mday),2,'0') +
    StrToLen(std::to_string(cur_time->tm_mon + 1),2,'0') +
    std::to_string(cur_time->tm_year + 1900) + ".log";
    
    
    int res;
    
    if (stat(directory.c_str(), &st) == -1) {
        res = mkdir(directory.c_str(), 0700);
        if(res) {
            throw logger::error("cannot create directory");
        }
    }
    
    if (stat(year_directory.c_str(), &st) == -1) {
        res = mkdir(year_directory.c_str(), 0700);
        if(res) {
            throw logger::error("cannot create directory");
        }
    }
    
    if (stat(month_directory.c_str(), &st) == -1) {
        res = mkdir(month_directory.c_str(), 0700);
        if(res) {
            throw logger::error("cannot create directory");
        }
    }
    
    
    std::ofstream file_out(filename, std::ios::app);
    
    if(!file_out.is_open()) {
        throw logger::error("cannot open file");
    }
    
    
    std::string log_type = "ERROR";
    
    file_out << std::to_string(cur_time->tm_year + 1900) << '-' << StrToLen(std::to_string(cur_time->tm_mon + 1),2,'0') << '-' << StrToLen(std::to_string(cur_time->tm_mday),2,'0') << ' '; // date
    file_out << StrToLen(std::to_string(cur_time->tm_hour),2,'0') << ':' << StrToLen(std::to_string(cur_time->tm_min),2,'0') << ':' << StrToLen(std::to_string(cur_time->tm_sec),2,'0') << ' '; // time
    
    file_out << '[' << log_type << "] "; // message type
    
    file_out << FILENAME << ':' << LINE << ' ' << FUNC << " -> "; // info
    file_out << '\"' <<  error.what() << "\" error stack : \n";
    
    while(!error.error_stack_.empty()) {
        
        file_out << '\t' << error.error_stack_.top() << '\n';
        
        error.error_stack_.pop();
    }
    
    
    
    return file_out.good();
    
};



// Macro that pass to the logger::FileLog additional info about place where it has been called
#define FileLog(...) logger::FileLog(__FILE__,__FILENAME__,__LINE__,__func__ ,__VA_ARGS__)

#endif /* FILE_LOG_HPP */
