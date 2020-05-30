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

#ifndef LOG_HPP
#define LOG_HPP


#include <cstddef>          // size_t
#include <utility>          // std::move
#include <string>           // std::string, std::to_string
#include <exception>        // std::exception
#include <vector>           // std::vector
#include <stack>            // std::stack
#include <queue>            // std::queue
#include <iostream>         // std::cout
#include <time.h>           // time, localtime
#include <sstream>          // std::stringstream
#include <unordered_map>    // std::unordered_map

#include "console_modifiers.hpp" // kModifier


#define __FILENAME__ (strrchr("/" __FILE__, '/') + 1)

namespace logger {
   
#ifndef FILE_LOG_HPP
    
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
    
#endif
    
    
    
    
    
    // @struct style
    //
    //
    // @constructor style(std::initializer_list<kModifier>) : construct vector of modifiers with initializer list
    // @constructor style()                                 : default constructor
    //
    // @member modifiers - std::vector of kModifiers representing the set of modifiers that will be used with this style
    //
    //
    // structure for holding modifiers that applied to elements with this style
    
    struct style {
        std::vector<kModifier> modifiers;
        
        style(std::initializer_list<kModifier> il) : modifiers(il) {};
        style(){};
    };
    
    
    
    
    
    // @member binded_styles_
    //
    // hash table that contains correspondence between styles and it names
    
    std::unordered_map<std::string, style> binded_styles;
    
    
    
    
    
    // @function BindConsoleStyle(s,...)
    //
    //
    // @param s    - std::string : name of style (should be unique)
    // @param ...  - modifiers that will be applied to that style
    //
    // @return bool
    //
    // set correspondence between style and it name and return true if everything fine and false otherwise
    
    template <class ...Args>
    bool BindConsoleStyle(std::string, Args...);
    
    
    
    
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
    // Creates new error
    // push to error's stack information about current place
    // and then return error
    
    error Trace(error&&, const char*, const char*, int);
    
    
    
    
    
#ifndef FILE_LOG_HPP
    
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
    
#endif
    
    
    
    
    
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
    bool ConsoleLog(const char*, const char*, int, const char*, const std::string&, Args...);
    
    
    
    
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
    
}




// @Implementation of
//  logger::BindConsoleStyle

template <class ...Args>
bool logger::BindConsoleStyle(std::string s, Args... args) {
    
    return binded_styles.emplace(std::make_pair(std::move(s), style({args...}))).second;
    
}




// @Implementation of
//  logger::Trace

logger::error& logger::Trace(logger::error& error, const char* PATH, const char* FUNC, int LINE) {
    
    error.push(PATH, FUNC, LINE);
    
    return error;
    
}




// @Implementation of
//  logger::Trace

logger::error logger::Trace(logger::error&& error, const char* PATH, const char* FUNC, int LINE) {
    
    error.push(PATH, FUNC, LINE);
    
    return error;
    
}




#ifndef FILE_LOG_HPP

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




// @Implementation of
//  logger::ConsoleLog

template <class ...Args>
bool logger::ConsoleLog(const char* PATH, const char* FILENAME, int LINE, const char* FUNC, const std::string& s, Args... args) {
    
    static time_t               the_time  = time(NULL);             // static time object
    struct tm                   *cur_time = localtime(&the_time);   // current time object
    size_t                      n         = s.length();             // length of string
    size_t                      prev_it   = 0;                      // start pos of last substring without commands
    
    
    std::queue<std::string>     queue;                              // queue with variable converted to string
    std::stack<logger::style*>  modifier_stack;                     // stack with active modifiers
    static logger::style        default_style({logger::RESET});     // default style is no-style
    
    
    modifier_stack.push(&default_style);
    
    ProcessVars(queue, args...);    // convert vars to string and push them to the queue
    

    
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
                    result_ss << style.modifiers;
                    
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
                result_ss << modifier_stack.top()->modifiers;
                
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
    
    std::cout << logger::FG_WHITE << logger::BG_RED << "[ERROR]" << logger::RESET << " error message : \"" << logger::FG_RED << error.what() << logger::RESET <<  "\" error stack :\n" ;
    while(!error.error_stack_.empty()) {
        std::cout << '\t' << logger::UNDERLINE <<  error.error_stack_.top() << logger::UNDERLINE_OFF << "\n";
        error.error_stack_.pop();
    }
    
    return std::cout.good();
    
};




// Macro that pass to the logger::ConsoleLog additional info about place where it has been called
#define ConsoleLog(...) logger::ConsoleLog(__FILE__,__FILENAME__,__LINE__,__func__ ,__VA_ARGS__)

// Macro that pass to the logger::Trace additional info about place where it has been called
#define Trace(x) logger::Trace(x,__FILE__,__func__,__LINE__)


#endif /* LOG_HPP */
