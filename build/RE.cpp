#include <exception>
#include <string>
#include "RE.hpp"

RuntimeError :: RuntimeError(const std :: string& message) : message(message) {}

const char* RuntimeError :: what() const noexcept 
{
    printf("%s", message.c_str());
    return message.c_str();
}