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

#ifndef LOG_CONSOLE_MODIFIERS_HPP
#define LOG_CONSOLE_MODIFIERS_HPP

#include <sstream>  // std::stringstream
#include <vector>   // std::vector

namespace logger {
    
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
    
};

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

#endif /* LOG_CONSOLE_MODIFIERS_HPP */
