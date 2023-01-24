#pragma once
#include "includes.h"

class Logger : private std::streambuf, public std::ostream {
public:
    Logger() : std::ostream(this) {
        std::tm tm;
        time_t now = time(0);
        localtime_s(&tm, &now);
        cout << "[" << std::put_time(&tm, "%H:%M:%S") << "] ";
    }

    ~Logger() {
        cout << endl;
    }

    friend inline Logger& operator<<(Logger& l, CCPoint p);

private:
    int overflow(int c) override {
        std::cout.put(c);
        return 0;
    }
};

inline Logger& operator<<(Logger& l, CCPoint p) {
    l << "{" << p.x << "," << p.y << "}";
    return l;
}

#define log Logger()

#define HOOK(addr, hook, orig) MH_CreateHook((PVOID)(gd::base + addr), hook, (LPVOID*)&orig)
#define CC_HOOK(symbol, hook, orig) MH_CreateHook((PVOID)((int)GetProcAddress(GetModuleHandle("libcocos2d.dll"), symbol)), hook, (PVOID*)&orig)

// credits to HJFod
#define CCARRAY_FOREACH_BETTER(array, type, name) \
    if (array && array->count()) \
        for (auto [i, name] = std::tuple<size_t, type> {0u, nullptr}; i < array->count() && (obj = (type)array->objectAtIndex(i)); i++)

namespace fs = std::filesystem;

string currentPath() {
    fs::path path{ fs::current_path() };
    string path_string{ path.u8string() };

    return path_string;
}

std::size_t filesInDirCount(fs::path path)
{
    return std::distance(fs::directory_iterator(path), fs::directory_iterator{});
}

size_t framesCount() {
    return filesInDirCount(fs::path(currentPath() + "\\bganim"));
}