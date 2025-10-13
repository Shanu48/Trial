import socket
from Crypto.PublicKey import RSA
from Crypto.Signature import pkcs1_15
from Crypto.Hash import SHA256

# Generate RSA key pair for Alice
key = RSA.generate(2048)
private_key = key
public_key = key.publickey()

# User input message
message = input("Enter message to send securely: ")

# Sign message
h = SHA256.new(message.encode())
signature = pkcs1_15.new(private_key).sign(h)

# Connect to server
client_socket = socket.socket()
client_socket.connect(("localhost", 12345))

# Send message, signature, and Alice's public key
client_socket.sendall(f"{message}||{signature.hex()}||{public_key.export_key().decode()}".encode())
client_socket.close()

print("\n[Client] Message and signature sent to server.")