server: server.o BTVehicleServer.o NXTVehicle.o BTVehicle.o connection.o SessionController.o
	g++ -L/usr/local/boost_1_39_0/stage/lib -I/usr/local/boost_1_39_0 -lboost_thread-xgcc40-mt-1_39 -lboost_filesystem-xgcc40-mt-1_39 -undefined dynamic_lookup  -I/Users/brad/BluetoothRC/BTVehicleRemote -o server server.o BTVehicleServer.o NXTVehicle.o BTVehicle.o connection.o SessionController.o

server.o: server.cc NXTVehicle.o BTVehicleServer.o BTVehicle.o connection.o
	g++ -c -I/usr/local/boost_1_39_0 -I/Users/brad/BluetoothRC/BTVehicleRemote server.cc

BTVehicleServer.o: BTVehicleServer.cc BTVehicleServer.h BTVehicle.o connection.o SessionController.o
	g++ -c -I/usr/local/boost_1_39_0 -I/Users/brad/BluetoothRC/BTVehicleRemote -L/usr/local/boost_1_39_0/stage/lib BTVehicleServer.cc

NXTVehicle.o: /Users/brad/BluetoothRC/BTVehicleRemote/NXTVehicle.h /Users/brad/BluetoothRC/BTVehicleRemote/NXTVehicle.cc BTVehicle.o
	g++ -c -I/usr/local/boost_1_39_0 /Users/brad/BluetoothRC/BTVehicleRemote/NXTVehicle.cc

connection.o: BTVehicle.o connection.h connection.cc request.h SessionController.o
	g++ -c -I/usr/local/boost_1_39_0 -I/Users/brad/BluetoothRC/BTVehicleRemote -L/usr/local/boost_1_39_0/stage/lib connection.cc
	
SessionController.o: SessionController.cc request.h BTVehicle.o SessionController.h SessionController.cc
	g++ -c -I/usr/local/boost_1_39_0 -I/Users/brad/BluetoothRC/BTVehicleRemote -L/usr/local/boost_1_39_0/stage/lib SessionController.cc

BTVehicle.o: /Users/brad/BluetoothRC/BTVehicleRemote/BTVehicle.h /Users/brad/BluetoothRC/BTVehicleRemote/BTVehicle.cc
	g++ -c -I/usr/local/boost_1_39_0 /Users/brad/BluetoothRC/BTVehicleRemote/BTVehicle.cc

clean:
	rm *.o
	rm server