## supported by @KennardWang https://blog.csdn.net/qq_42950838/article/details/114714194?spm=1001.2014.3001.5501
import numpy as np

# set parameters by hand
SIZE = 8
K = [1, 2, 3, 6]
P = [1, 2, 2, 2]

print("Plaintext :", P)
print("Key :", K)
print()

def swap(S, i, j):
    tmp = S[i]
    S[i] = S[j]
    S[j] = tmp


# 1. S and T initialization
S = np.arange(0, SIZE)
T = np.arange(0, SIZE)

for i in range(0, SIZE):
    T[i] = K[i % len(K)]

print("1. Initialize S and T :")
print("S =", S)
print("T =", T)
print()


# 2. Permutation
print("2. Permutation S :")
j = 0
for i in range(0, SIZE):
    j = (j + S[i] + T[i]) % 8
    swap(S, i, j)
    print("Step", i, ": S =", S)
print()


# 3. Stream Generation
print("3. Stream Generation :")
i = 0
j = 0
count = 0
k = np.arange(0, len(K))
while count < len(K):
    i = (i + 1) % SIZE
    j = (j + S[i]) % SIZE
    swap(S, i, j)
    t = (S[i] + S[j]) % SIZE
    k[i-1] = S[t]

    print("Step", count, ": i =", i, ", j =", j, ", S =", S, ", t =", t)
    count = count + 1
print()


# 4. Encryption & Decryption
print("4. Encryption and Decryption :")
print("k stream :", k)

C = np.arange(0, len(K))
for i in range(0, len(K)):
    C[i] = P[i] ^ k[i]

print("Ciphertext :", C)
