//cache_lib.cc

#include "cache.hh"
#include "fifo_evictor.hh"
#include <vector>
#include <iostream>
#include <map>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/basic_stream.hpp>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/utility/string_ref.hpp> 
#include <boost/spirit/include/qi.hpp>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
namespace pt = boost::property_tree; // from <boost/property_tree.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>

class Cache::Impl
{

public:

    // Set up member vars
    std::string host_ip_;
    std::string external_port_;
    int version = 11;
    tcp::resolver::results_type results;
    net::io_context ioc; // The io_context is required for all I/O
    // These objects perform our I/O
    tcp::resolver resolver_;
    beast::tcp_stream* stream_;

    ~Impl() {
        beast::error_code ec;
        stream_->socket().shutdown(tcp::socket::shutdown_both, ec); //close stream
	delete stream_;
    }    

    // Cache client constructor
    Impl(std::string host, std::string port) : resolver_(ioc) {

	host_ip_ = host;
	external_port_ = port;
	
	//try to establish connection to host via port given
	try
    	{

		// Look up the domain name
		results = resolver_.resolve(host_ip_, external_port_);

		// Make the connection on the IP address we get from a lookup
		stream_ = new beast::tcp_stream(ioc);
		stream_->connect(results);
		
		// not_connected happens sometimes
		// so don't bother reporting it.
		//
		//if(ec && ec != beast::errc::not_connected)
		//    throw beast::system_error{ec};
		

	}

	    //if don't not not work, end session w/ error.	
	    catch(std::exception const& e)
	    {
		std::cerr << "Error: " << e.what() << std::endl;
		return;
	    }
	
	}

    void set(key_type key, val_type val, size_type){

		stream_->connect(results);
		// Set up an HTTP PUT request message
		auto target = "/" + key + "/" + val; // build target 
		http::request<http::string_body> req{http::verb::put,target,version};
		req.set(http::field::host, host_ip_);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

		// Send the HTTP request to the remote host
		http::write(*stream_,req);

		// This buffer is used for reading and must be persisted
		beast::flat_buffer buffer;

		// Declare a container to hold the response
		// Make sure to parse and check for errors!
		http::response<http::dynamic_body> res;

		// Receive the HTTP response
		http::read(*stream_, buffer, res);

		// Write the message to standard out, this should be the value of key
		// and the size of the value.
		std::cout << res << std::endl;

		return;


    }

     // Retrieve a pointer to the value associated with key in the cache,
     // or nullptr if not found.
     // Sets the actual size of the returned value (in bytes) in val_size.
     val_type get(key_type key, size_type&) {

		stream_->connect(results);
		// Set up an HTTP GET request message
		auto target = "/" + key; // build target
		http::request<http::string_body> req{http::verb::get, target, version};
		req.set(http::field::host, host_ip_);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

		// Send the HTTP request to the remote host
		http::write(*stream_, req);

		// This buffer is used for reading and must be persisted
		beast::flat_buffer buffer;

		// Declare a container to hold the response
		http::response<http::string_body> res;

		// Receive the HTTP response
		http::read(*stream_, buffer, res);

		// Write the message to standard out, this should be the value of key
		// and the size of the value
		std::cout << res << std::endl;
		
		// if key found, return
		if (res.result() == http::status::ok) {

			// read string body as json and return
			std::stringstream ss;		
			ss << res.body();
			pt::ptree pt;
			read_json(ss, pt);
			auto temp = pt.get<std::string>("value");
			Cache::val_type value = temp.c_str();

			return value;
		}
	
		else {
			return nullptr;
		}

    }


     // Delete an object from the cache, if it's still there
     bool del(key_type key) {

		stream_->connect(results);
		// Set up an HTTP GET request message
		auto target = "/" + key; // build target
		http::request<http::string_body> req{http::verb::delete_, target, version};
		req.set(http::field::host, host_ip_);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

		// Send the HTTP request to the remote host
		http::write(*stream_, req);

		// This buffer is used for reading and must be persisted
		beast::flat_buffer buffer;

		// Declare a container to hold the response
		http::response<http::string_body> res;

		// Receive the HTTP response
		http::read(*stream_, buffer, res);

		// Write the message to standard out
		std::cout << res << std::endl;
		
		// if the server found the key, then return true
		if (res.result() == http::status::ok){return true;}

		// if not, return false
		else{return false;}
     }

     // Compute the total amount of memory used up by all cache values (not keys)
     size_type space_used() const {

		stream_->connect(results);
		// Set up an HTTP head request message
		auto target = "Makessense";
		http::request<http::empty_body> req{http::verb::head, target, version};
		req.set(http::field::host, host_ip_);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
		
		// Send the HTTP request to the remote host
		http::write(*stream_, req);

		// This buffer is used for reading and must be persisted
		beast::flat_buffer buffer;

		// Declare a container to hold the response
		http::response<http::empty_body> res;

		// Receive the HTTP response
		http::read(*stream_, buffer, res);

		// Write the message to standard out
		std::cout << res << std::endl;

		// Parse Space_used field of header and return!
		auto res_string = res.base().at("Space-Used");
		int space_used;
		boost::spirit::qi::parse(std::cbegin(res_string), std::cend(res_string), boost::spirit::qi::int_, space_used);

		return space_used;

     }

     // Delete all data from the cache
     void reset(){

		stream_->connect(results);
		// Set up an HTTP POST request message
		auto target = "/reset";
		http::request<http::string_body> req{http::verb::post	, target, version};
		req.set(http::field::host, host_ip_);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);


		// Send the HTTP request to the remote host
		http::write(*stream_, req);

		// This buffer is used for reading and must be persisted
		beast::flat_buffer buffer;

		// Declare a container to hold the response
		http::response<http::dynamic_body> res;

		// Receive the HTTP response
		http::read(*stream_, buffer, res);

		// Write the message to standard out, this should be the value of key
		// and the size of the value
		std::cout << res << std::endl;

     }

 private:     
    std::unordered_map<key_type, std::pair<val_type,size_type>,hash_func> storage; //stores value pointer and size of value

    hash_func hasherer;
};


Cache::Cache(std::string host, std::string port)
: pImpl_(new Impl(host, port))
{}

Cache::~Cache() {} //don't set to default...will give memory error

void Cache::set(key_type key, 
        Cache::val_type val, 
        Cache::size_type size)
{
    pImpl_->set(key,val,size);
}

Cache::val_type Cache::get(key_type key, 
        Cache::size_type& val_size) const
{
    return pImpl_->get(key,val_size);
}


bool Cache::del(key_type key)
{
    return pImpl_->del(key);
}

Cache::size_type Cache::space_used() const
{
    return pImpl_->space_used();
}

void Cache::reset()
{
    pImpl_->reset();
}
