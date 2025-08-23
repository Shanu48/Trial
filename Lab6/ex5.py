"""
John is reading a mystery book involving cryptography.
In one part of the book, the author gives a ciphertext "CIW"
and two paragraphs later the author tells the reader that this is a shift cipher and the plaintext is "yes".
In the next chapter, the hero found a tablet in a cave with "XVIEWYWI" engraved on it.
John immediately found the actual meaning of the ciphertext.
Identify the type of attack and plaintext.
"""
alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

def caesar_decrypt(ciphertext, shift):
    plaintext = ""
    for char in ciphertext.upper():
        if char.isalpha():
            shifted = (ord(char) - ord('A') - shift) % 26
            plaintext += chr(shifted + ord('A'))
        else:
            plaintext += char
    return plaintext

def find_shift(known_plaintext, known_ciphertext):
    p = known_plaintext.upper()
    c = known_ciphertext.upper()
    shift = (ord(c[0]) - ord(p[0])) % 26
    for i in range(1, len(p)):
        if (ord(c[i]) - ord(p[i])) % 26 != shift:
            return None
    return shift

def menu():
    print("CAESAR CIPHER - Known Plaintext Attack")
    print("Enter the known plaintext:")
    known_plaintext = input().strip()
    print("Enter the corresponding known ciphertext:")
    known_ciphertext = input().strip()
    print("Enter the ciphertext to decrypt:")
    unknown_ciphertext = input().strip()
    return known_plaintext, known_ciphertext, unknown_ciphertext

known_plaintext, known_ciphertext, unknown_ciphertext = menu()

shift_key = find_shift(known_plaintext, known_ciphertext)
if shift_key is None:
    print("Could not find a consistent shift key with the given inputs.")
else:
    print(f"Identified shift key: {shift_key}")
    decrypted_message = caesar_decrypt(unknown_ciphertext, shift_key)
    print("Type of attack: Known plaintext attack")
    print(f"Decrypted message: {decrypted_message}")
