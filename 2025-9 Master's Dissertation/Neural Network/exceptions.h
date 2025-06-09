#pragma once
#include <exception>
#include <iostream>

#define MYEXCEPT

#ifdef MYEXCEPT
namespace helpers {
	class MyExcept {
	public:
		MyExcept(const std::string& message) {
			std::cout << std::endl << "Error: " << message << std::endl;
			while (true)
				// Do not remove breakpoint
				int i = 0;
		}
	};
}
#else
namespace helpers {
	class MyExcept : public std::exception
	{
		std::string _message;
	public:
		explicit MyExcept(const std::string& message) : _message(message), std::exception(message.c_str()) {}

		virtual const char* what() const noexcept override {
			return _message.c_str();
		}
	};
}
#endif

namespace helpers {
	class NotImplementedException : public MyExcept {
	public:
		NotImplementedException() : MyExcept("Code not implemented") {}
	};
	class ShouldNotRunException : public MyExcept {
	public:
		ShouldNotRunException() : MyExcept("Should not run") {}
	};
}