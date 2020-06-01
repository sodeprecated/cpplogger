#include <iostream>
#include "../release/log.hpp"

class Class {
    int x_;
public:
    Class(int x) : x_(x) {}
    friend std::ostream& operator <<(std::ostream& os, const Class& a) {
        os << "World " << a.x_;
        return os;
    }
};

void FunctionThatTrowsError() {
    throw Trace(logger::error("error occur"));
}

int main() {
    
    logger::BindConsoleStyle("Foo", logger::BG_WHITE, logger::FG_RED, logger::BOLD);
    
    logger::BindConsoleStyle("Blink", logger::SLOW_BLINK);
    
    logger::BindLogDirectory("./boo/");
    
    try {
        FunctionThatTrowsError();
    } catch (logger::error& e) {
        Trace(e);
        FileLog(e);
    }
    
    ConsoleLog("%FILE:%FUNC:%LINE %.Foo([%dd.%mm.%yy - %h:%m:%s]%) -> %.Blink(%v %v!%)","Hello",Class(1));
    
    FileLog(logger::T_WARNING,Class(2),"some warning");
    
    return 0;
}
