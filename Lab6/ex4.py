"""
Use a Hill cipher to encipher the message "We live in an insecure world". Use the following key:
 𝐾 = [03 03
      02 07]
"""

import numpy as np

alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'aaaaa

def modInverse(a, m):
    # Find modular inverse of a under modulo m using Extended Euclidean Algorithm
    a = a % m
    for x in range(1, m):
        if (a * x) % m == 1:
            return x
    return None

def matrixModInverse(matrix, modulus):
    det = int(round(np.linalg.det(matrix))) % modulus
    det_inv = modInverse(det, modulus)
    if det_inv is None:
        return None
    # Adjugate matrix:
    adjugate = np.round(det * np.linalg.inv(matrix)).astype(int) % modulus
    # modular inverse of matrix:
    matrix_modulus_inv = (det_inv * adjugate) % modulus
    return matrix_modulus_inv

def prepareText(text, n):
    # Prepare text: uppercase, remove non-alpha, pad with 'X' to make length multiple of n
    text = ''.join(filter(str.isalpha, text.upper()))
    while len(text) % n != 0:
        text += 'X'
    return text

def textToNumbers(text):
    return [alphabet.index(c) for c in text]

def numbersToText(numbers):
    return ''.join(alphabet[n] for n in numbers)

def hillEncrypt(plaintext, key_matrix):
    n = key_matrix.shape[0]
    plaintext = prepareText(plaintext, n)
    plaintext_nums = textToNumbers(plaintext)
    ciphertext = []
    for i in range(0, len(plaintext_nums), n):
        block = np.array(plaintext_nums[i:i+n])
        encrypted_block = key_matrix.dot(block) % 26
        ciphertext.extend(encrypted_block)
    return numbersToText(ciphertext)

def hillDecrypt(ciphertext, key_matrix):
    n = key_matrix.shape[0]
    ciphertext = prepareText(ciphertext, n)
    ciphertext_nums = textToNumbers(ciphertext)
    inverse_key = matrixModInverse(key_matrix, 26)
    if inverse_key is None:
        return None  # Key matrix not invertible mod 26
    plaintext = []
    for i in range(0, len(ciphertext_nums), n):
        block = np.array(ciphertext_nums[i:i+n])
        decrypted_block = np.dot(inverse_key, block) % 26
        decrypted_block = decrypted_block.astype(int)
        plaintext.extend(decrypted_block)
    return numbersToText(plaintext)

def menu():
    print("HILL CIPHER (2x2 key matrix)")
    print("1. Encryption")
    print("2. Decryption")
    ch = int(input("Enter your choice: "))
    if ch == 1:
        text = input("Enter the plaintext: ")
    else:
        text = input("Enter the ciphertext: ")
    print("Enter the 2x2 key matrix row-wise (4 numbers, space separated):")
    key_nums = list(map(int, input().split()))
    key_matrix = np.array(key_nums).reshape(2,2)
    return ch, text, key_matrix

ch, text, key_matrix = menu()

if ch == 1:
    cipher = hillEncrypt(text, key_matrix)
    print("Plaintext:", text)
    print("Ciphertext:", cipher)
else:
    plain = hillDecrypt(text, key_matrix)
    if plain is None:
        print("Error: Key matrix is not invertible modulo 26, cannot decrypt.")
    else:
        print("Ciphertext:", text)
        print("Plaintext:", plain.replace("X",""))
