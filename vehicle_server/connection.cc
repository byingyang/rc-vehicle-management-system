#include <sstream>
#include "connection.h"

connection::connection(boost::asio::io_service& io_service, SessionController &sc)
	: strand(io_service), socket(io_service), session_controller(sc) { }

boost::asio::ip::tcp::socket &connection::get_socket() {
  	return socket;
}

void connection::start() {
	std::size_t sz = boost::asio::read_until(socket, buffer, '\n');
	std::istream is(&buffer);
	std::string data;
	std::getline(is, data);
	parse_request(data, sz);
	send_reply();
	boost::system::error_code ignored_ec;
	socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
}

void connection::send_reply() {
	if(error_code != 0) {
		std::string ecstr;
		std::stringstream in;
		in << error_code;
		ecstr = in.str();
		boost::asio::write(socket, boost::asio::buffer(ecstr));
	}
	else if(!reply.empty()) {
		boost::asio::write(socket, boost::asio::buffer(reply));
	}
}

void connection::parse_request(std::string &data, std::size_t bytes_transferred) {
	std::stringstream in(data);
	in >> request.command >> request.vehicle_index >> request.motor_index >> request.speed;
	if(request.command == "count") {
		num_vehicles();
	}
	else if(request.command == "name") {
		name(request.vehicle_index);
	}
	else if(request.command == "connect") {
		connect(request.vehicle_index);
	}
	else if(request.command == "motors") {
		num_motors(request.vehicle_index);
	}
	else if(request.command == "speed") {
		set_speed(request.vehicle_index, request.motor_index, request.speed);
	}
	else if(request.command == "disconnect") {
		disconnect(request.vehicle_index);
	}
}

void connection::num_vehicles() {
	int sz = session_controller.NumVehicles();
	std::stringstream out;
	out << sz;
	reply = out.str();
}

// puts the name of the requested vehicle in reply
void connection::name(int index) {
	boost::shared_ptr<BTVehicle> btv = get_vehicle(index);
	char name[255];
	btv->name(name, 255);
	reply = std::string(name);
}

void connection::connect(int index) {
	boost::shared_ptr<BTVehicle> btv = get_vehicle(index);
	bool b = btv->drive();
	if(b)
		btv->set_motorstates(MOTOR_ON);
	error_code = 1;
}

void connection::disconnect(int index) {
	boost::shared_ptr<BTVehicle> btv = get_vehicle(index);
	if(btv->is_connected()) {
		btv->stop_driving();
	}
}

void connection::num_motors(int index) {
	boost::shared_ptr<BTVehicle> btv = get_vehicle(index);
	int nm = btv->motor_count();
	std::stringstream out;
	out << nm;
	reply = out.str();
}

void connection::set_speed(int index, int motor, int speed) {
	boost::shared_ptr<BTVehicle> btv = get_vehicle(index);
	// we know that it's driving
	btv->set_motorspeed(motor, speed);
}

boost::shared_ptr<BTVehicle> connection::get_vehicle(int index) {
	return session_controller.GetVehicle(index);
}
