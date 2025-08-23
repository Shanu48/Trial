"""
Use a brute-force attack to decipher the following message.
Assume that you know it is an affine cipher and that the plaintext "ab" is enciphered to "GL":
XPALASXYFGFUKPXUSOGEUTKCDGEXANMGNVS
"""

alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'


def modInverse(a, m):
    a = a % m
    for x in range(1, m):
        if (a * x) % m == 1:
            return x
    return None


def affine_decrypt(ciphertext, a, b):
    a_inv = modInverse(a, 26)
    if a_inv is None:
        return None
    plaintext = ""
    for char in ciphertext:
        if char.isalpha():
            y = alphabet.index(char)
            x = (a_inv * (y - b)) % 26
            plaintext += alphabet[x]
        else:
            plaintext += char
    return plaintext


def find_key(plaintext_pair, ciphertext_pair):
    p1, p2 = [alphabet.index(c) for c in plaintext_pair]
    c1, c2 = [alphabet.index(c) for c in ciphertext_pair]

    for a in range(1, 26, 2):  # a must be coprime with 26, so try odd numbers excluding 13
        if a == 13:
            continue
        a_inv = modInverse(a, 26)
        if a_inv is None:
            continue

        # Solve for b: c1 = (a * p1 + b) mod 26 => b = (c1 - a*p1) mod 26
        b = (c1 - a * p1) % 26
        # Check if second letter matches: c2 == (a*p2 + b) mod 26
        if (a * p2 + b) % 26 == c2:
            return a, b
    return None, None


def menu():
    print("AFFINE CIPHER BRUTE-FORCE WITH KNOWN PLAINTEXT- CIPHERTEXT PAIR")
    print("Known plaintext (2 letters):")
    pt_pair = input().strip().upper()
    print("Corresponding ciphertext (2 letters):")
    ct_pair = input().strip().upper()
    print("Ciphertext to decrypt:")
    ciphertext = input().strip().upper()
    return pt_pair, ct_pair, ciphertext


pt_pair, ct_pair, ciphertext = menu()

a, b = find_key(pt_pair, ct_pair)
if a is None:
    print("No valid key found.")
else:
    print(f"Found key: a = {a}, b = {b}")
    decrypted = affine_decrypt(ciphertext, a, b)
    print("Decrypted message:")
    print(decrypted)
