import random

# 5-digit primes from previous example
p = 20207   # prime modulus
q = 10103   # prime divisor of p-1
g = 2       # generator

def diffie_hellman_keygen():
    """Generate private and public keys."""
    private_key = random.randint(1, q-1)
    public_key = pow(g, private_key, p)
    return private_key, public_key

def diffie_hellman_shared_secret(private_key, other_public_key):
    """Compute shared secret using own private key and other party's public key."""
    shared_secret = pow(other_public_key, private_key, p)
    return shared_secret

def main():
    print("=== Diffie-Hellman Key Exchange Demo ===")

    # Alice generates her keys
    alice_priv, alice_pub = diffie_hellman_keygen()
    print(f"Alice's public key: {alice_pub}")

    # Bob generates his keys
    bob_priv, bob_pub = diffie_hellman_keygen()
    print(f"Bob's public key: {bob_pub}")

    # Alice computes shared secret using Bob's public key
    alice_shared = diffie_hellman_shared_secret(alice_priv, bob_pub)

    # Bob computes shared secret using Alice's public key
    bob_shared = diffie_hellman_shared_secret(bob_priv, alice_pub)

    print(f"Alice's computed shared secret: {alice_shared}")
    print(f"Bob's computed shared secret: {bob_shared}")

    if alice_shared == bob_shared:
        print("Success! Shared secrets match.")
    else:
        print("Error! Shared secrets do not match.")

if __name__ == "__main__":
    main()