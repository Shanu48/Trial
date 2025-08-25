from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad

def hexKeyToBytes(hex_key):
    return bytes.fromhex(hex_key)

block_size = 16  # AES block size is 16 bytes

def aesEncrypt(plaintext, key):
    cipher = AES.new(key, AES.MODE_ECB)
    padded_text = pad(plaintext.encode(), block_size)
    ciphertext = cipher.encrypt(padded_text)
    return ciphertext

def aesDecrypt(ciphertext, key):
    cipher = AES.new(key, AES.MODE_ECB)
    decrypted_padded = cipher.decrypt(ciphertext)
    decrypted = unpad(decrypted_padded, block_size).decode()
    return decrypted

def menu():
    print("AES-128 Encryption / Decryption (ECB mode)")
    print("Key must be 32 hex digits (16 bytes).")
    choice = int(input("Enter your choice (1-Encrypt, 2-Decrypt): "))
    key_hex = input("Enter the key (32 hex digits): ")
    while len(key_hex) != 32:
        print("Error: Key must be exactly 32 hex digits.")
        key_hex = input("Enter the key (32 hex digits): ")
    key = hexKeyToBytes(key_hex)

    if choice == 1:
        plaintext = input("Enter plaintext: ")
        return choice, plaintext, key
    else:
        ciphertext_hex = input("Enter ciphertext (hex): ")
        return choice, ciphertext_hex, key

choice, text, key = menu()

if choice == 1:
    ciphertext = aesEncrypt(text, key)
    print("Plaintext:", text)
    print("Ciphertext (hex):", ciphertext.hex())
else:
    ciphertext_bytes = bytes.fromhex(text)
    plaintext = aesDecrypt(ciphertext_bytes, key)
    print("Ciphertext (hex):", text)
    print("Decrypted plaintext:", plaintext)
