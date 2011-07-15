#include "BTVehicleServer.h"

BTVehicleServer::BTVehicleServer(const char *address, const char *port, std::size_t thread_pool_size)
  	: thread_pool_size(thread_pool_size), 
	acceptor(io_service),
	session_controller(),
 	new_connection(new connection(io_service, session_controller)) {
	
  	// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
  	boost::asio::ip::tcp::resolver resolver(io_service);
  	boost::asio::ip::tcp::resolver::query query(address, port);
  	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
  	acceptor.open(endpoint.protocol());
  	acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  	acceptor.bind(endpoint);
  	acceptor.listen();
  	acceptor.async_accept(new_connection->get_socket(),
      boost::bind(&BTVehicleServer::handle_accept, this,
        boost::asio::placeholders::error));
}

void BTVehicleServer::run() {
  	// Create a pool of threads to run all of the io_services.
  	std::vector<boost::shared_ptr<boost::thread> > threads;
  	for(std::size_t i = 0; i < thread_pool_size; i++) {
    	boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service)));
    	threads.push_back(thread);
  	}

  	// Wait for all threads in the pool to exit.
  	for(std::size_t i = 0; i < threads.size(); i++)
    	threads[i]->join();
}

bool BTVehicleServer::AddVehicle(boost::shared_ptr<BTVehicle> v) {
	return session_controller.AddVehicle(v);
}

void BTVehicleServer::stop() {
  	io_service.stop();
}

void BTVehicleServer::handle_accept(const boost::system::error_code& e) {
  	if (!e) {
    	new_connection->start();
    	new_connection.reset(new connection(io_service, session_controller));
    	acceptor.async_accept(new_connection->get_socket(),
        boost::bind(&BTVehicleServer::handle_accept, this,
          boost::asio::placeholders::error));
  	}
}