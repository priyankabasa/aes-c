/*
 *Name: Sai Priyanka Basa Shanker  
 * Student Number: D24125575
 *
 * Description:
 * This header file (rijndael.h) defines the interface for AES-128 block encryption 
 * and decryption. It contains:
 *
 * 1. Basic Definitions:
 *    - BLOCK_SIZE (16): Sets the AES block size to 128 bits (16 bytes)
 *    - BLOCK_ACCESS macro: Provides easy access to individual bytes in the 4x4 state matrix
 * 
 * 2. Function Declarations:
 *    - aes_encrypt_block(): Encrypts one 128-bit block using a 128-bit key
 *    - aes_decrypt_block(): Decrypts one 128-bit block using a 128-bit key
 */

#ifndef RIJNDAEL_H
#define RIJNDAEL_H

#define BLOCK_ACCESS(block, row, col) (block[(row * 4) + col])
#define BLOCK_SIZE 16

/*
 * These should be the main encrypt/decrypt functions (i.e. the main
 * entry point to the library for programmes hoping to use it to
 * encrypt or decrypt data)
 */
unsigned char *aes_encrypt_block(unsigned char *plaintext, unsigned char *key);
unsigned char *aes_decrypt_block(unsigned char *ciphertext, unsigned char *key);

#endif
