/*
 *Name: Sai Priyanka Basa Shanker  
 * Student Number: D24125575
 *
 * Description:
 * This is a test program that demonstrates AES-128 encryption and decryption.
 * 1. Creates sample plaintext (1-16) and key (mixed values)
 * 2. Encrypts the plaintext using aes_encrypt_block()
 * 3. Decrypts the ciphertext back using aes_decrypt_block()
 * 4. Prints all three versions (original, encrypted, decrypted) in:
 *    - 4x4 grid format (decimal numbers)
 *    - Hexadecimal values
 */

#include <stdio.h>
#include <stdlib.h>
#include "rijndael.h"

void print_128bit_block(unsigned char *block, const char *label) {
    printf("\n%s\n", label);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%3d ", block[i * 4 + j]);
        }
        printf("\n");
    }
}

void print_hex_block(unsigned char *block) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02x ", block[i]);
    }
    printf("\n");
}

int main() {
    unsigned char plaintext[BLOCK_SIZE] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };

    unsigned char key[BLOCK_SIZE] = {
      111, 222, 33, 44,
      55, 66, 77, 88,
      99, 100, 111, 123,
      132, 143, 154, 165
    };

    unsigned char *ciphertext = aes_encrypt_block(plaintext, key);
    unsigned char *recovered = aes_decrypt_block(ciphertext, key);

    print_128bit_block(plaintext, "\033[1;32mOriginal Plaintext:\033[0m");
    printf("\nHex: "); print_hex_block(plaintext);

    print_128bit_block(ciphertext, "\033[1;34mCiphertext (Encrypted):\033[0m");
    printf("\nHex: "); print_hex_block(ciphertext);

    print_128bit_block(recovered, "\033[1;33mRecovered Plaintext:\033[0m");
    printf("\nHex: "); print_hex_block(recovered);

    free(ciphertext);
    free(recovered);
    return 0;
}