#ifndef _AES_H_
#define _AES_H_
#include <cstring>
#include <array>
#include <cstdint>
/*
* Plaintext 16 bytes(128 bits)
* intput state 16 bytes 4*4
*/
constexpr uint8_t TEXT_LEN = 16;
constexpr uint8_t BYTE = 4;
constexpr uint8_t ROW = 4;
constexpr uint8_t ROUND = 10; //AES 128 bits 10 rounds


using wordType = std::array<uint8_t, BYTE>;
using blockType = std::array<wordType, ROW>;


class AES {
private:
	/*int NP;
	int NB;
	int NL;*/
public:
	AES();
	virtual ~AES();

	//secondary tool
	void copy(blockType& B1, const blockType& B2);
	void copy(blockType& B,const uint8_t* b);
	void copy_inv(uint8_t* b,const blockType& B);

	

	void expansKey();
	void aesEncryption();
};

#endif // !_AES_H_

