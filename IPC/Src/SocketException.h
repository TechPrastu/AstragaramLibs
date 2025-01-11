// SocketException class
#ifndef SocketException_class
#define SocketException_class

#include <exception>
#include <string>

class SocketException : public std::exception
{
public:
    explicit SocketException( const std::string& message ) : m_s( message ) {}
    ~SocketException() noexcept override = default;

    const char* what() const noexcept override
    {
        return m_s.c_str();
    }

private:
    std::string m_s;
};

#endif
