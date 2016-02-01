// implement    : /cdna/kernel/kernel.hpp
// author       : thelordofthetimes@gmail.com

#include <cdna/kernel/kernel.hpp>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

namespace cdna {
namespace kernel {

kernel::kernel(const std::string &name)
{
    this->name_ = name;
    this->live_thread_ = new boost::thread(boost::bind(&kernel::live_entry_, 
                this));
}

kernel::~kernel()
{
    // stop, join, release live_thread_
    if(this->live_thread_ != NULL) {
        this->live_thread_->interrupt();
        this->live_thread_->join();
        this->live_thread_->~thread();
        free(this->live_thread_);
    }
}

const boost::asio::ip::tcp::endpoint
kernel::endpoint()
{
    return this->endpoint_;
}

const std::string
kernel::name()
{
    return this->name_;
}

bool
kernel::start()
{
    // start, wait live thread done
    this->live_thread_->join();

    // start success
    return true;
}

bool
kernel::start_async()
{
    // start, detach live_thread_
    this->live_thread_->detach();

    // start success
    return true;
}

void
kernel::signal_in(const cdna::com::signal &signal)
{
    this->signal_in_mutex_.lock();
    this->signal_in_.push(signal);
    this->signal_in_mutex_.unlock();
}

const cdna::com::signal
kernel::signal_out()
{
    // wait until out signal is exists
    while(true) {

        // lock access to signal_out_
        this->signal_out_mutex_.lock();

        if(this->signal_out_.size() > 0) {
            break;
        }

        // unlock access to signal_out_
        this->signal_out_mutex_.unlock();
    }

    // get message from message list
    cdna::com::signal signal_out = this->signal_out_.front();
    this->signal_out_.pop();

    // unlock access to signal_out_
    this->signal_out_mutex_.unlock();

    // return out message
    return signal_out;
}

void
kernel::live_entry_()
{
    // loop of life
    // with each loop, check signal_in_, process it and put result to 
    //  signal_out_
    while(!this->live_thread_->interruption_requested()) {

        // lock access to signal_in_
        this->signal_in_mutex_.lock();

        // check signal_in_ have any singal
        if(this->signal_in_.size() > 0) {

            // pop signal from signal_in_
            cdna::com::signal signal_in = this->signal_in_.front();
            this->signal_in_.pop();

            // process signal_in
            std::ostringstream out_message;
            cdna::com::signal signal_out(this->endpoint_, signal_in.source(),
                    "receive: " + signal_in.content());

            // put signal_out to signal_out_
            this->signal_out_mutex_.lock();
            this->signal_out_.push(signal_out);
            this->signal_out_mutex_.unlock();
        }

        // unlock access to signal_in_
        this->signal_in_mutex_.unlock();
    }
}

}}
