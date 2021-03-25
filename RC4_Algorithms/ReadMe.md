# RC4 
## Principle 
- Used in secure web browsing and wireless LANs
- Very simple and efficient implementation
- Can use variable size key: 8 to 2048 bits
- Several theoretical limitations of RC4
  - No known attacks if use 128-bit key and discard initial values of stream
  - RC4 is used in Wireless Encryption Protocol, WEP (shown to be weak security for wireless LANs)—problem with how keys are used, not RC4 algorithm
  
## RC4 algorithm
The nonstandard **Pseudo-Algorithm of RC4** is be shown below:
```python
## 1. initial State of S and T
## initialise S to values 0 to 255
## initialise T with repeating values of key K
for i = 0 to 255 
do 
  S[i] = i;
  T[i] = K[i mod keyLen];
done

## 2. Initial Permutation of S
j =0;
for i = 0 to 255
do
  j = (j + S[i] + T[i]) mod 256;
  Swap (S[i],S[j]);
done

## 3.Stream Generation
i, j, cnt = 0; 
while(cnt<keyLen)
do
  i = (i + 1) mod 256;
  j = (j+ S[i]) mod 256;
  Swap (S[i],S[j]);
  t = (S[i] + S[j]) mod 256; 
  k =S[t];
  cnt++;
done 
```

