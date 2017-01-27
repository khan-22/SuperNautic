#include "Log.hpp"

detail::Log* detail::Log::_instance = nullptr;

detail::Log::Log()
{
	
}

detail::Log::~Log()
{
	_file.open("log.txt");

	_file << _message.str();

	_file.close();
}

detail::Log* detail::Log::getInstance()
{
	if (!_instance)
	{
		_instance = new Log();
	}

	return _instance;
}

void detail::Log::close()
{
	delete _instance;
}
