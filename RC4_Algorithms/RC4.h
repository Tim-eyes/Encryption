#ifndef _RC4_H_
#define _RC4_H_
#include <cstdint>
static constexpr int LEN = 256;

class RC4
{
private:
	uint8_t sBox[256];
	uint8_t* kBox;
	uint8_t tBox[256];
	uint8_t* plaintext, *cipher;
	int sizeKey;
	int rc4I = 0, rc4J = 0;
	void swap(uint8_t box[], int i, int j);
	void initBox();
	void streamGene();

public:	
	RC4(uint8_t* p, uint8_t* k,int size);
	virtual ~RC4();
	void rc4Func();
	uint8_t* getSBox() { return sBox; }
	uint8_t* getKBox() { return kBox; }
	uint8_t* getTBox() { return tBox; }
	uint8_t* getPlaintext() { return plaintext; }
	uint8_t* getCipher() { return cipher; }
	int getSizeKey() { return sizeKey; }


};

#endif // !_RC4_H_

