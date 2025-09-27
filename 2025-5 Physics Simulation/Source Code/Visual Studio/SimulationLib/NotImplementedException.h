#pragma once
#include <exception>
#include <string>
#include <iostream>
#include <Windows.h>

class NotImplementedException : public std::exception
{
public:
	NotImplementedException(const char* message = "Function not yet implemented")
		: std::exception(message) {}
};

class AbstractClassException : public std::exception {
public:
	AbstractClassException(const char* message = "A virtual function was called that is deliberately not implemented")
		: std::exception(message) {}
};

class ShouldNotRunException : public std::exception {
public:
	ShouldNotRunException(const char* message = "This code should not run")
		: std::exception(message) {}
};

class MyException : public std::exception
{
	std::string _message;
public:
	explicit MyException(const std::string& message) : _message(message), std::exception(message.c_str()) {}

	virtual const char* what() const noexcept override {
		return _message.c_str();
	}
};

namespace helpers {
	inline void myExcept(const std::string& msg) {
		std::cout << std::endl << "Error: " << msg << std::endl;
		while (true)
			// Do not remove breakpoint
			Sleep(1000);
	}
}