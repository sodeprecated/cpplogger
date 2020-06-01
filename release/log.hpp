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

// This is full log library in one file ready to use
// Just include it

#ifndef LOG_HPP
#define LOG_HPP

#include <string.h>                     // strrchr
#include <time.h>                       // time, localtime
#include <cstddef>                      // size_t
#include <exception>                    // std::exception
#include <utility>                      // std::move
#include <string>                       // std::string, std::to_string
#include <vector>                       // std::vector
#include <stack>                        // std::stack
#include <queue>                        // std::queue
#include <iostream>                     // std::cout
#include <sstream>                      // std::stringstream
#include <unordered_map>                // std::unordered_map
#include <fstream>                      // std::ofstream

#if defined(_WIN32) | defined(_WIN64)
#include <windows.h>                    //  DWORD, HANDLE, GetStdHandle, GetConsoleMode, SetConsoleMode, WIN32_FIND_DATA, HANDLE, CreateDirectory
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define OS_WIN
#else
#include <sys/stat.h>                   // stat
#include <unistd.h>                     // mkdir
#define OS_UNIX
#endif

#define __FILENAME__ (strrchr("/" __FILE__, '/') + 1)

namespace logger {
    
    typedef enum : unsigned char {
        T_WARNING,
        T_DEBUG,
        T_INFO,
        T_ERROR,
        T_CRITICAL
    } log_message_type;
    
    
    
    typedef enum : unsigned char {
        RESET               =       0,
        BOLD                =       1,
        FAINT               =       2,
        ITALIC              =       3,
        UNDERLINE           =       4,
        SLOW_BLINK          =       5,
        RAPID_BLINK         =       6,
        INVERSE             =       7,
        CONCEAL             =       8,
        CROSSED             =       9,
        BOLD_OFF            =      21,
        UNDERLINE_OFF       =      24,
        BLINK_OFF           =      25,
        INVERSE_OFF         =      27,
        REVEAL              =      28,
        CROSSED_OFF         =      29,
        FG_DEFAULT          =      39,
        FG_BLACK            =      30,
        FG_RED              =      31,
        FG_GREEN            =      32,
        FG_YELLOW           =      33,
        FG_BLUE             =      34,
        FG_MAGENTA          =      35,
        FG_CYAN             =      36,
        FG_LIGHT_GRAY       =      37,
        FG_DARK_GRAY        =      90,
        FG_LIGHT_RED        =      91,
        FG_LIGHT_GREEN      =      92,
        FG_LIGHT_YELLOW     =      93,
        FG_LIGHT_BLUE       =      94,
        FG_LIGHT_MAGENTA    =      95,
        FG_LIGHT_CYAN       =      96,
        FG_WHITE            =      97,
        BG_BLACK            =      40,
        BG_RED              =      41,
        BG_GREEN            =      42,
        BG_YELLOW           =      43,
        BG_BLUE             =      44,
        BG_MAGENTA          =      45,
        BG_CYAN             =      46,
        BG_WHITE            =      47,
        BG_DEFAULT          =      49
    } kModifier;
    
    
    
    
    template < class CharT, class Traits >
    std::basic_ostream<CharT, Traits>& operator<<( std::basic_ostream<CharT, Traits>& os, const logger::kModifier& x ) {
        os << "\033[" << (int)x << "m";
        return os;
    }
    
    
    
    
    template < class CharT, class Traits >
    std::basic_ostream<CharT, Traits>& operator<<( std::basic_ostream<CharT, Traits>& os,const std::vector<logger::kModifier>& x ) {
        if(x.size() == 0) return os;
        
        size_t n = x.size();
        
        os << "\033[";
        
        for(size_t i = 0; i < n - 1; ++i) {
            os << (int)x[i] << ';';
        }
        os << (int)x.back() << 'm';
        
        return os;
    }
    
    
    
    
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
    // @method push()
    //      @return void
    //
    //      Pushes information to the error stack
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
    
    
    
    
    // @typedef style
    //
    //
    // structure for holding modifiers that applied to elements with this style
    
    typedef std::vector<kModifier> style;
    
    
    
    
    // @member binded_styles_
    //
    // hash table that contains correspondence between styles and it names
    
    std::unordered_map<std::string, style> binded_styles;
    
    
    
    
    // @member log_directory_
    //
    // string with path to logging directory
    
    std::string log_directory_;
    
    
    
    
    
    // @function BindConsoleStyle(s,...)
    //
    //
    // @param s    - std::string : name of style (should be unique)
    // @param ...  - modifiers that will be applied to that style
    //
    // @return bool
    //
    //
    // set correspondence between style and it name and return true if everything fine and false otherwise
    
    template <class ...Args>
    bool BindConsoleStyle(std::string, Args...);
    
    
    
    
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
    
    
    
    
#ifdef OS_WIN
    // @function EnableWindowsAnsiEscapeSequence()
    //
    //
    // @return void
    //
    // @throw logger::error
    //
    //
    // enables ansi escape sequence on windows platform
    // throws logger::error if there were errors
    // with windows console
    
    void EnableWindowsAnsiEscapeSequence();
#endif
    
    
    
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
    //
    // Does nothing
    
    void ProcessVars(std::queue<std::string>*);
    
    
    
    
    
    // template<T>
    // @function ProcessVars(queue,var)
    //
    //
    // @param queue - std::queue<std::string>   : queue with processed variables
    // @param var   - T                         : current processed value
    //
    // @return void
    //
    //
    // create a stringstream and pass @var to it
    // then push string to the @queue
    
    template <class T>
    void ProcessVars(std::queue<std::string>*, const T&);
    
    
    
    
    
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
    //
    // create a stringstream and pass @var to it
    // then push string to the @queue
    // pass args recursively
    
    template <class T, class ...Args>
    void ProcessVars(std::queue<std::string>*, const T&, const Args&...);
    
    
    
    
    // @function Trace(error, path, func, line)
    //
    //
    // @param error - logger::error& : error to trace
    // @param path  - const char*    : path to current file
    // @param func  - const char*    : function name
    // @param line  - int            : line number
    //
    // @return logger::error&
    //
    //
    // Push to error's stack information about current place
    // and then return error
    
    error& Trace(error&, const char*, const char*, int);
    
    
    
    
    // @function Trace(error, path, func, line)
    //
    //
    // @param error - logger::error&& : error to trace
    // @param path  - const char*    : path to current file
    // @param func  - const char*    : function name
    // @param line  - int            : line number
    //
    // @return logger::error
    //
    //
    // Creates new error
    // push to error's stack information about current place
    // and then return error
    
    error Trace(error&&, const char*, const char*, int);
    
    
    
    
    // @function ConsoleLog(default args, s, args)
    //
    //
    // @param default args                            : set of arguments that define macro information
    // @param s             - std::string/const char* : target string - string that will be parsed
    // @param args          - pack                    : variables that will be put instead of "%v" in target string
    //
    // @return bool
    //
    // @throw logger::error
    //
    //
    // parse string @s in accoring to defined rules and pass it to std::cout(console output).
    // if some error occurs, throws logger::error with information about error
    // return true if everything ok and false if there were errors with console output
    
    template <class ...Args>
    bool ConsoleLog(const char*, const char*, int, const char*, const std::string&, const Args&...);
    
    
    
    
    // @function ConsoleLog(error)
    //
    //
    // @param error - const logger::error& : error
    //
    // @return bool
    //
    // @throw logger::error
    //
    //
    // output error's stack to the console
    // if some error occurs, throws logger::error with information about error
    // return true if everything ok and false if there were errors with console output
    
    bool ConsoleLog(const char*, const char*, int, const char*, error&);
    
    
    
    
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
//  logger::BindConsoleStyle

template <class ...Args>
bool logger::BindConsoleStyle(std::string s, Args... args) {
    
    return binded_styles.emplace(std::make_pair(std::move(s), style({args...}))).second;
    
}




// @Implementation of
//  logger::BindLogFile

void logger::BindLogDirectory(const char* s) {

#ifdef OS_UNIX
    struct stat st = {0};   // structure for holding stat
    
    if (stat(s, &st) == -1) {
        throw logger::error("path is invalid");
    }
#endif // OS_UNIX
    
#ifdef OS_WIN
    WIN32_FIND_DATA data;
    HANDLE hFile = FindFirstFile(s, &data);
    
    if (hFile == INVALID_HANDLE_VALUE) // directory doesn't exist
        throw logger::error("path is invalid");
#endif // OS_WIN
    
    logger::log_directory_ = s;
    
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

void logger::ProcessVars(std::queue<std::string>* queue) {
    
}




// @Implementation of
//  logger::ProcessVars

template <class T>
void logger::ProcessVars(std::queue<std::string>* queue, const T& var) {
    
    std::stringstream ss;
    
    ss << var;
    
    queue->push(ss.str()); // copy elision
    
}




// @Implementation of
//  logger::ProcessVars

template <class T, class ...Args>
void logger::ProcessVars(std::queue<std::string>* queue, const T& var, const Args&... args) {
    
    std::stringstream ss;
    
    ss << var;
    
    queue->push(ss.str()); // copy elision
    
    ProcessVars(queue,args...);
    
}




#ifdef OS_WIN
// @Implementation of
//  logger::EnableWindowsAnsiEscapeSequence

void logger::EnableWindowsAnsiEscapeSequence() {
    
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        throw logger::error("cannot enable ansi escape sequence");
    }
    
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        throw logger::error("cannot enable ansi escape sequence");
    }
    
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        throw logger::error("cannot enable ansi escape sequence");
    }
    
}
#endif




// @Implementation of
//  logger::Trace

logger::error& logger::Trace(logger::error& error, const char* path, const char* func, int line) {
    
    error.push(path, func, line);
    
    return error;
    
}




// @Implementation of
//  logger::Trace

logger::error logger::Trace(logger::error&& error, const char* path, const char* func, int line) {
    
    error.push(path, func, line);
    
    return error;
}




// @Implementation of
//  logger::ConsoleLog

template <class ...Args>
bool logger::ConsoleLog(const char* PATH, const char* FILENAME, int LINE, const char* FUNC, const std::string& s, const Args&... args) {
    
#ifdef OS_WIN
    static bool escape_sequence_enabled = false;
    if(!escape_sequence_enabled) {
        EnableWindowsAnsiEscapeSequence();
        escape_sequence_enabled = true;
    }
#endif
    
    
    static time_t               the_time  = time(NULL);             // static time object
    struct tm                   *cur_time = localtime(&the_time);   // current time object
    size_t                      n         = s.length();             // length of string
    size_t                      prev_it   = 0;                      // start pos of last substring without commands
    
    
    std::queue<std::string>     queue;                              // queue with variable converted to string
    std::stack<logger::style*>  modifier_stack;                     // stack with active modifiers
    static logger::style        default_style({logger::RESET});     // default style is no-style
    
    
    modifier_stack.push(&default_style);
    
    ProcessVars(&queue, args...);    // convert vars to string and push them to the queue
    
    
    
    std::stringstream result_ss;    // stringstream containing output
    
    
    for(size_t i = 0; i < n; ++i) {
        if(s[i] == '%') { // command
            
            if(i + 1 == n) {
                throw logger::error("parse error : empty command");
            }
            
            if(i != prev_it) {
                
                result_ss.write((char*)(s.c_str() + prev_it),sizeof(char) * (i - prev_it));
                //result_ss << s.substr(prev_it,i - prev_it);
            }
            
            if(s[i + 1] == 'y') {         // %yyyy / %yy
                if(i + 2 != n && s[i + 2] == 'y') {
                    if(i + 4 != n && s[i + 3] == s[i + 4] && s[i + 3] == 'y') {
                        // %yyyy
                        result_ss << std::to_string(cur_time->tm_year + 1900);
                        i += 4;
                    } else {
                        // %yy
                        result_ss << StrToLen(std::to_string(cur_time->tm_year % 100),2,'0');
                        i += 2;
                    }
                } else {
                    throw logger::error("no such command : \"%y\"");
                }
                
            } else if(s[i + 1] == 'm') {  // %mm / %m
                if(i + 2 != n && s[i + 2] == 'm') {
                    // %mm
                    result_ss << StrToLen(std::to_string(cur_time->tm_mon + 1),2,'0');
                    i += 2;
                } else {
                    // %m
                    result_ss << StrToLen(std::to_string(cur_time->tm_min),2,'0');
                    i += 1;
                }
            } else if(s[i + 1] == 's') {  // %s
                result_ss << StrToLen(std::to_string(cur_time->tm_sec),2,'0');
                i += 1;
            } else if(s[i + 1] == 'h') {  // %h
                result_ss << StrToLen(std::to_string(cur_time->tm_hour),2,'0');
                i += 1;
            } else if(s[i + 1] == 'd') {  // %dd
                if(i + 2 != n && s[i + 2] == 'd') {
                    result_ss << StrToLen(std::to_string(cur_time->tm_mday),2,'0');
                    i += 2;
                } else {
                    throw logger::error("no such command : \"%d\"");
                }
            } else if(s[i + 1] == 'v') {  // %v
                result_ss << queue.front();
                queue.pop();
                i += 1;
            } else if(s[i + 1] == 'F') {  // %FILE
                if(i + 4 != n && s[i + 2] == 'I' && s[i + 3] == 'L' && s[i + 4] == 'E') {
                    result_ss << FILENAME;
                    i += 4;
                } else if(i + 4 != n && s[i + 2] == 'U' && s[i + 3] == 'N' && s[i + 4] == 'C') {
                    result_ss << FUNC;
                    i += 4;
                } else {
                    throw logger::error("parse error");
                }
            } else if(s[i + 1] == 'P') {  // %PATH
                if(i + 4 != n && s[i + 2] == 'A' && s[i + 3] == 'T' && s[i + 4] == 'H') {
                    result_ss << PATH;
                    i += 4;
                } else {
                    throw logger::error("parse error");
                }
            } else if(s[i + 1] == 'L') {  // %LINE
                if(i + 4 != n && s[i + 2] == 'I' && s[i + 3] == 'N' && s[i + 4] == 'E') {
                    result_ss << LINE;
                    i += 4;
                } else {
                    throw logger::error("parse error");
                }
            } else if(s[i + 1] == '.') {  // %.
                
                size_t class_name_begin = i+2,              // start pos of class name
                class_name_end = std::string::npos;  // end pos of class name
                
                for(size_t j = i + 2; j < n; ++j) {
                    if(s[j] == '(') {
                        class_name_end = j;
                        break;
                    }
                }
                
                if(class_name_end == std::string::npos) {
                    throw logger::error("parse error");
                }
                
                try{
                    // try to find style with such name
                    logger::style &style = logger::binded_styles.at(s.substr(class_name_begin, class_name_end - class_name_begin));
                    
                    // update last active modifier
                    modifier_stack.push(&style);
                    
                    // apply modifier
                    result_ss << style;
                    
                }catch(std::out_of_range e) {
                    throw logger::error(e.what());
                }
                
                i = class_name_end;
                
            } else if(s[i + 1] == ')') {  // %)
                
                if(modifier_stack.size() < 2){
                    throw logger::error("parse error: modifier stack is empty");
                }
                
                modifier_stack.pop();
                
                result_ss << logger::RESET;
                result_ss << *modifier_stack.top();
                
                i += 1;
                
            } else if(s[i + 1] == '%') {  // %%
                result_ss << '%';
                i += 1;
            } else {                      // nothing
                throw logger::error("parse error : empty command");
            }
            
            prev_it = i + 1; // update non-command sequence
        }
    }
    
    if(n != prev_it) {
        result_ss.write((char*)(s.c_str() + prev_it),sizeof(char) * (n - prev_it));
    }
    
    // output stringstream, disable all modifiers and move to next line
    std::cout << std::move(result_ss.str()) << logger::RESET << '\n';
    
    
    return std::cout.good();
    
}




// @Implementation of
//  logger::ConsoleLog

bool logger::ConsoleLog(const char*, const char*, int, const char*, logger::error& error) {
    
#ifdef OS_WIN
    static bool escape_sequence_enabled = false;
    if(!escape_sequence_enabled) {
        EnableWindowsAnsiEscapeSequence();
        escape_sequence_enabled = true;
    }
#endif
    
    std::cout << logger::FG_WHITE << logger::BG_RED << "[ERROR]" << logger::RESET << " error message : \"" << logger::FG_RED << error.what() << logger::RESET <<  "\" error stack :\n" ;
    while(!error.error_stack_.empty()) {
        std::cout << '\t' << logger::UNDERLINE <<  error.error_stack_.top() << logger::UNDERLINE_OFF << "\n";
        error.error_stack_.pop();
    }
    
    return std::cout.good();
    
};




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
    
    
    
#ifdef OS_UNIX
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
#endif // OS_UNIX
    
#ifdef OS_WIN
    if (CreateDirectory(directory.c_str(), NULL))
    {
        // Directory created
    }
    else if (ERROR_ALREADY_EXISTS == GetLastError())
    {
        // Directory already exists
    }
    else
    {
        throw logger::error("cannot create directory");
    }
    
    
    if (CreateDirectory(year_directory.c_str(), NULL))
    {
        // Directory created
    }
    else if (ERROR_ALREADY_EXISTS == GetLastError())
    {
        // Directory already exists
    }
    else
    {
        throw logger::error("cannot create directory");
    }
    
    
    if (CreateDirectory(month_directory.c_str(), NULL))
    {
        // Directory created
    }
    else if (ERROR_ALREADY_EXISTS == GetLastError())
    {
        // Directory already exists
    }
    else
    {
        throw logger::error("cannot create directory");
    }
    
#endif // OS_WIN
    
    
    
    
    
    
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
    
    
    ProcessVars(&queue, args...);    // convert vars to string and push them to the queue
    
    
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
    
    
#ifdef OS_UNIX
    int res;
    
    if (stat(directory.c_str(), &st) == -1) {
        res = mkdir(directory.c_str(), 0700);
        if (res) {
            throw logger::error("cannot create directory");
        }
    }
    
    if (stat(year_directory.c_str(), &st) == -1) {
        res = mkdir(year_directory.c_str(), 0700);
        if (res) {
            throw logger::error("cannot create directory");
        }
    }
    
    if (stat(month_directory.c_str(), &st) == -1) {
        res = mkdir(month_directory.c_str(), 0700);
        if (res) {
            throw logger::error("cannot create directory");
        }
    }
#endif // OS_UNIX
    
#ifdef OS_WIN
    if (CreateDirectory(directory.c_str(), NULL))
    {
        // Directory created
    }
    else if (ERROR_ALREADY_EXISTS == GetLastError())
    {
        // Directory already exists
    }
    else
    {
        throw logger::error("cannot create directory");
    }
    
    
    if (CreateDirectory(year_directory.c_str(), NULL))
    {
        // Directory created
    }
    else if (ERROR_ALREADY_EXISTS == GetLastError())
    {
        // Directory already exists
    }
    else
    {
        throw logger::error("cannot create directory");
    }
    
    
    if (CreateDirectory(month_directory.c_str(), NULL))
    {
        // Directory created
    }
    else if (ERROR_ALREADY_EXISTS == GetLastError())
    {
        // Directory already exists
    }
    else
    {
        throw logger::error("cannot create directory");
    }
    
#endif // OS_WIN
    
    
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
    
}

// Macro that pass to the logger::ConsoleLog additional info about place where it has been called
#define ConsoleLog(...) logger::ConsoleLog(__FILE__,__FILENAME__,__LINE__,__func__ ,__VA_ARGS__)

// Macro that pass to the logger::Trace additional info about place where it has been called
#define Trace(x) logger::Trace(x,__FILE__,__func__,__LINE__)

// Macro that pass to the logger::FileLog additional info about place where it has been called
#define FileLog(...) logger::FileLog(__FILE__,__FILENAME__,__LINE__,__func__ ,__VA_ARGS__)

// DEBUG ONLY mode
#ifdef DEBUG_ONLY
#if defined(DEBUG) | defined(_DEBUG)
#define ConsoleLog(...) NULL
#define FileLog(...) NULL
#endif
#endif


#endif /* LOG_HPP */
