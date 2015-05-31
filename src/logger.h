#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <chrono>
#include <ctime>

class Logger {
public:
    Logger();
    Logger(const std::string & logfile);

    void restart() const;

    template<typename... Ts>
    void log(Ts&&... ts) {
        auto str = get_string(ts...);
        std::ofstream of(fname, std::ofstream::app);
        of << str << std::flush;
    }

    template<typename... Ts>
    void log_err(Ts&&... ts) {
        auto str = get_string(ts...);
        std::ofstream of(fname, std::ofstream::app);
        of << str << std::flush;
        std::cerr << str << std::flush;
    }

private:
    std::string fname;

    template<typename... Ts>
    std::string get_string(Ts&&... ts) {
        std::stringstream ss;
        auto t0 = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto t1 = localtime(&t0);
        ss << std::put_time(t1, "%a %b %d %H:%M:%S %Y") << ": ";
        build_string(ss, ts...);
        ss << std::endl;
        return ss.str();
    }

    template<typename T>
    void build_string(std::stringstream & ss, T && t) {
        ss << t;
    }

    template<typename T, typename... Ts>
    void build_string(std::stringstream & ss, T && t, Ts... ts) {
        ss << t;
        build_string(ss, ts...);
    }
};
