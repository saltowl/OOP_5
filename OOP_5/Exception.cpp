#include "Exception.h"

ProgramException::ProgramException(const std::string& message) noexcept : message(message) {}

const char* ProgramException::what() const noexcept
{
	return this->message.c_str();
}

WrongAnswer::WrongAnswer(const std::string& message) noexcept : ProgramException(message) {}

IOException::IOException(const std::string& message) noexcept : ProgramException(message) {}