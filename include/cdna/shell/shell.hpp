// identity     : class cdna::shell::shell
// using        : bridge between kernel and outside
// author       : thelordofthetimes@gmail.com

#ifndef _CDNA_SHELL_SHELL_HPP_
#define _CDNA_SHELL_SHELL_HPP_

#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <cdna/kernel/kernel.hpp>

namespace cdna {
namespace shell {

class shell {

protected:

    // name of shell
    // readable identity of shell
    std::string name_;

    // endpoint of kernel
    boost::asio::ip::tcp::endpoint host_;

    // instance of kernel
    cdna::kernel::kernel *kernel_;

    // thread which receive signal from outside
    boost::thread *receive_thread_;

    // thread which send signal to outside
    boost::thread *send_thread_;

public:

    // constructor
    // params
    //  - name: name of kernel
    //  - host: endpoint of kernel
    //  - kernel: instance of kernel
    shell(const std::string name,
            boost::asio::ip::tcp::endpoint host,
            cdna::kernel::kernel *kernel);

    // destructor
    // release dynamic memory
    ~shell();

    // start shell synchronous
    void start();

    // start shell asynchronous
    void start_async();

protected:

    // start kernel
    void start_kernel_();
    void start_kernel_async_();

    // start receive
    void start_receive_();
    void start_receive_async_();

    // start send
    void start_send_();
    void start_send_async_();

    // create thread to recevie signal from outside
    void  create_receive_thread_();

    // create thread to send signal from kernel to outside
    void create_send_thread_();

    // entry for recevie thread
    void recevie_entry_();

    // entry for send thread
    void send_entry_();
};

}}

#endif // _CDNA_SHELL_SHELL_HPP_
