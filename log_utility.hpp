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

#ifndef LOG_UTILITY_HPP
#define LOG_UTILITY_HPP

#include <cstddef>          // size_t
#include <utility>          // std::move
#include <string>           // std::string, std::to_string
#include <queue>            // std::queue
#include <sstream>          // std::stringstream

namespace logger {
    
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

#endif /* LOG_UTILITY_HPP */
