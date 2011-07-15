#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include <vector>
#include "request.h"
#include "BTVehicle.h"
#include "connection.h"
#include "SessionController.h"

/// Represents a single connection from a client.
class connection : public boost::enable_shared_from_this<connection>, private boost::noncopyable {
public:
  	// Construct a connection with the given io_service.
  	explicit connection(boost::asio::io_service& io_service, SessionController &);
	explicit connection(const connection &c);

  	// Get the socket associated with the connection.
  	boost::asio::ip::tcp::socket &get_socket();

  	// Start the operation for the connection.
  	void start();

private:

  	// Parse the request into a reply
  	void parse_request(std::string &, std::size_t);

	// get number of vehicles
	void num_vehicles();
	
	// get name of vehicle at 1-based index and put it in reply
	void name(int index);
	
	// start a session with a particular vehicle
	void connect(int index);
	
	// number of motors
	void num_motors(int index);

	// set speed
	void set_speed(int, int, int);
	
	// disconnect session from the vehicle
	void disconnect(int index);
	
	void send_reply();
	
	boost::shared_ptr<BTVehicle> get_vehicle(int);
	
  	// Strand to ensure the connection's handlers are not called concurrently.
  	boost::asio::io_service::strand strand;

  	// Socket for the connection.
  	boost::asio::ip::tcp::socket socket;
	
	// buffer to hold incoming request string
	boost::asio::streambuf buffer;

	// session controller
	SessionController &session_controller;

  	// The incoming request.
  	struct request request;

  	// The reply to be sent back to the client.
	std::string reply;
	
	int error_code;
};

typedef boost::shared_ptr<connection> connection_ptr;

#endif