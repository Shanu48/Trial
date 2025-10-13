import random
import hashlib

# -- Utilities --

def sha256_int(data: str) -> int:
    return int(hashlib.sha256(data.encode()).hexdigest(), 16)

def gcd(a, b):
    while b:
        a, b = b, a % b
    return a

def modinv(a, m):
    # Modular inverse using Extended Euclidean Algorithm
    m0 = m
    y = 0
    x = 1

    if m == 1:
        return 0

    while a > 1:
        q = a // m
        t = m

        m = a % m
        a = t
        t = y

        y = x - q * y
        x = t

    if x < 0:
        x += m0

    return x

# -- Global parameters (demo primes, replace with secure ones in real life) --

p = 65537
q = 65521  # q divides p-1
g = 2

# ---------- Schnorr Signature ----------

def schnorr_keygen():
    x = random.randint(1, q-1)  # private key
    y = pow(g, x, p)            # public key
    return x, y

def schnorr_sign(msg: str, x: int):
    k = random.randint(1, q-1)
    r = pow(g, k, p) % q
    e = sha256_int(str(r) + msg) % q
    s = (k + x*e) % q
    return (e, s)

def schnorr_verify(msg: str, signature: tuple, y: int):
    e, s = signature
    r = (pow(g, s, p) * pow(y, q - e, p)) % p % q
    e_ver = sha256_int(str(r) + msg) % q
    return e == e_ver

# ---------- ElGamal Signature ----------

def elgamal_keygen():
    x = random.randint(1, p-2)  # private key
    y = pow(g, x, p)            # public key
    return x, y

def elgamal_sign(msg: str, x: int):
    H = sha256_int(msg)
    while True:
        k = random.randint(1, p-2)
        if gcd(k, p-1) == 1:
            break
    r = pow(g, k, p)
    k_inv = modinv(k, p-1)
    s = (k_inv * (H - x * r)) % (p-1)
    return (r, s)

def elgamal_verify(msg: str, signature: tuple, y: int):
    r, s = signature
    if not (0 < r < p):
        return False
    H = sha256_int(msg)
    v1 = (pow(y, r, p) * pow(r, s, p)) % p
    v2 = pow(g, H, p)
    return v1 == v2

# ---------- Helper to parse tuple inputs ----------

def parse_tuple_input(prompt):
    while True:
        s = input(prompt).strip()
        try:
            # Expected input format: (num1, num2) or (num1,)
            t = eval(s, {"__builtins__": None}, {})
            if isinstance(t, tuple):
                return t
            else:
                print("Please enter a tuple, e.g. (12345, 67890)")
        except:
            print("Invalid input. Please enter tuple in proper format.")

# ---------- Main menu ----------

def main():
    while True:
        print("\n--- Digital Signature using ElGamal & Schnorr ---")
        choice = input("1. Sign a message\n2. Verify a signature\n3. Exit\nChoose (1-3): ").strip()

        if choice == '1':
            msg = input("\nEnter message to sign: ")
            sp, spub = schnorr_keygen()
            ssig = schnorr_sign(msg, sp)
            ep, epub = elgamal_keygen()
            esig = elgamal_sign(msg, ep)

            print(f"\n--- Signing Results ---\nMessage: {msg}\n")
            print(f"Schnorr Signature: {ssig}\nSchnorr Public Key: {spub}\nSchnorr Private Key (keep secret!): {sp}\n")
            print(f"ElGamal Signature: {esig}\nElGamal Public Key: {epub}\nElGamal Private Key (keep secret!): {ep}\n")
            print("Send the message, public keys, and signatures to the verifier.\n")

        elif choice == '2':
            msg = input("\nEnter message to verify: ")
            ssig = parse_tuple_input("Enter Schnorr signature (e, s) as tuple: ")
            spub = int(input("Enter Schnorr public key (integer): "))
            esig = parse_tuple_input("Enter ElGamal signature (r, s) as tuple: ")
            epub = int(input("Enter ElGamal public key (integer): "))

            print(f"\nVerifying Schnorr signature...\nSchnorr signature is {'VALID.' if schnorr_verify(msg, ssig, spub) else 'INVALID.'}")
            print(f"\nVerifying ElGamal signature...\nElGamal signature is {'VALID.' if elgamal_verify(msg, esig, epub) else 'INVALID.'}")

        elif choice == '3':
            print("Exiting...")
            break

        else:
            print("Invalid choice, please enter 1, 2 or 3.")

if __name__ == "__main__":
    main()