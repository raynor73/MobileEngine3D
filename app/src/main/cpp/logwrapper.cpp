#include "logwrapper.h"

void Log::e(const std::string &tag, const std::string &text)
{
	log(ANDROID_LOG_ERROR, tag, text);
}

void Log::w(const std::string &tag, const std::string &text)
{
	log(ANDROID_LOG_WARN, tag, text);
}

void Log::i(const std::string &tag, const std::string &text)
{
	log(ANDROID_LOG_INFO, tag, text);
}

void Log::d(const std::string &tag, const std::string &text)
{
	log(ANDROID_LOG_DEBUG, tag, text);
}

void Log::log(int priority, const std::string &tag, const std::string &text)
{
	__android_log_write(priority, tag.c_str(), text.c_str());
}
