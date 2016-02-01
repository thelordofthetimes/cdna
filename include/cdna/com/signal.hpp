// identity     : /cdna/kernel/com/signal.hpp
// using        : wrap signal from node
// author       : thelordofthetimes@gmail.com

#ifndef _CDNA_KERNEL_COM_SIGNAL_HPP_
#define _CDNA_KERNEL_COM_SIGNAL_HPP_

#include <string>
#include <boost/asio.hpp>

namespace cdna {
namespace com {

class signal {

protected:

    // source endpoint
    // it store where signal was sent
    boost::asio::ip::tcp::endpoint source_;

    // dest endpoint
    // it store where signal will go
    boost::asio::ip::tcp::endpoint dest_;

    // content of signal
    // it store content of signal
    std::string content_;

public:

    // constructor
    // create instance of signal
    // params
    //  - source: source enpoint
    //  - dest: dest endpoint
    //  - content: content of signal
    signal(const boost::asio::ip::tcp::endpoint source, 
            const boost::asio::ip::tcp::endpoint dest, 
            const std::string content = "");

    // destructor
    // release dynamic memory
    ~signal();

    // get source endpoint
    // return source endpoint
    boost::asio::ip::tcp::endpoint source();

    // get dest endpoint
    // return dest endpoint
    boost::asio::ip::tcp::endpoint dest();

    // get content of signal
    // return content of signal
    std::string content();
};

}}
#endif // _CDNA_KERNEL_COM_SIGNAL_HPP_
