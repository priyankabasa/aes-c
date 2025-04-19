import ctypes

# Load compiled shared library
aes = ctypes.CDLL('./rijndael.so')

# Set argument, return types
aes.aes_encrypt_block.argtypes = [ctypes.POINTER(ctypes.c_ubyte), ctypes.POINTER(ctypes.c_ubyte)]
aes.aes_encrypt_block.restype = ctypes.POINTER(ctypes.c_ubyte)

aes.aes_decrypt_block.argtypes = [ctypes.POINTER(ctypes.c_ubyte), ctypes.POINTER(ctypes.c_ubyte)]
aes.aes_decrypt_block.restype = ctypes.POINTER(ctypes.c_ubyte)


plaintext = bytes([
     0x32, 0x88, 0x31, 0xe0,
    0x43, 0x5a, 0x31, 0x37,
    0xf6, 0x30, 0x98, 0x07,
    0xa8, 0x8d, 0xa2, 0x34
])

key = bytes([
    0x2b, 0x7e, 0x15, 0x16,
    0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88,
    0x09, 0xcf, 0x4f, 0x3c
])


plaintext_array = (ctypes.c_ubyte * 16)(*plaintext)
key_array = (ctypes.c_ubyte * 16)(*key)

# Encrypt
ciphertext_ptr = aes.aes_encrypt_block(plaintext_array, key_array)
ciphertext = [ciphertext_ptr[i] for i in range(16)]

# Decrypt
ciphertext_array = (ctypes.c_ubyte * 16)(*ciphertext)
recovered_ptr = aes.aes_decrypt_block(ciphertext_array, key_array)
recovered_plaintext = [recovered_ptr[i] for i in range(16)]


print("\nPlaintext:")
print(' '.join(f'{b:02x}' for b in plaintext))

print("\nCiphertext (Encrypted):")
print(' '.join(f'{b:02x}' for b in ciphertext))

print("\nRecovered Plaintext:")
print(' '.join(f'{b:02x}' for b in recovered_plaintext))