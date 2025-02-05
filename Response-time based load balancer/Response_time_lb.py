import socket
import threading
import time
from statistics import mean

class LoadBalancer:
    def __init__(self, host='10.0.1.254', port=80):
        self.host = host
        self.port = port
        self.servers = [
            ('10.0.2.1', 80),  # server1
            ('10.0.2.2', 80),   # server2
            ('10.0.2.3', 80),	#server3
            ('10.0.2.4', 80)	#server4
        ]
        # Dictionary to store response times for each server
        self.response_times = {server: [] for server in self.servers}
        # Window size for calculating average response time
        self.window_size = 5
        # Lock for thread-safe updates to response_times
        self.lock = threading.Lock()

        # Create server socket
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.socket.bind((self.host, self.port))
        self.socket.listen(5)

    def get_server_average_response_time(self, server):
        """Calculate average response time for a server"""
        with self.lock:
            times = self.response_times[server]
            if not times:
                return 0  # Return 0 if no measurements yet
            return mean(times[-self.window_size:])  # Use last window_size measurements

    def update_response_time(self, server, response_time):
        """Update response time measurements for a server"""
        with self.lock:
            self.response_times[server].append(response_time)
            # Keep only the last window_size measurements
            if len(self.response_times[server]) > self.window_size:
                self.response_times[server] = self.response_times[server][-self.window_size:]

    def get_next_server(self):
        """Get server with lowest average response time"""
        response_times = {server: self.get_server_average_response_time(server) for server in self.servers}
        best_server = min(
            response_times,
            key=response_times.get
        )
        print("Selected server: ",best_server)
        print("Current average response times:")
        for server, avg_time in response_times.items():
        	print("  ", server, ": ", avg_time, "s")

        # for server in self.servers:
        #     avg_time = self.get_server_average_response_time(server)
        #     print("  ",server,": ",avg_time,"s")
        return best_server

    def measure_response_time(self, server, request):
        """Measure response time for a server"""
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
                start_time = time.time()

                # Connect and send request
                server_socket.connect(server)
                server_socket.send(request.encode())

                # Get response
                response = b''
                # while True:
                data = server_socket.recv(4096)
                # if not data:
                #     break
                response += data

                # Calculate response time
                response_time = time.time() - start_time
                self.update_response_time(server, response_time)
                return response

        except Exception as e:
            print("Error measuring response time for ",server,": ",e)
            # Penalize server for failed request with high response time
            self.update_response_time(server, float('inf'))
            raise

    def handle_client(self, client_socket, client_address):
        """Handle individual client connections."""
        try:
            # Receive client request
            request = client_socket.recv(4096).decode()
            if not request:
                return

            # Get server with best response time
            server_addr = self.get_next_server()

            try:
                # Forward request and measure response time
                response = self.measure_response_time(server_addr, request)
                # Send response back to client
                client_socket.send(response)
            except Exception as e:
                print("Error forwarding request to ",server_addr,": ",e)
                # Try another server if the first one fails
                for backup_server in self.servers:
                    if backup_server != server_addr:
                        try:
                            response = self.measure_response_time(backup_server, request)
                            client_socket.send(response)
                            break
                        except Exception as backup_e:
                            print("Backup server ",backup_server," also failed: ",backup_e)

        except Exception as e:
            print("Error handling client ",client_address,": ",e)
        finally:
            client_socket.close()

    def start(self):
        """Start the load balancer."""
        print("Load balancer listening on ",self.host,":",self.port)

        while True:
            try:
                # Accept client connection
                client_socket, client_address = self.socket.accept()
                print("Accepted connection from ",client_address)

                # Handle client in a new thread
                client_thread = threading.Thread(
                    target=self.handle_client,
                    args=(client_socket, client_address)
                )
                client_thread.start()

            except Exception as e:
                print("Error accepting connection: ",e)
                break

        self.socket.close()

# Start the load balancer
load_balancer = LoadBalancer()
load_balancer.start()
