import socket
import threading
from collections import deque
import time
import random

class LoadBalancer:
    def __init__(self, host='10.0.1.254', port=80):
        """Initialize the load balancer with the given host and port."""
        self.host = host
        self.port = port
        # List of available backend servers
        self.current_server = 0
        self.servers = [
            ('10.0.2.1', 80),  # server1
            ('10.0.2.2', 80),   # server2
            ('10.0.2.3', 80)    #server3
        ]
        # Create server socket
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.socket.bind((self.host, self.port))
        self.socket.listen(5)
    def handle_client(self, client_socket, client_address):
        """Handle individual client connections."""
        try:
            # Receive client request
            request = client_socket.recv(4096).decode()
            if not request:
                return

            # Get next server using round-robin
            server_addr = self.get_next_server()

            # Forward request to server
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
                server_socket.connect(server_addr)
                server_socket.send(request.encode())

                # Get response from server
                response = b''
                while True:
                    data = server_socket.recv(4096)
                    if not data:
                        break
                    response += data

                # Send response back to client
                client_socket.send(response)

        except Exception as e:
            print("Error handling client ",client_address,": ",e)
        finally:
            client_socket.close()
    def get_next_server(self):
        """Get next server using round-robin."""
        server = self.servers[self.current_server]
        self.current_server = (self.current_server + 1) % len(self.servers)
        # self.current_server = random.randint(0,len(self.servers)-1)
        print("Selected server: ",server)
        return server

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

load_balancer = LoadBalancer()
load_balancer_thread = threading.Thread(target=load_balancer.start)
load_balancer_thread.start()
