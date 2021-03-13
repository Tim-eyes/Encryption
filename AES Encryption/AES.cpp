#include "AES.h"
#include "Tables.h"
#include <string>
#include <cstring>
#include <stdexcept>


AES::~AES() { }

void AES::copy(blockType& B1,const blockType& B2)
{
	for (auto i = 0; i < ROW; i++) {
		for (auto j = 0; j < BYTE; j++) {
			B1[i][j] = B2[i][j];
		}
	}
}

void AES::copy(blockType& B, const uint8_t* b)
{
	for (auto i = 0; i < ROW; i++) {
		for (auto j = 0; j < BYTE; j++) {
			B[i][j] = b[i * BYTE + j];
		}
	}
}

void AES::copy_inv(uint8_t* b,const blockType& B)
{
	for (auto i = 0; i < ROW; i++) {
		for (auto j = 0; j < BYTE; j++) {
			b[i * BYTE + j] = B[i][j];
		}
	}
}
