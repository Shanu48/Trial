def custom_hash(s: str) -> int:
    hash_value = 5381  # Initial hash value

    for char in s:
        # Multiply current hash by 33 and add ASCII value of char
        hash_value = (hash_value * 33) + ord(char)

        # Mix bits using bitwise XOR with shifted hash
        hash_value = hash_value ^ (hash_value >> 16)

    # Apply mask to keep hash within 32-bit range
    hash_value = hash_value & 0xFFFFFFFF

    return hash_value


# Example usage
a = input("Enter the plaintext to be hashed: ")
print(custom_hash(a))