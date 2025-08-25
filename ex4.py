"""
Encrypt the message "Classified Text" using Triple DES with the
key "1234567890ABCDEF1234567890ABCDEF1234567890ABCDEF".
Then decrypt the ciphertext to verify the original message
"""

from Crypto.Cipher import DES3
from Crypto.Util.Padding import pad, unpad
import binascii

def tripleDesEncrypt(plaintext, key):
    cipher = DES3.new(key, DES3.MODE_ECB)
    padded = pad(plaintext.encode(), DES3.block_size)
    return cipher.encrypt(padded)

def tripleDesDecrypt(ciphertext, key):
    cipher = DES3.new(key, DES3.MODE_ECB)
    decrypted_padded = cipher.decrypt(ciphertext)
    return unpad(decrypted_padded, DES3.block_size).decode()

def getKey():
    while True:
        key_input = input("Enter a 48-character hexadecimal Triple DES key (24 bytes): ")
        if len(key_input) == 48:
            try:
                key_bytes = binascii.unhexlify(key_input)
                # Validate key length
                if len(key_bytes) == 24:
                    return key_bytes
            except binascii.Error:
                pass
        print("Invalid key! Must be 48 hex characters (24 bytes). Try again.")

print("=== Triple DES Encryption and Decryption ===")

message = input("Enter the plaintext message: ")
key = getKey()

ciphertext = tripleDesEncrypt(message, key)
print("Ciphertext (hex):", ciphertext.hex())

decrypted = tripleDesDecrypt(ciphertext, key)
print("Decrypted text:", decrypted)
