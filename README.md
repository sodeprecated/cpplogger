# cpplogger
### Announcements:
#### Release 1.0.0
[Release 1.0.0](https://github.com/MrDanikus/cpplogger/releases/tag/release-1.0.0) is now available.

## Welcome to cpplogger!

__cpplogger__ will bring new colors to your life! Make simple console output more exciting just in a few lines of code.

### Getting started:

#### Namespace:
* __cpplogger__ works in `logger::` namespace

#### Types:
* `logger::error` structure inherited from `std::exception` that holds all errors and has own stack for easier tracing with `Trace` macro.
* `logger::log_message_type` enum with common log types.
* `logger::kModifier` enum with modifiers that you could apply to your console output. See more about this in example section.
* `logger::style` typedef for `std::vector<kModifier>` with overloaded `operator<<`.
#### Functions:
* `logger::BindConsoleStyle(s, args...)` (__args must be instances of `logger::kModifier`__) creates a new `logger::style` with name `s` and modifiers `args...`. Returns `true` if new style was successfully created. More information about styles in example section. 
* `logger::BindLogDirectory(s)` this function redefine default(project __working__ directory) logging directory to `s` (__`s` must be a valid path__, doesn't matter relative or full).
#### Macros:
* `ConsoleLog(s, args...)` takes string as first argument (See in description below more about parsing rules), then takes variable that has overloaded `operator<<` and put them instead of `%v` in string. `logger:: kModifier` in argument list is __undefined behaviour__. Returns `true` if everything OK with console output.
* `Trace(x)` takes `logger::error` as argument and push to error's stack current filepath, function name and line where `Trace` was called. Returns `logger::error`.
* `FileLog(type, args...)` takes `logger::log_message_type` as first argument. Creates folders in format `logs/{year}/{month}/ddmmyyyy.log` and outputs(in specific format) all variables provided to the function(new line for each variable).
* `DEBUG_ONLY` disables file and console output. Type `#define DEBUG_ONLY` before(!) including cpplogger files.
* `OS_WIN`/`OS_UNIX` determines current working system.


#### Platforms:
+ Windows
+ MacOs
+ Linux

### Example:
#### Parsing rules:

##### Variables
- `%v` Variables and modifiers are put instead of ‘%v’ in the order that they are specified in arguments list. Variable must has overloaded `operator<<`.
##### Date/Time

- `%h` puts current hour in format of two digits e.g 00,05,13
- `%m` puts current minute in format of two digits e.g 00,10,59
- `%s` puts current second in format of two digits e.g 00,10,59
- `%dd` puts day in format of two digits e.g. 01,02,12
- `%mm` puts month in format of two digits e.g. 01,02,12 
- `%yy` puts year in format of two digits e.g. 19,20,21
- `%yyyy` puts day in format of four digits e.g. 2019,2020,2021

##### Macros

- `%FILE` puts current filename (!without path)

- `%FUNC` puts current function name

- `%LINE` puts current line in source file

- `%PATH` puts full path to the current file including filename

- `%%` puts `%`


##### Class
- `%.ClassName( ... %)` Apply `logger::style` to the string enclosed between `%.ClassName(` and `%)`. Supports nested classes. Class name shouldn't contain `%` because of undefined behaviour. Classes without close bracket `%)` are __undefined behaviour__.

__All other symbols are put as is!__

#### Code example:

````C++
#include <iostream>
#include "log.hpp"

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

````
if we compile (considering that you have `./boo` folder and `log.hpp` file in your directory) this code with `g++ main.cpp -o main -std="c++17"` and then type `main #./main for unix systems` to the console we will get:

![](https://github.com/MrDanikus/cpplogger/raw/master/image/console-sample-output.png "console-sample-output")

In `./boo` folder will appear file structure with format `logs/{year}/{month}/` where you can find `.log` file similar to this:

![](https://github.com/MrDanikus/cpplogger/raw/master/image/log-sample-output.png "log-sample-output")

