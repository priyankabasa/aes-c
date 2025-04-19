AES-128 Implementation in C

Features:
•	All core AES operations
•	SubBytes, ShiftRows, MixColumns, AddRoundKey
•	Key expansion via expand_key
•	Full block encryption and decryption
•	A Python test script (test_aes.py) using ctypes to validate the C implementation
•	GitHub Actions CI for automated building and testing


Building and Running
Build the project using Make
Run the program:
./main
Run the Python test script (requires Python 3): python3 test_aes.py

Project Structure
main.c — Main C program for manual encryption/decryption tests
rijndael.c — Core AES-128 algorithm implementation
rijndael.h — Header file for AES definitions
test_aes.py — Python script for testing the C library
Makefile — Build instructions
.github/workflows/ — GitHub Actions workflow for CI/CD

Continuous Integration
This project uses GitHub Actions to automatically build and test the code on every push, ensuring reliability and correctness through continuous integration.

