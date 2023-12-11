#include <exception>
#include <string>
#include "RE.hpp"

RuntimeError :: RuntimeError(const std :: string& message) : message(message) {}

const char* RuntimeError :: what() const noexcept 
{
    return message.c_str();
}