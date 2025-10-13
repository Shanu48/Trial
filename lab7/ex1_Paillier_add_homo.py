import random
from math import gcd


def lcm(a, b):
    return a * b // gcd(a, b)


def modinv(a, m):
    # Extended Euclidean Algorithm for modular inverse
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


def get_prime(bits):
    # Simple prime generation using sympy if available or naive method
    # For simplicity, we will do a basic primality test with random numbers (not cryptographically secure)
    def is_prime(n, k=5):
        if n <= 1:
            return False
        if n <= 3:
            return True
        if n % 2 == 0:
            return False

        # Miller-Rabin primality test
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

    while True:
        p = random.getrandbits(bits)
        # Ensure p is odd and of proper bit length
        p |= (1 << bits - 1) | 1
        if is_prime(p):
            return p


class Paillier:
    def __init__(self, bits=512):
        print("Generating p...")
        self.p = get_prime(bits)
        print("Generating q...")
        self.q = get_prime(bits)
        while self.q == self.p:
            self.q = get_prime(bits)
        self.n = self.p * self.q
        self.n_sq = self.n ** 2
        self.lambda_ = lcm(self.p - 1, self.q - 1)
        self.g = self.n + 1  # common choice for g
        # Calculate mu
        x = pow(self.g, self.lambda_, self.n_sq)
        l = (x - 1) // self.n
        self.mu = modinv(l, self.n)
        self.public_key = (self.n, self.g)
        self.private_key = (self.lambda_, self.mu)
        print("Key generation completed.")

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
        x = pow(c, self.lambda_, self.n_sq)
        l = (x - 1) // self.n
        m = (l * self.mu) % self.n
        return m

    def add_encrypted(self, c1, c2):
        # Homomorphic addition: multiply ciphertexts mod n^2
        return (c1 * c2) % self.n_sq


def main():
    print("Paillier Encryption Scheme Demo")
    bits = int(input("Enter number of bits for prime generation (e.g., 16 for testing, 512+ for security): "))
    paillier = Paillier(bits)

    m1 = int(input("Enter first integer (0 <= m < n): "))
    m2 = int(input("Enter second integer (0 <= m < n): "))

    if not (0 <= m1 < paillier.n) or not (0 <= m2 < paillier.n):
        print(f"Error: integers must be in range [0, {paillier.n})")
        return

    c1 = paillier.encrypt(m1)
    c2 = paillier.encrypt(m2)
    print(f"Encrypted {m1}: {c1}")
    print(f"Encrypted {m2}: {c2}")

    c_sum = paillier.add_encrypted(c1, c2)
    print(f"Encrypted sum (ciphertext): {c_sum}")

    decrypted_sum = paillier.decrypt(c_sum)
    print(f"Decrypted sum: {decrypted_sum}")

    print(
        f"Verification: {m1} + {m2} = {decrypted_sum} -> {'Correct' if decrypted_sum == (m1 + m2) % paillier.n else 'Incorrect'}")


if __name__ == "__main__":
    main()
