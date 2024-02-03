#include "Logger.h"
#include <assert.h>
#include <thread>


int main()
{
	const char* name = "User";
//	Logger::EnableFileOutput();
	Logger::SetPriority(LogLevel::INFO);
	LOG_INFO("Hello %s", name);

	LOG_CRITICAL("OH NO!", name);

	LOG_WARN("Hello %d %d", 15, 28);

	Logger::Info("This is the test msg %d", 10);
	Logger::Trace("This is the test msg %d", 30);

	
	return 0;
}