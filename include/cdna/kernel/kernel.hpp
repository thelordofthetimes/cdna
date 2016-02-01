// identity     : /cdna/kernel/kernel.hpp
// using        : recevie/process/response signal from ouside
// author       : thelordofthetimes@gmail.com

#ifndef _CDNA_KERNEL_KERNEL_HPP_
#define _CDNA_KERNEL_KERNEL_HPP_

#include <string>
#include <queue>
#include <list>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <cdna/com/signal.hpp>

namespace cdna {
namespace kernel {

class kernel {

protected:

    // kernel endpoint
    // identity of kernel in network
    boost::asio::ip::tcp::endpoint endpoint_;

    // name of kernel
    // readable identity of kernel
    std::string name_;

    // thread using for live_thread_
    boost::thread *live_thread_;

    // mutexs
    boost::mutex signal_in_mutex_;
    boost::mutex signal_out_mutex_;

    // queue in signal 
    // list of signal send from outside
    std::queue<cdna::com::signal> signal_in_;

    // queue out signal 
    // list of signal send to outside
    std::queue<cdna::com::signal> signal_out_;

public:

    // constructor
    // create instance of kernel
    // params
    //  - name: name of kernel
    kernel(const std::string &name);

    // destructor
    // release dynamic memory
    ~kernel();

    // accessors
    const boost::asio::ip::tcp::endpoint endpoint();
    const std::string name();

    // start kernel
    bool start();
    bool start_async();

    // send signal to kernel
    // params
    //  - signal: signal to send
    void signal_in(const cdna::com::signal &signal);

    // get signal on top of signal out of kernel
    // return signal
    const cdna::com::signal signal_out();

protected:

    // loop of kernel to receive/process/response signal and evole
    void live_entry_();
};

}}

#endif // _CDNA_KERNEL_KERNEL_HPP_
