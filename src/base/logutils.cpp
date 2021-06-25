#include "logutils.h"

#include <string>
#include "plog/Log.h"
#include "plog/Appenders/ColorConsoleAppender.h"
#include <dirent.h>

using std::string;

void intializeLog(string mode, string logDirName, string logFileName)
{

    string logPath = logDirName + + "/" + logFileName;

    if(mode == "t" || mode == "tf")
    {
        static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
        plog::init(plog::debug, &consoleAppender);
    }
 
    if(logPath.length() > 0 && (mode == "f" || mode == "tf"))
    {
        plog::init(plog::debug, logPath.c_str(), 1024 * 1024 * 10, 8);
    }
}
