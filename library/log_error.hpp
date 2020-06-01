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

#ifndef LOG_ERROR_HPP
#define LOG_ERROR_HPP

#include <utility>          // std::move
#include <exception>        // std::exception
#include <string>           // std::string
#include <stack>            // std::stack

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
    
}

#endif /* LOG_ERROR_HPP */
