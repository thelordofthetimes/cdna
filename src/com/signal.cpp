// implement        : /cdna/kernel/com/signal.hpp
// author           : thelordofthetimes@gmail.com

#include <cdna/com/signal.hpp>

namespace cdna {
namespace com {

signal::signal(const boost::asio::ip::tcp::endpoint source, 
        const boost::asio::ip::tcp::endpoint dest,
        std::string content) 
{
    this->source_ = source;
    this->dest_ = dest;
    this->content_ = content;
}

signal::~signal() 
{
}

boost::asio::ip::tcp::endpoint
signal::source() 
{
    return this->source_;
}

boost::asio::ip::tcp::endpoint
signal::dest() 
{
    return this->dest_;
}

std::string 
signal::content() 
{
    return this->content_;
}

}}
