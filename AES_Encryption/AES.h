#ifndef _AES_H_
#define _AES_H_
#include <cstring>
#include <cstdint>
#include <array>
#include <stdio.h>
/*
* Plaintext 16 bytes(128 bits)
* intput state 16 bytes 4*4
*/
constexpr uint8_t TEXT_LEN = 16;
constexpr uint8_t COLUMN = 4;
constexpr uint8_t ROW = 4;
constexpr uint8_t ROUND = 10; //AES 128 bits 10 rounds


using wordType = std::array<uint8_t, COLUMN>;
using blockType = std::array<wordType, ROW>;



class AES {
private:
	int Nr;
	int Nb;
	int Nk;
	uint8_t blockBytesLen;

	void substituteBytes(uint8_t **state);
	void shiftRow(uint8_t** state, int i, int n);
	void shiftRow(uint8_t** state);
	void mixColumn(uint8_t **state);
	void addAoundKey(uint8_t **state,uint8_t *key);
	void aesEncrypt(uint8_t in[], uint8_t out[], uint8_t *roundKeys);
	
	void invMixColumn(uint8_t** state);
	void invShiftRow(uint8_t** state);
	void invSubstituteBytes(uint8_t** state);
	void aesDecrypt(uint8_t in[], uint8_t out[], uint8_t *roundKeys);

	void keyExpansion(uint8_t key[],uint8_t w[]);
	
	//secondary tool
	uint8_t xtime(uint8_t st);
	uint8_t xtime(uint8_t a,uint8_t b);

	void subWord(uint8_t* a);
	void rotWord(uint8_t* a);
	void xorWord(uint8_t* a, uint8_t* b, uint8_t* c);
	void rCon(uint8_t* a, int n);
	void xorBlock(uint8_t* a, uint8_t* b, uint8_t* c, int len);

	
public:
	AES(int keyLen = 128);
	virtual ~AES();

	//TBD
	uint8_t* encrptECB();
	uint8_t* decrptECB();
	uint8_t* encrptCBC();
	uint8_t* decrptCBC();
	uint8_t* encrptCFB();
	uint8_t* decrptCFB();
	uint8_t* encrptOFB();
	uint8_t* decrptOFB();
	

	
	void print(uint8_t arr[], uint8_t n);
};

#endif // !_AES_H_

