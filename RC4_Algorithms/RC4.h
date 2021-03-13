#ifndef _RC4_H_
#define _RC4_H_

static constexpr int LEN = 256;

class RC4
{
private:
	unsigned char sBox[256];
	unsigned char* kBox;
	unsigned char tBox[256];
	unsigned char* plaintext, *cipher;
	int sizeKey;
	int rc4I = 0, rc4J = 0;
	void swap(unsigned char box[], int i, int j);
	void initBox();
	void streamGene();

public:	
	RC4(unsigned char* p, unsigned char* k,int size);
	virtual ~RC4();
	void rc4Func();
	unsigned char* getSBox() { return sBox; }
	unsigned char* getKBox() { return kBox; }
	unsigned char* getTBox() { return tBox; }
	unsigned char* getPlaintext() { return plaintext; }
	unsigned char* getCipher() { return cipher; }
	int getSizeKey() { return sizeKey; }


};

#endif // !_RC4_H_

