#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "RC4.h"
#include <iostream>
#include <malloc.h>


RC4::RC4(unsigned char* p, unsigned char* k,int size):sizeKey(size)
{
	plaintext=(unsigned char *)malloc(sizeof(p) + 1);
	cipher = (unsigned char*)malloc(sizeof(p) + 1);
	kBox = (unsigned char*)malloc(sizeof(k) + 1);

}

RC4::~RC4(){}

void RC4::rc4Func()
{
	initBox();
	streamGene();
}


void RC4::swap(unsigned char box[], int i, int j)
{
	unsigned char tmp = box[i];
	box[i] = box[j];
	box[j] = tmp;
}

void RC4::initBox()
{
	for (int i = 0; i < LEN; i++) {
		sBox[i] = (unsigned char)i;
		tBox[i] = kBox[i % sizeKey];
	}
	int j = 0;
	for (int i = 0; i < LEN; i++) {
		j = (j + sBox[i] + tBox[i]) % LEN;
		std::swap(sBox[i], sBox[j]);
	}
}

void RC4::streamGene()
{
	for (int i = 0; i < sizeKey; i++) {
		rc4I = (rc4I + 1) % LEN;
		rc4J = (rc4J + sBox[rc4I]) % LEN;
		std::swap(sBox[rc4I], sBox[rc4J]);
		int tmp = (sBox[rc4I] + sBox[rc4J]) % LEN;
		cipher[i] = sBox[tmp] ^ plaintext[i];
	}
}


