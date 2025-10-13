from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding
from cryptography.hazmat.backends import default_backend
import os

# 1a. Dataset
documents = {
    1: "apple banana fruit",
    2: "banana orange juice",
    3: "juice fruit salad",
    4: "apple pie recipe",
    5: "orange fruit vitamin",
    6: "fruit salad recipe",
    7: "banana smoothie recipe",
    8: "vitamin supplement fruit",
    9: "pie baking recipe",
    10: "smoothie apple juice",
}

# 1b. AES encryption/decryption functions
class AESCipher:
    def __init__(self, key):
        self.key = key
        self.block_size = algorithms.AES.block_size // 8  # block size in bytes

    def encrypt(self, plaintext):
        iv = os.urandom(self.block_size)
        cipher = Cipher(algorithms.AES(self.key), modes.CBC(iv), backend=default_backend())
        encryptor = cipher.encryptor()

        # Padding plaintext to block size
        padder = padding.PKCS7(algorithms.AES.block_size).padder()
        padded = padder.update(plaintext.encode()) + padder.finalize()

        ciphertext = encryptor.update(padded) + encryptor.finalize()
        return iv + ciphertext  # prepend IV for use in decryption

    def decrypt(self, ciphertext):
        iv = ciphertext[:self.block_size]
        actual_ciphertext = ciphertext[self.block_size:]
        cipher = Cipher(algorithms.AES(self.key), modes.CBC(iv), backend=default_backend())
        decryptor = cipher.decryptor()

        padded_plaintext = decryptor.update(actual_ciphertext) + decryptor.finalize()

        # Remove padding
        unpadder = padding.PKCS7(algorithms.AES.block_size).unpadder()
        plaintext = unpadder.update(padded_plaintext) + unpadder.finalize()
        return plaintext.decode()

# 1c. Create inverted index
def build_inverted_index(docs):
    inverted_index = {}
    for doc_id, text in docs.items():
        for word in text.lower().split():
            if word not in inverted_index:
                inverted_index[word] = set()
            inverted_index[word].add(doc_id)
    return inverted_index

# Encrypt the inverted index
def encrypt_index(inverted_index, cipher):
    encrypted_index = {}
    for word, doc_ids in inverted_index.items():
        encrypted_word = cipher.encrypt(word)
        # Encrypt document IDs as comma-separated string
        doc_ids_str = ','.join(map(str, doc_ids))
        encrypted_doc_ids = cipher.encrypt(doc_ids_str)
        encrypted_index[encrypted_word] = encrypted_doc_ids
    return encrypted_index

# 1d. Search function
def search(query, cipher, encrypted_index, docs):
    encrypted_query = cipher.encrypt(query.lower())
    # Search for encrypted query in encrypted_index keys
    for encrypted_word in encrypted_index:
        # decrypt word to compare
        decrypted_word = cipher.decrypt(encrypted_word)
        if decrypted_word == query.lower():
            # decrypt document IDs
            encrypted_doc_ids = encrypted_index[encrypted_word]
            decrypted_doc_ids_str = cipher.decrypt(encrypted_doc_ids)
            doc_ids = list(map(int, decrypted_doc_ids_str.split(',')))
            # fetch and display documents
            print(f"Documents matching '{query}':")
            for doc_id in doc_ids:
                print(f"Doc {doc_id}: {docs[doc_id]}")
            return
    print(f"No documents found for query '{query}'.")

def main():
    key = os.urandom(32)  # AES-256 key
    cipher = AESCipher(key)

    print("Building inverted index...")
    inverted_index = build_inverted_index(documents)

    print("Encrypting inverted index...")
    encrypted_index = encrypt_index(inverted_index, cipher)

    # Let's test with a few search queries
    while True:
        query = input("\nEnter search query (or 'exit' to quit): ").strip()
        if query.lower() == 'exit':
            break
        search(query, cipher, encrypted_index, documents)

if __name__ == "__main__":
    main()