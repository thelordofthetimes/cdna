// impementation    : /cdna/shell/shell.hpp
// author           : thelordofthetimes@gmail.com

#include <cdna/shell/shell.hpp>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/asio/buffer.hpp>

#include <cdna/com/signal.hpp>

namespace cdna {
namespace shell {

shell::shell(const std::string name,
        boost::asio::ip::tcp::endpoint host,
        cdna::kernel::kernel *kernel)
{
    // save properties
    this->name_ = name;
    this->host_ = host;
    this->kernel_ = kernel;

    // set default value
    this->receive_thread_ = NULL;
    this->send_thread_ = NULL;

    // create thread
    this->create_receive_thread_();
    this->create_send_thread_();
}

shell::~shell()
{
    // release receive_thread_
    if(this->receive_thread_ != NULL) {
        this->receive_thread_->interrupt();
        this->receive_thread_->join();
        free(this->receive_thread_);
    }

    // release send_thread_
    if(this->send_thread_ != NULL) {
        this->send_thread_->interrupt();
        this->send_thread_->join();
        free(this->send_thread_);
    }
}

void
shell::start()
{
    this->start_kernel_();
    this->start_receive_();
    this->start_send_();
}

void
shell::start_send_async_()
{
    this->start_kernel_async_();
    this->start_receive_async_();
    this->start_send_async_();
}

void
shell::start_kernel_()
{
    this->kernel_->start();
}

void
shell::start_kernel_async_()
{
    this->kernel_->start_async();
}

void
shell::start_receive_()
{
    this->receive_thread_->detach();
}

void
shell::start_receive_async_()
{
    this->receive_thread_->detach();
}

void
shell::start_send_()
{
    this->send_thread_->join();
}

void
shell::create_receive_thread_()
{
    auto entry = boost::bind(&shell::recevie_entry_, this);
    this->receive_thread_ = new boost::thread(entry);
}

void
shell::create_send_thread_()
{
    auto entry = boost::bind(&shell::send_entry_, this);
    this->send_thread_ = new boost::thread(entry);
}

void
shell::send_entry_()
{
    boost::asio::io_service io;

    while(!this->send_thread_->interruption_requested()) {

        // get signal form kernel
        cdna::com::signal out_signal = this->kernel_->signal_out();


        // connect to outside
        // todo: resolve agent endpoint
        boost::asio::ip::tcp::socket socket(io);
        auto agent_ip = boost::asio::ip::address::from_string("127.0.0.1");
        boost::asio::ip::tcp::endpoint agent_endpoint(agent_ip, 10001);
        socket.connect(out_signal.dest());

        // send signal to destination
        auto out_buffer = boost::asio::buffer(out_signal.content());
        socket.write_some(out_buffer);

    }
}

void
shell::recevie_entry_()
{
    boost::asio::io_service io;
    boost::asio::ip::tcp::acceptor acceptor(io, this->host_);

    while(!this->receive_thread_->interruption_requested()) {

        // waith signal form outside
        boost::asio::ip::tcp::socket socket(io);
        acceptor.accept(socket);

        // read signal from outside
        boost::system::error_code error;
        boost::array<char, 1024> data_in;
        auto buffer_in = boost::asio::buffer(data_in);
        socket.read_some(buffer_in, error);

        // check read error
        if(error) {
            continue;
        }

        // convert buffer to string
        std::string content_in(data_in.begin(), data_in.end());

        // prepare signal to send to kernel
        boost::asio::ip::tcp::endpoint source = socket.remote_endpoint();
        boost::asio::ip::tcp::endpoint dest = this->kernel_->endpoint();
        cdna::com::signal signal_in(source, dest, content_in);

        // send signal to kernel
        this->kernel_->signal_in(signal_in);
    }
}

}}
