import socket
import sys

# python script for client for easy of testing
# Create a UDS socket
sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = './test'
print(server_address[:-1])
print('connecting to %s' % server_address)
try:
    sock.connect(server_address[:-1])
    message = 'get world!'
    print('sending:', message)
    sock.sendall(message.encode())
    amount_received = 0
    amount_expected = len(message)
    data = sock.recv(18)
    # print('received "%s"' % data.decode())

    # while amount_received < amount_expected:
    #     data = sock.recv(18)
    #     amount_received += len(data)
    #     print('received "%s"' % data)

except socket.error:
    print(socket.error)
    sys.exit(1)

