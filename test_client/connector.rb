require 'socket'

class VehicleConnector
  attr_accessor :hostname, :port, :socket
  
  def initialize(hn, p)
    @hostname = hn
    @port = p
  end
  
  def connect
    @socket = TCPSocket.open(@hostname, @port)
  end
  
  def available_vehicles
    connect
    @socket.puts("count")
    ret = @socket.readline.chomp.to_i
    disconnect
    ret
  end
  
  def name(index)
    connect
    @socket.puts("name #{index}")
    ret = @socket.readline.chomp
    disconnect
    ret
  end
  
  def motor_count(index)
    connect
    @socket.puts("motors #{index}")
    ret = @socket.readline.to_i
    disconnect
    ret
  end
  
  def drive(index)
    connect
    @socket.puts("connect #{index}")
    ret = @socket.readline.to_i == 1
    disconnect
    ret
  end
  
  def set_speed(index, motor, speed)
    connect
    @socket.puts("speed #{index} #{motor} #{speed}")
    disconnect
  end
  
  def disconnect_vehicle
    connect
    @socket.puts("disconnect #{index}")
    disconnect
  end
  
  def disconnect
    @socket.close
  end
end

c = VehicleConnector.new('0.0.0.0', 52000)
for i in 1..c.available_vehicles do
  puts "Vehicle #{i}: Name: #{c.name(i)} MotorCount: #{c.motor_count(i)}"
  puts "error connecting!" if !c.drive(i)
  c.set_speed(i, 0, 5)
  c.set_speed(i, 2, -50)
end