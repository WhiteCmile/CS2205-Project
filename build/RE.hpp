#ifndef RE_HPP
#define RE_HPP

#include <exception>
#include <string>

class RuntimeError : public std :: exception
{
private:
    std :: string message;
public:

    RuntimeError(const std :: string&);
    const char* what() const noexcept override;
};

#endif