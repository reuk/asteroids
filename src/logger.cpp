#include "logger.h"

using namespace std;
using namespace std::chrono;

Logger::Logger(const string & logfile):
    fname(logfile)
{

}

Logger::Logger():
    Logger("gl.log")
{

}

void Logger::restart() const {
    ofstream of(fname, ofstream::trunc);
}
