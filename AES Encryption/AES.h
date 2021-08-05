#ifndef _AES_H_
#define _AES_H_
#include <cstring>
#include <cstdint>
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


//void copy(blockType& B1, const blockType& B2);
//void copy(blockType& B,const uint8_t* b);
//void copy_inv(uint8_t* b,const blockType& B);

class AES {
private:
	int Nr;
	int Nb;
	int Nk;
	uint8_t blockBytesLen;

	void substituteBytes(uint8_t **state);
	void shiftRow(uint8_t** state, int i, int n);
	void shiftRow(uint8_t** state);
	void mixColumn(uint8_t* m);
	void mixColumn(uint8_t **state);
	void addAoundKey(uint8_t **state,uint8_t *key);
	
	void invMixColumn(uint8_t** state);
	void invShiftColum(uint8_t** state);
	void invSubstituteBytes(uint8_t** state);
	
	void keyExpansion(uint8_t key[],uint8_t w[]);
	
	//secondary tool
	uint8_t xtime(uint8_t st);

	void subWord(uint8_t *a);
	void rotWord(uint8_t *a);
	void xorWord(uint8_t *a,uint8_t *b,uint8_t *c);
	void rCon(uint8_t *a,int n);
	void xorBlock(uint8_t* a, uint8_t* b, uint8_t* c, int len);


public:
	AES(int keyLen = 128);
	virtual ~AES();


	uint8_t* aesEncryption(uint8_t in[],uint8_t key[],uint8_t inLen, uint8_t &out);
	void print(uint8_t arr[], uint8_t n);
};

#endif // !_AES_H_

