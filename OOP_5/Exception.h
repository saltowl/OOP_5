#pragma once
#include <string>

class ProgramException : public std::exception
{
public:
	ProgramException(const std::string& message) noexcept;
	~ProgramException() noexcept = default;

	const char* what() const noexcept override;

private:
	std::string message;
};

class WrongAnswer : public ProgramException
{
public:
	WrongAnswer(const std::string& message) noexcept;
};

class IOException : public ProgramException
{
public:
	IOException(const std::string& message) noexcept;
};