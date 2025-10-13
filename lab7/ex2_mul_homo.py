import random
from math import gcd

def extended_gcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = extended_gcd(b % a, a)
        return (g, x - (b // a) * y, y)

def modinv(a, m):
    g, x, y = extended_gcd(a, m)
    if g != 1:
        raise Exception('Modular inverse does not exist')
    else:
        return x % m

def is_prime(n, k=5):
    # Miller-Rabin primality test
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

def get_prime(bits):
    while True:
        p = random.getrandbits(bits)
        p |= (1 << bits - 1) | 1  # ensure p has proper bits and is odd
        if is_prime(p):
            return p

class RSA:
    def __init__(self, bits=512):
        print("Generating prime p...")
        self.p = get_prime(bits)
        print("Generating prime q...")
        self.q = get_prime(bits)
        while self.q == self.p:
            self.q = get_prime(bits)
        self.n = self.p * self.q
        self.phi = (self.p - 1) * (self.q - 1)

        # Choose e
        self.e = 65537
        if gcd(self.e, self.phi) != 1:
            # Find another e
            self.e = 3
            while gcd(self.e, self.phi) != 1:
                self.e += 2

        self.d = modinv(self.e, self.phi)

        self.public_key = (self.e, self.n)
        self.private_key = (self.d, self.n)

        print("RSA key generation completed.")

    def encrypt(self, m):
        if not (0 <= m < self.n):
            raise ValueError(f"Message {m} out of range [0, n)")
        c = pow(m, self.e, self.n)
        return c

    def decrypt(self, c):
        m = pow(c, self.d, self.n)
        return m

    def multiply_ciphertexts(self, c1, c2):
        # Homomorphic property of RSA: multiply ciphertexts mod n
        return (c1 * c2) % self.n

def main():
    print("RSA Encryption Scheme Demo")
    bits = int(input("Enter number of bits for prime generation (e.g., 16 for testing, 512+ for security): "))
    rsa = RSA(bits)

    m1 = int(input(f"Enter first integer (0 <= m < {rsa.n}): "))
    m2 = int(input(f"Enter second integer (0 <= m < {rsa.n}): "))

    if not (0 <= m1 < rsa.n) or not (0 <= m2 < rsa.n):
        print("Error: integers must be in range [0, n)")
        return

    c1 = rsa.encrypt(m1)
    c2 = rsa.encrypt(m2)
    print(f"Encrypted {m1}: {c1}")
    print(f"Encrypted {m2}: {c2}")

    c_product = rsa.multiply_ciphertexts(c1, c2)
    print(f"Encrypted product (ciphertext): {c_product}")

    decrypted_product = rsa.decrypt(c_product)
    print(f"Decrypted product: {decrypted_product}")

    expected_product = (m1 * m2) % rsa.n
    print(f"Verification: {m1} * {m2} mod n = {expected_product} -> {'Correct' if decrypted_product == expected_product else 'Incorrect'}")

if __name__ == "__main__":
    main()
