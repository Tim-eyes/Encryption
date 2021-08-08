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

using u_int = unsigned int;

class AES {
private:
	int Nr;
	int Nb;
	int Nk;
	u_int blockBytesLen;

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
	void xorBlock(uint8_t* a, uint8_t* b, uint8_t* c, u_int len);

	uint8_t* setPaddingArr(uint8_t in[], u_int inLen, u_int _len);
	u_int getBlockLen(u_int len);
	
public:
	AES(int keyLen);
	~AES();

	
	uint8_t* encryptECB(uint8_t in[], u_int inLen, uint8_t key[], u_int & outLen);
	uint8_t* decryptECB(uint8_t in[], u_int inLen, uint8_t key[]);
	uint8_t* encryptCBC(uint8_t in[], u_int inLen, uint8_t key[], uint8_t* iv, u_int & outLen);
	uint8_t* decryptCBC(uint8_t in[], u_int inLen, uint8_t key[], uint8_t* iv);
	//TBD
	uint8_t* encryptCFB(uint8_t in[], u_int inLen, uint8_t key[], uint8_t* iv, u_int& outLen);
	uint8_t* decryptCFB(uint8_t in[], u_int inLen, uint8_t key[], uint8_t* iv);
	uint8_t* encryptOFB(uint8_t in[], u_int inLen, uint8_t key[], uint8_t* iv, u_int& outLen);
	uint8_t* decryptOFB(uint8_t in[], u_int inLen, uint8_t key[], uint8_t* iv);
	

	
	void print(uint8_t arr[], uint8_t n);
};

#endif // !_AES_H_

