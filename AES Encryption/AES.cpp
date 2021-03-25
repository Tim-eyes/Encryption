#include "AES.h"
#include "Tables.h"
#include <string>
#include <cstring>
#include <stdexcept>


AES::~AES() { }

void copy(blockType& B1,const blockType& B2)
{
	for (auto i = 0; i < ROW; i++) {
		for (auto j = 0; j < COLUMN; j++) {
			B1[i][j] = B2[i][j];
		}
	}
}

void copy(blockType& B, const uint8_t* b)
{
	for (auto i = 0; i < ROW; i++) {
		for (auto j = 0; j < COLUMN; j++) {
			B[i][j] = b[i * COLUMN + j];
		}
	}
}

void copy_inv(uint8_t* b,const blockType& B)
{
	for (auto i = 0; i < ROW; i++) {
		for (auto j = 0; j < COLUMN; j++) {
			b[i * COLUMN + j] = B[i][j];
		}
	}
}

void AES::expansKey(key& K, const uint8_t* key)
{
	copy(K.round(0), key);
	for (uint8_t r = 0; r < ROUND + 1; r++) {

	}
}

void AES::aesEncryption()
{
}
