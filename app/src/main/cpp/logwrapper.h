#ifndef LOGWRAPPER_H
#define LOGWRAPPER_H

#include <android/log.h>
#include <string>

class Log
{
public:
	static void e(const std::string &tag, const std::string &text);
	static void w(const std::string &tag, const std::string &text);
	static void i(const std::string &tag, const std::string &text);
	static void d(const std::string &tag, const std::string &text);

private:
	static void log(int priority, const std::string &tag, const std::string &text);
};

#endif //LOGWRAPPER_H
