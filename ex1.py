"""
Encrypt the message "Confidential Data" using DES with the following key: "A1B2C3D4".
Then decrypt the ciphertext to verify the original message.
"""
from Crypto.Cipher import DES
from Crypto.Util.Padding import pad, unpad

block_size = 8  # DES block size

def desEncrypt(plaintext, key):
    cipher = DES.new(key, DES.MODE_ECB)
    padded_text = pad(plaintext.encode(), block_size)
    ciphertext = cipher.encrypt(padded_text)
    return ciphertext

def desDecrypt(ciphertext, key):
    cipher = DES.new(key, DES.MODE_ECB)
    decrypted_padded = cipher.decrypt(ciphertext)
    decrypted = unpad(decrypted_padded, block_size).decode()
    return decrypted

def menu():
    print("DES Encryption / Decryption")
    print("Key must be exactly 8 characters long.")
    print("1. Encrypt")
    print("2. Decrypt")
    choice = int(input("Enter your choice: "))
    key_input = input("Enter key (8 characters): ")
    while len(key_input) != 8:
        print("Error: Key must be exactly 8 characters.")
        key_input = input("Enter key (8 characters): ")
    key = key_input.encode()

    if choice == 1:
        plaintext = input("Enter plaintext: ")
        return choice, plaintext, key
    else:
        ciphertext_hex = input("Enter ciphertext (hex): ")
        return choice, ciphertext_hex, key

choice, text, key = menu()

if choice == 1:
    ciphertext = desEncrypt(text, key)
    print("Plaintext:", text)
    print("Ciphertext (hex):", ciphertext.hex())
else:
    ciphertext_bytes = bytes.fromhex(text)
    plaintext = desDecrypt(ciphertext_bytes, key)
    print("Ciphertext (hex):", text)
    print("Decrypted plaintext:", plaintext)
