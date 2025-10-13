import random
from math import gcd
import hashlib

# Paillier Implementation (based on previous example)
def lcm(a, b):
    return a * b // gcd(a, b)

def modinv(a, m):
    g, x, y = extended_gcd(a, m)
    if g != 1:
        raise Exception('Modular inverse does not exist')
    else:
        return x % m

def extended_gcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = extended_gcd(b % a, a)
        return (g, x - (b // a) * y, y)

def get_prime(bits=16):
    # Simple prime generator for demo (not cryptographically secure)
    while True:
        p = random.getrandbits(bits)
        p |= (1 << bits - 1) | 1
        if is_prime(p):
            return p

def is_prime(n, k=5):
    if n <= 1:
        return False
    if n <= 3:
        return True
    if n % 2 == 0:
        return False

    def miller_rabin_test(d, n):
        a = random.randint(2, n - 2)
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            return True
        while d != n - 1:
            x = (x * x) % n
            d *= 2
            if x == 1:
                return False
            if x == n - 1:
                return True
        return False

    d = n - 1
    while d % 2 == 0:
        d //= 2

    for _ in range(k):
        if not miller_rabin_test(d, n):
            return False
    return True

class Paillier:
    def __init__(self, bits=64):
        self.p = get_prime(bits)
        self.q = get_prime(bits)
        while self.q == self.p:
            self.q = get_prime(bits)
        self.n = self.p * self.q
        self.n_sq = self.n ** 2
        self.lambda_ = lcm(self.p - 1, self.q - 1)
        self.g = self.n + 1
        x = pow(self.g, self.lambda_, self.n_sq)
        l = (x - 1) // self.n
        self.mu = modinv(l, self.n)
        self.public_key = (self.n, self.g)
        self.private_key = (self.lambda_, self.mu)

    def encrypt(self, m):
        if not (0 <= m < self.n):
            raise ValueError(f"Message {m} out of range [0, n)")
        while True:
            r = random.randrange(1, self.n)
            if gcd(r, self.n) == 1:
                break
        c = (pow(self.g, m, self.n_sq) * pow(r, self.n, self.n_sq)) % self.n_sq
        return c

    def decrypt(self, c):
        x = pow(c, self.private_key[0], self.n_sq)
        l = (x - 1) // self.n
        m = (l * self.private_key[1]) % self.n
        return m

# Hash function to convert words into integers
def word_to_int(word, n):
    h = hashlib.sha256(word.encode()).digest()
    return int.from_bytes(h, 'big') % n

# 2a. Dataset
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

# 2c. Build encrypted inverted index
def build_inverted_index(docs):
    inverted_index = {}
    for doc_id, text in docs.items():
        for word in text.lower().split():
            if word not in inverted_index:
                inverted_index[word] = set()
            inverted_index[word].add(doc_id)
    return inverted_index

def encrypt_index(index, paillier):
    encrypted_index = {}
    for word, doc_ids in index.items():
        # Encrypt word as integer hash
        word_int = word_to_int(word, paillier.n)
        encrypted_word = paillier.encrypt(word_int)
        # Encrypt each document ID
        encrypted_doc_ids = [paillier.encrypt(doc_id) for doc_id in doc_ids]
        encrypted_index[encrypted_word] = encrypted_doc_ids
    return encrypted_index

# 2d. Search
def search(query, paillier, encrypted_index, docs):
    query_int = word_to_int(query.lower(), paillier.n)
    encrypted_query = paillier.encrypt(query_int)
    # Search encrypted query in encrypted index keys by decrypting keys (inefficient but demo purpose)
    for encrypted_word, encrypted_doc_ids in encrypted_index.items():
        decrypted_word_int = paillier.decrypt(encrypted_word)
        if decrypted_word_int == query_int:
            # Decrypt doc IDs
            doc_ids = [paillier.decrypt(c) for c in encrypted_doc_ids]
            print(f"Documents containing '{query}':")
            for doc_id in doc_ids:
                print(f"Doc {doc_id}: {docs[doc_id]}")
            return
    print(f"No documents found containing '{query}'.")

def main():
    print("Generating Paillier keys...")
    paillier = Paillier(bits=64)  # small bits for demo; use bigger in practice
    print("Building inverted index...")
    inverted_index = build_inverted_index(documents)
    print("Encrypting inverted index...")
    encrypted_index = encrypt_index(inverted_index, paillier)

    while True:
        query = input("\nEnter search query (or 'exit' to quit): ").strip()
        if query.lower() == 'exit':
            break
        search(query, paillier, encrypted_index, documents)

if __name__ == "__main__":
    main()
