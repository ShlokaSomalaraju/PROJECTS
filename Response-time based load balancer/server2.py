import socket
import threading
from collections import deque
import time

class SimpleHTTPServer:
    def __init__(self, host, port, server_name):
        """Initialize HTTP server with given host, port and server name."""
        self.host = host
        self.port = port
        self.server_name = server_name
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.socket.bind((self.host, self.port))
        self.socket.listen(5)

    def handle_request(self, client_socket):
        """Handle HTTP request."""
        try:
            request = client_socket.recv(4096).decode()
            if not request:
                return

            # Create a simple HTTP response
            response = """
            HTTP/1.1 200 OK
Content-Type: text/html
<h1>Response from {}</h1>
<p>Time: {}</p>
""".format(self.server_name,time.strftime('%Y-%m-%d %H:%M:%S'))

            client_socket.send(response.encode())

        except Exception as e:
            print("Error handling request: ", e)
        finally:
            client_socket.close()

    def start(self):
        """Start the HTTP server."""
        print("Server ",self.server_name," listening on ",self.host,":",self.port)

        while True:
            try:
                client_socket, client_address = self.socket.accept()
                print("Received connection from ",client_address)
                threading.Thread(
                    target=self.handle_request,
                    args=(client_socket,)
                ).start()
            except Exception as e:
                print("Error accepting connection: ",e)
                break

        self.socket.close()

server = SimpleHTTPServer('10.0.2.2',80,'Server 2')
server.start()
