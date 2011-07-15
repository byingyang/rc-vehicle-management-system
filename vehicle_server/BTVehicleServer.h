#ifndef BTVEHICLESERVER_H
#define BTVEHICLESERVER_H

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <string>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "BTVehicle.h"
#include "connection.h"

class BTVehicleServer : private boost::noncopyable {
public:
  	// Construct the server to listen on the specified TCP address and port
  	explicit BTVehicleServer(const char *, const char *, std::size_t);

  	// Run the server's io_service loop.
  	void run();

  	// Stop the server.
  	void stop();
	
	// Add a vehicle
	bool AddVehicle(boost::shared_ptr<BTVehicle> v);

private:
  	// Handle completion of an asynchronous accept operation.
  	void handle_accept(const boost::system::error_code& e);

  	// The number of threads that will call io_service::run().
  	std::size_t thread_pool_size;

  	// The io_service used to perform asynchronous operations.
  	boost::asio::io_service io_service;

  	// Acceptor used to listen for incoming connections.
  	boost::asio::ip::tcp::acceptor acceptor;

  	// The next connection to be accepted.
  	connection_ptr new_connection;

	// The session controller
	SessionController session_controller;
};

#endif