"""
Compare the encryption and decryption times for DES and AES-256
for the message "Performance Testing of Encryption Algorithms".
Use a standard implementation and report your findings.
"""
from Crypto.Cipher import DES, AES
from Crypto.Util.Padding import pad, unpad
import time

def desEncrypt(plaintext, key):
    cipher = DES.new(key, DES.MODE_ECB)
    padded = pad(plaintext.encode(), DES.block_size)
    return cipher.encrypt(padded)

def desDecrypt(ciphertext, key):
    cipher = DES.new(key, DES.MODE_ECB)
    decrypted_padded = cipher.decrypt(ciphertext)
    return unpad(decrypted_padded, DES.block_size).decode()

def aesEncrypt(plaintext, key):
    cipher = AES.new(key, AES.MODE_ECB)
    padded = pad(plaintext.encode(), AES.block_size)
    return cipher.encrypt(padded)

def aesDecrypt(ciphertext, key):
    cipher = AES.new(key, AES.MODE_ECB)
    decrypted_padded = cipher.decrypt(ciphertext)
    return unpad(decrypted_padded, AES.block_size).decode()

def measureTime(func, *args):
    start = time.perf_counter_ns()
    result = func(*args)
    end = time.perf_counter_ns()
    return result, end - start  # Return time in nanoseconds


print("=== Performance Testing of Encryption Algorithms ===")

message = input("Enter the message to encrypt: ")

# DES key input and validation
while True:
    des_key_input = input("Enter an 8-character DES key: ")
    if len(des_key_input) == 8:
        des_key = des_key_input.encode()
        break
    else:
        print("DES key must be exactly 8 characters.")

# AES-256 key input and validation
while True:
    aes_key_input = input("Enter a 32-character AES-256 key (hex or ASCII): ")
    if len(aes_key_input) == 32:
        aes_key = aes_key_input.encode()
        break
    else:
        print("AES-256 key must be exactly 32 characters.")

print("\n=== DES Encryption/Decryption ===")
des_ciphertext, des_enc_time = measureTime(desEncrypt, message, des_key)
des_plaintext, des_dec_time = measureTime(desDecrypt, des_ciphertext, des_key)
print(f"Encrypted (hex): {des_ciphertext.hex()}")
print(f"Encryption time: {des_enc_time:.2f} nanoseconds")
print(f"Decryption time: {des_dec_time:.2f} nanoseconds")
print(f"Decrypted text: {des_plaintext}")

print("\n=== AES-256 Encryption/Decryption ===")
aes_ciphertext, aes_enc_time = measureTime(aesEncrypt, message, aes_key)
aes_plaintext, aes_dec_time = measureTime(aesDecrypt, aes_ciphertext, aes_key)
print(f"Encrypted (hex): {aes_ciphertext.hex()}")
print(f"Encryption time: {aes_enc_time:.2f} nanoseconds")
print(f"Decryption time: {aes_dec_time:.2f} nanoseconds")
print(f"Decrypted text: {aes_plaintext}")
