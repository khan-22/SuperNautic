#pragma once

#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

namespace detail
{
	/*
		DO NOT USE THIS CLASS MANUALLY!
		This class is responsible for logging everything that is
		written to the console.

		To do this, always use the LOG(...) macro.
	*/
	class Log
	{
	public:
		~Log();

		static Log* getInstance();
		static void close();

		template <typename Arg>
		void print(Arg arg);

		template <typename Arg, typename ...TArgs>
		void print(Arg arg, TArgs... mArgs);

		template <typename ...TArgs>
		void print(TArgs... mArgs);

	protected:

	private:
		static Log*	_instance;

		std::stringstream	_message;
		std::ofstream		_file;

		Log();
	};


	template <class Arg>
	void Log::print(Arg arg)
	{
		_message << std::forward<Arg>(arg) << std::endl;
		std::cout << std::forward<Arg>(arg) << std::endl;
	}

	template <class Arg, class ...TArgs>
	void Log::print(Arg arg, TArgs... mArgs)
	{
		_message << std::forward<Arg>(arg);
		std::cout << std::forward<Arg>(arg);

		print(std::forward<TArgs>(mArgs)...);
	}

	template <class ... TArgs>
	void Log::print(TArgs... mArgs)
	{
		print(std::forward<TArgs>(mArgs)...);
	}
}


#if !defined(NDEBUG)
	#define LOG(...) detail::Log::getInstance()->print(__VA_ARGS__)
	#define LOG_ERROR(...) detail::Log::getInstance()->print((strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__), " - Line: ", __LINE__, " - ", __VA_ARGS__)
	#define CLOSE_LOG() detail::Log::close();

#else
	#define LOG(...)
	#define LOG_ERROR(...)
	#define CLOSE_LOG()
#endif



#endif //LOG_H
