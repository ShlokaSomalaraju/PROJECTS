from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import RemoteController
from mininet.cli import CLI
from mininet.log import setLogLevel
from mininet.clean import cleanup

cleanup()

class CustomTopo(Topo):
    def build(self):
        # Add middle host (router)
        middle_host = self.addHost('h1')

        # Add clients
        client1 = self.addHost('c1')
        client2 = self.addHost('c2')
        client3 = self.addHost('c3')
        client4 = self.addHost('c4')
        client5 = self.addHost('c5')
        client6 = self.addHost('c6')

        # Add servers
        server1 = self.addHost('server1')
        server2 = self.addHost('server2')
        server3 = self.addHost('server3')
        server4 = self.addHost('server4')

        # Add two switches
        client_switch = self.addSwitch('s1')
        server_switch = self.addSwitch('s2')

        # Connect clients to client switch
        self.addLink(client1, client_switch)
        self.addLink(client2, client_switch)
        self.addLink(client3, client_switch)
        self.addLink(client4, client_switch)
        self.addLink(client5, client_switch)
        self.addLink(client6, client_switch)

        # Connect servers to server switch
        self.addLink(server1, server_switch)
        self.addLink(server2, server_switch)
        self.addLink(server3, server_switch)
        self.addLink(server4, server_switch)


        # Connect the middle host to both switches
        self.addLink(middle_host, client_switch)
        self.addLink(middle_host, server_switch)

def setup():
    # Set log level
    setLogLevel('info')

    # Create network and controller
    topo = CustomTopo()
    net = Mininet(topo=topo)

    # Start network
    net.start()

    # Enable IP forwarding on the middle host
    middle_host = net.get('h1')
    middle_host.cmd('sysctl -w net.ipv4.ip_forward=1')

    # Get network nodes
    client1 = net.get('c1')
    client2 = net.get('c2')
    client3 = net.get('c3')
    client4 = net.get('c4')
    client5 = net.get('c5')
    client6 = net.get('c6')

    server1 = net.get('server1')
    server2 = net.get('server2')
    server3 = net.get('server3')
    server4 = net.get('server4')


    # Assign IPs
    # Client subnet
    client1.setIP('10.0.1.1', 24)
    client2.setIP('10.0.1.2', 24)
    client3.setIP('10.0.1.3', 24)
    client4.setIP('10.0.1.4', 24)
    client5.setIP('10.0.1.5', 24)
    client6.setIP('10.0.1.6', 24)

    middle_host.setIP('10.0.1.254', 24, intf='h1-eth0')

    # Server subnet
    server1.setIP('10.0.2.1', 24)
    server2.setIP('10.0.2.2', 24)
    server3.setIP('10.0.2.3', 24)
    server4.setIP('10.0.2.4',24)
    middle_host.setIP('10.0.2.254', 24, intf='h1-eth1')

    # Add default routes for clients and servers
    client1.cmd('ip route add default via 10.0.1.254')
    client2.cmd('ip route add default via 10.0.1.254')
    client3.cmd('ip route add default via 10.0.1.254')
    client4.cmd('ip route add default via 10.0.1.254')
    client5.cmd('ip route add default via 10.0.1.254')
    client6.cmd('ip route add default via 10.0.1.254')
    server1.cmd('ip route add default via 10.0.2.254')
    server2.cmd('ip route add default via 10.0.2.254')
    server3.cmd('ip route add default via 10.0.2.254')
    server4.cmd('ip route add default via 10.0.2.254')
    clients = [net.get('c{}'.format(i)) for i in range(1, 6)]
    servers = [net.get('server{}'.format(i)) for i in range(1, 4)]

    # Test connectivity
    net.pingAll()
    # middle_host.cmd('xterm -hold -e "python3 Response_time_lb.py "&')
    # for i, server in enumerate(servers, start=1):
    #     server.cmd('xterm -hold -e "python3 server{}.py" &'.format(i))
    # for client in clients:
    #     client.cmd('xterm -hold -e "python3 client.py" &')  # Run client.py in the background

    # Allow some time for scripts to run
    # time.sleep(5)

    # Open CLI for interaction
    CLI(net)

    # Stop network
    net.stop()

if __name__ == '__main__':
    setup()
