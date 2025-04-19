import ctypes

# Load compiled shared library
aes = ctypes.CDLL('./rijndael.so')

# Set argument, return types
aes.aes_encrypt_block.argtypes = [ctypes.POINTER(ctypes.c_ubyte), ctypes.POINTER(ctypes.c_ubyte)]
aes.aes_encrypt_block.restype = ctypes.POINTER(ctypes.c_ubyte)

aes.aes_decrypt_block.argtypes = [ctypes.POINTER(ctypes.c_ubyte), ctypes.POINTER(ctypes.c_ubyte)]
aes.aes_decrypt_block.restype = ctypes.POINTER(ctypes.c_ubyte)

#print result in hexadecimal format
def print_result(label, data):
    print(f"\n{label}:")
    print(' '.join(f'{b:02x}' for b in data))

test_cases = [
    {
        "plaintext": bytes([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]),
        "key": bytes([50, 20, 46, 86, 67, 9, 70, 27, 75, 17, 51, 17, 4, 8, 6, 99])
    },
    {
        "plaintext": bytes([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
        "key": bytes([1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1])
    },
    {
        "plaintext": bytes([255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255]),
        "key": bytes([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    },
    {
        "plaintext": bytes([8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128]),
        "key": bytes([128, 120, 112, 104, 96, 88, 80, 72, 64, 56, 48, 40, 32, 24, 16, 8])
    },
    {
        "plaintext": bytes([15, 31, 47, 63, 79, 95, 111, 127, 143, 159, 175, 191, 207, 223, 239, 255]),
        "key": bytes([255, 239, 223, 207, 191, 175, 159, 143, 127, 111, 95, 79, 63, 47, 31, 15])
    }
]

#running test cases
for idx, test_case in enumerate(test_cases, 1):
    plaintext = test_case["plaintext"]
    key = test_case["key"]

    plaintext_array = (ctypes.c_ubyte * 16)(*plaintext)
    key_array = (ctypes.c_ubyte * 16)(*key)

    # Encrypt
    ciphertext_ptr = aes.aes_encrypt_block(plaintext_array, key_array)
    ciphertext = [ciphertext_ptr[i] for i in range(16)]

    # Decrypt
    ciphertext_array = (ctypes.c_ubyte * 16)(*ciphertext)
    recovered_ptr = aes.aes_decrypt_block(ciphertext_array, key_array)
    recovered_plaintext = [recovered_ptr[i] for i in range(16)]

    print_result(f"Test Case {idx} - Plaintext", plaintext)
    print_result(f"Test Case {idx} - Ciphertext (Encrypted)", ciphertext)
    print_result(f"Test Case {idx} - Recovered Plaintext", recovered_plaintext)