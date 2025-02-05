import socket
import time

# Define the IP address and port of the load balancer
load_balancer_ip = '10.0.1.254'  # Change this to the IP of your load balancer
load_balancer_port = 80  # Change this to the port of your load balancer

# Create a socket to communicate with the load balancer
# client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
i=0
total_req = 0
latencies = []
start_time = time.time()

try:

    for j in range(25):  # You can adjust the number of requests here
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        start_req = time.time()
        client_socket.connect((load_balancer_ip, load_balancer_port))
        print("Connected to load balancer at ",load_balancer_ip,":",load_balancer_port)
        request = "GET / HTTP/1.1\r\nHost: "+load_balancer_ip+"\r\n\r\n"
        i=i+1
        print("Sending request #",i)
        client_socket.send(request.encode())  # Send the request

        # Receive the response from the load balancer
        response = client_socket.recv(1024)
        end_req = time.time()
        print("Response #",i,": ",response.decode())

        latency = (end_req - start_req)*1000
        latencies.append(latency)
        total_req += 1

        client_socket.close()

        time.sleep(1)  # Sleep for 0.1 second between requests

except Exception as e:
	print("Error: ",e)

finally:
    elapsed_time = time.time() - start_time
    throughput = total_req/elapsed_time

    print("\n :")
    print("Average Latency:{} ms".format(sum(latencies)/len(latencies)))
    print("Throughput:{} requests/sec".format( throughput))
	# Close the socket when done
	# client_socket.close()
    print("Connection closed.")
