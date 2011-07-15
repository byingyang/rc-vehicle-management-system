remote: remote.o NXTVehicle.o BTVehicle.o
	g++ -O3 -L/usr/local/boost_1_39_0/stage/lib -lboost_thread-xgcc40-mt-1_39 -I/usr/local/boost_1_39_0 -o remote remote.o NXTVehicle.o BTVehicle.o

remote.o: remote.cc NXTVehicle.o
	g++ -O3 -c -I/usr/local/boost_1_39_0 remote.cc

NXTVehicle.o: NXTVehicle.h NXTVehicle.cc BTVehicle.o
	g++ -O3 -c -I/usr/local/boost_1_39_0 NXTVehicle.cc

BTVehicle.o: BTVehicle.h BTVehicle.cc
	g++ -O3 -c -I/usr/local/boost_1_39_0 BTVehicle.cc

clean:
	rm *.o
	rm remote