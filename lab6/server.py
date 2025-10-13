import socket
from Crypto.PublicKey import RSA
from Crypto.Signature import pkcs1_15
from Crypto.Hash import SHA256

# Generate RSA key pair (for verification, public key is shared)
key = RSA.generate(2048)
public_key = key.publickey()

print("Server Public Key (Share this with client):")
print(public_key.export_key().decode())

# Start socket server
server_socket = socket.socket()
server_socket.bind(("localhost", 12345))
server_socket.listen(1)
print("\n[Server] Waiting for client...")

conn, addr = server_socket.accept()
print("[Server] Connected by", addr)

# Receive message and signature
data = conn.recv(8192).decode()
message, signature_hex, pub_key_str = data.split("||")

# Load Alice's public key
alice_pub_key = RSA.import_key(pub_key_str.encode())
signature = bytes.fromhex(signature_hex)

# Verify signature
h = SHA256.new(message.encode())
try:
    pkcs1_15.new(alice_pub_key).verify(h, signature)
    print("\nSignature is valid")
    print("Message received:", message)
except (ValueError, TypeError):
    print("\nSignature is invalid")

conn.close()