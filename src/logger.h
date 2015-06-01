#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <chrono>
#include <ctime>

class Logger {
public:
    Logger() = delete;

    static void restart();

    template<typename... Ts>
    static void log(Ts&&... ts) {
        auto str = get_string(ts...);
        std::ofstream of(fname, std::ofstream::app);
        of << str << std::flush;
    }

    template<typename... Ts>
    static void log_err(Ts&&... ts) {
        auto str = get_string(ts...);
        std::ofstream of(fname, std::ofstream::app);
        of << str << std::flush;
        std::cerr << str << std::flush;
    }

private:
    static const std::string fname;

    template<typename... Ts>
    static std::string get_string(Ts&&... ts) {
        std::stringstream ss;
        auto t0 = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto t1 = localtime(&t0);
        ss << std::put_time(t1, "%a %b %d %H:%M:%S %Y") << ": ";
        build_string(ss, ts...);
        ss << std::endl;
        return ss.str();
    }

    template<typename T>
    static void build_string(std::stringstream & ss, T && t) {
        ss << t;
    }

    template<typename T, typename... Ts>
    static void build_string(std::stringstream & ss, T && t, Ts... ts) {
        ss << t;
        build_string(ss, ts...);
    }
};
