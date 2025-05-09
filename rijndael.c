/*
 * Name: Sai Priyanka Basa Shanker  
 * Student Number: D24125575 
 *  
 * Description:  
 * This code implements the Advanced Encryption Standard (AES), specifically the Rijndael  
 * algorithm, for 128-bit block encryption and decryption. AES is a symmetric-key algorithm,  
 * meaning the same key is used for both encryption and decryption. This implementation  
 * supports a 128-bit (16-byte) key and operates on 128-bit (16-byte) blocks of data.  
 *  
 * The code consists of the following 
 *  
 * 1. Substitution Boxes (S-Boxes):  
 *    - s_box[256]: Precomputed lookup table for byte substitution during encryption.  
 *    - inv_s_box[256]: Inverse substitution box for decryption.  
 *    These provide non-linearity to the cipher, making it resistant to cryptanalysis.  
 *  
 * 2. Key Expansion:  
 *    - expand_key(): Generates 11 round keys (176 bytes) from the original 128-bit cipher key.  
 *    - Uses Rijndael's key schedule algorithm with XOR operations and S-box substitutions.  
 *    - Includes round constants (rcon) to eliminate symmetries in the key expansion.  
 *  
 * 3. Encryption Functions:  
 *    - sub_bytes(): Replaces each byte in the state using the S-box.  
 *    - shift_rows(): Cyclically shifts rows of the state matrix.  
 *    - mix_columns(): Mixes columns using finite field multiplication for diffusion.  
 *    - add_round_key(): XORs the state with the round key.  
 *    - aes_encrypt_block(): Executes 10 rounds of AES encryption (initial round,  
 *      8 main rounds with all operations, and a final round without mix_columns).  
 *  
 * 4. Decryption Functions:  
 *    - invert_sub_bytes(): Uses the inverse S-box to reverse byte substitution.  
 *    - invert_shift_rows(): Reverses the row shifting operation.  
 *    - invert_mix_columns(): Reverses the column mixing using inverse coefficients.  
 *    - aes_decrypt_block(): Executes inverse operations in reverse order for decryption.  
 *  
 * 5. Helper Functions:  
 *    - gmul(): Performs finite field (Galois Field) multiplication used in mix_columns.  
 *    - BLOCK_ACCESS macro: Provides convenient access to the state matrix elements.  
 *  
 * The encryption/decryption follows the standard AES structure:  
 *   - Encryption: AddRoundKey → (SubBytes → ShiftRows → MixColumns → AddRoundKey)*9 →  
 *                (SubBytes → ShiftRows → AddRoundKey)  
 *   - Decryption: AddRoundKey → InvShiftRows → InvSubBytes → (AddRoundKey →  
 *                InvMixColumns → InvShiftRows → InvSubBytes)*9 → AddRoundKey  
 */

 #include <stdlib.h>
 #include <string.h>   
 #include <stdio.h>
 #include "rijndael.h"
 
   static const unsigned char s_box[256] = {
     0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
     0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
     0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
     0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
     0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
     0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
     0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
     0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
     0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
     0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,  
     0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
     0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
     0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
     0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
     0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
     0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
     0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
     0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
     0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
     0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
     0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
     0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
     0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
     0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
     0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
     0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
     0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
     0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
     0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
     0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
     0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
     0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
   };
   
   static const unsigned char inv_s_box[256] = {
     0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
     0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
     0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
     0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
     0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
     0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
     0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
     0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
     0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
     0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
     0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
     0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
     0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
     0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
     0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
     0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
     0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
     0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
     0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
     0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
     0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
     0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
     0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
     0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
     0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
     0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
     0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
     0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
     0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
     0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
     0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
     0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
   };
   
 // Helper functions for MixColumns
 static unsigned char gmul(unsigned char a, unsigned char b) {
   unsigned char p = 0;
   for (int i = 0; i < 8; i++) {
     if (b & 1) p ^= a;
     unsigned char hi_bit_set = a & 0x80;
     a <<= 1;
     if (hi_bit_set) a ^= 0x1b; // Rijndael polynomial
     b >>= 1;
   }
   return p;
 }
 
 void sub_bytes(unsigned char *block) {
   for (int i = 0; i < BLOCK_SIZE; i++) {
     block[i] = s_box[block[i]];
   }
 }
 
 void shift_rows(unsigned char *block) {
   unsigned char temp;
 
   // Row 1 shift left by 1
   temp = BLOCK_ACCESS(block, 1, 0);
   BLOCK_ACCESS(block, 1, 0) = BLOCK_ACCESS(block, 1, 1);
   BLOCK_ACCESS(block, 1, 1) = BLOCK_ACCESS(block, 1, 2);
   BLOCK_ACCESS(block, 1, 2) = BLOCK_ACCESS(block, 1, 3);
   BLOCK_ACCESS(block, 1, 3) = temp;
 
   // Row 2 shift left by 2
   temp = BLOCK_ACCESS(block, 2, 0);
   BLOCK_ACCESS(block, 2, 0) = BLOCK_ACCESS(block, 2, 2);
   BLOCK_ACCESS(block, 2, 2) = temp;
   temp = BLOCK_ACCESS(block, 2, 1);
   BLOCK_ACCESS(block, 2, 1) = BLOCK_ACCESS(block, 2, 3);
   BLOCK_ACCESS(block, 2, 3) = temp;
 
   // Row 3 shift left by 3 (same as shift right by 1)
   temp = BLOCK_ACCESS(block, 3, 3);
   BLOCK_ACCESS(block, 3, 3) = BLOCK_ACCESS(block, 3, 2);
   BLOCK_ACCESS(block, 3, 2) = BLOCK_ACCESS(block, 3, 1);
   BLOCK_ACCESS(block, 3, 1) = BLOCK_ACCESS(block, 3, 0);
   BLOCK_ACCESS(block, 3, 0) = temp;
 }
 
 void mix_columns(unsigned char *block) {
   for (int c = 0; c < 4; ++c) {
     unsigned char *col = &block[c * 4];
     unsigned char a0 = col[0], a1 = col[1], a2 = col[2], a3 = col[3];
     col[0] = gmul(a0, 2) ^ gmul(a1, 3) ^ a2 ^ a3;
     col[1] = a0 ^ gmul(a1, 2) ^ gmul(a2, 3) ^ a3;
     col[2] = a0 ^ a1 ^ gmul(a2, 2) ^ gmul(a3, 3);
     col[3] = gmul(a0, 3) ^ a1 ^ a2 ^ gmul(a3, 2);
   }
 }
 
 
 void invert_sub_bytes(unsigned char *block) {
   for (int i = 0; i < BLOCK_SIZE; i++) {
     block[i] = inv_s_box[block[i]];
 }
 }
 
 void invert_shift_rows(unsigned char *block) {
   unsigned char temp;
 
   // Row 1 shift right by 1
   temp = BLOCK_ACCESS(block, 1, 3);
   BLOCK_ACCESS(block, 1, 3) = BLOCK_ACCESS(block, 1, 2);
   BLOCK_ACCESS(block, 1, 2) = BLOCK_ACCESS(block, 1, 1);
   BLOCK_ACCESS(block, 1, 1) = BLOCK_ACCESS(block, 1, 0);
   BLOCK_ACCESS(block, 1, 0) = temp;
 
   // Row 2 shift right by 2
   temp = BLOCK_ACCESS(block, 2, 0);
   BLOCK_ACCESS(block, 2, 0) = BLOCK_ACCESS(block, 2, 2);
   BLOCK_ACCESS(block, 2, 2) = temp;
   temp = BLOCK_ACCESS(block, 2, 1);
   BLOCK_ACCESS(block, 2, 1) = BLOCK_ACCESS(block, 2, 3);
   BLOCK_ACCESS(block, 2, 3) = temp;
 
   // Row 3 shift right by 3 (same as left by 1)
   temp = BLOCK_ACCESS(block, 3, 0);
   BLOCK_ACCESS(block, 3, 0) = BLOCK_ACCESS(block, 3, 1);
   BLOCK_ACCESS(block, 3, 1) = BLOCK_ACCESS(block, 3, 2);
   BLOCK_ACCESS(block, 3, 2) = BLOCK_ACCESS(block, 3, 3);
   BLOCK_ACCESS(block, 3, 3) = temp;
 }
 
 void invert_mix_columns(unsigned char *block) {
   for (int c = 0; c < 4; ++c) {
     unsigned char *col = &block[c * 4];
     unsigned char a0 = col[0], a1 = col[1], a2 = col[2], a3 = col[3];
     col[0] = gmul(a0, 0x0e) ^ gmul(a1, 0x0b) ^ gmul(a2, 0x0d) ^ gmul(a3, 0x09);
     col[1] = gmul(a0, 0x09) ^ gmul(a1, 0x0e) ^ gmul(a2, 0x0b) ^ gmul(a3, 0x0d);
     col[2] = gmul(a0, 0x0d) ^ gmul(a1, 0x09) ^ gmul(a2, 0x0e) ^ gmul(a3, 0x0b);
     col[3] = gmul(a0, 0x0b) ^ gmul(a1, 0x0d) ^ gmul(a2, 0x09) ^ gmul(a3, 0x0e);
   }
 }
 
 
 void add_round_key(unsigned char *block, unsigned char *round_key) {
   for (int i = 0; i < BLOCK_SIZE; i++) {
     block[i] ^= round_key[i];
   }
 }
 
 
  // Key expansion
 static const unsigned char rcon[11] = {
   0x00, 0x01, 0x02, 0x04, 0x08, 0x10,
   0x20, 0x40, 0x80, 0x1B, 0x36
 };
 
 unsigned char *expand_key(unsigned char *cipher_key) {
   int Nb = 4, Nk = 4, Nr = 10;
   unsigned char *round_keys = malloc(176); // 11 round keys * 16 bytes
 
   memcpy(round_keys, cipher_key, 16); // first round key is original key
 
   for (int i = 4; i < 4 * (Nr + 1); i++) {
     unsigned char temp[4];
     memcpy(temp, &round_keys[4 * (i - 1)], 4);
 
     if (i % Nk == 0) {
       // Rotate
       unsigned char t = temp[0];
       temp[0] = temp[1];
       temp[1] = temp[2];
       temp[2] = temp[3];
       temp[3] = t;
 
       // Substitute
       for (int j = 0; j < 4; j++) temp[j] = s_box[temp[j]];
 
       temp[0] ^= rcon[i / Nk];
     }
 
     for (int j = 0; j < 4; j++) {
       round_keys[4 * i + j] = round_keys[4 * (i - Nk) + j] ^ temp[j];
     }
   }
 
   return round_keys;
   // return 0;
 }
 

 // Encrypt and decrypt one block
 unsigned char *aes_encrypt_block(unsigned char *plaintext, unsigned char *key) {
   unsigned char *state = malloc(BLOCK_SIZE);
   memcpy(state, plaintext, BLOCK_SIZE);
 
   unsigned char *round_keys = expand_key(key);
   add_round_key(state, round_keys); // Initial round
 
   for (int round = 1; round < 10; round++) {
     sub_bytes(state);
     shift_rows(state);
     mix_columns(state);
     add_round_key(state, round_keys + round * BLOCK_SIZE);
   }
 
   sub_bytes(state);
   shift_rows(state);
   add_round_key(state, round_keys + 10 * BLOCK_SIZE); // Final round
 
   free(round_keys);
   return state;
 }
 
 unsigned char *aes_decrypt_block(unsigned char *ciphertext, unsigned char *key) {
   unsigned char *state = malloc(BLOCK_SIZE);
   memcpy(state, ciphertext, BLOCK_SIZE);
 
   unsigned char *round_keys = expand_key(key);
   add_round_key(state, round_keys + 10 * BLOCK_SIZE); // Initial round
 
   for (int round = 9; round >= 1; round--) {
     invert_shift_rows(state);
     invert_sub_bytes(state);
     add_round_key(state, round_keys + round * BLOCK_SIZE);
     invert_mix_columns(state);
   }
 
   invert_shift_rows(state);
   invert_sub_bytes(state);
   add_round_key(state, round_keys); // Final round
 
   free(round_keys);
   return state;
 }
 
 
 