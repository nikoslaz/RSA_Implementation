#include <stdio.h>
#include <stdlib.h>

// Compute gcd of two numbers
int gcd(int a, int b) {
    while (b) {
        a %= b;
        // Swap a and b
        int temp = a;
        a = b;
        b = temp;
    }
    return a;
}

// Compute (base^exponent) % m
long long power(long long base, long long exponent, long long m) {
    long long result = 1;
    base %= m;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % m;
        }
        exponent >>= 1;
        base = (base * base) % m;
    }
    return result;
}

// Find modular inverse of e modulo phi
int mod_inverse(int e, int phi) {
    for (int d = 2; d < phi; d++) {
        if((long long)e * d % phi == 1) {
            return d;
        }
    }

    fprintf(stderr, "Error: No modular inverse found for e = %d and phi = %d\n", e, phi);
    return -1; // Inverse not found
}

// RSA key generation
void generate_keys(int *e, int *d, int *n) {
    // Choose two large prime numbers, p and q
    int p = 991;
    int q = 997;

    //Compute n = p * q
    *n = p * q;

    // Compute Euler's Totient Function
    int phi = (p - 1) * (q - 1);

    // Choose e such that 1 < e < phi and gcd(e, phi) = 1
    int e_val;
    for (e_val = 2; e_val < phi; e_val++) {
        if (gcd(e_val, phi) == 1) {
            break;
        }
    }
    *e = e_val;

    // Compute d such that (e * d) % phi == 1
    *d = mod_inverse(*e, phi);
    if (*d == -1) {
        fprintf(stderr, "Error: Failed to compute modular inverse.\n");
        exit(1);
    }
} 

// Encrypt message using public key (e, n)
// Returns the ciphertext C = M^e mod n
long long encrypt(int e, int n, long long M) {
    return power(M, e, n);
}

// Decrypt message using private key (d, n)
// Returns the original message M = C^d mod n
long long decrypt(long long c, int d, int n) {
    return power(c, d, n);
}

int main() {
    int e, d, n;

    // Generate RSA keys
    generate_keys(&e, &d, &n);

    // Check modular inverse
    if (d == -1) {
        fprintf(stderr, "Error: Failed to compute modular inverse.\n");
        return 1;
    }

    printf("Public Key: (e: %d, n: %d)\n", e, n);
    printf("Private Key: (d: %d, n: %d)\n", d, n);

    // Meessage to encrypt
    int message = 2002;
    printf("Original Message: %d\n", message);

    // Encrypt the message
    long long encrypted_message = encrypt(e, n, message);
    if (encrypted_message == -1) {
        fprintf(stderr, "Error: Encryption failed.\n");
        return 1;
    }
    printf("Encrypted Message: %lld\n", encrypted_message);

    // Decrypt the message
    long long decrypted_message = decrypt(encrypted_message, d, n);
    if (decrypted_message == -1) {
        fprintf(stderr, "Error: Decryption failed.\n");
        return 1;
    }
    printf("Decrypted Message: %lld\n", decrypted_message);

    // Verify
    if (decrypted_message == message) {
        printf("Decryption successful! The decrypted message matches the original message.\n");
    } else {
        printf("Decryption failed! The decrypted message does not match the original message.\n");
    }

    return 0;
}