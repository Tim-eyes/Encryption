#include "AES.h"
#include "Tables.h"
#include <string>
#include <cstring>
#include <stdexcept>


AES::AES(int keyLen)
{
	this->Nb = 4;
	switch (keyLen) {
		case 128:
			this->Nr = 10;
			this->Nk = 4;
			break;
		case 192:
			this->Nr = 12;
			this->Nk = 6;
			break;
		case 256:
			this->Nr = 14;
			this->Nk = 8;
			break;
		default:
			throw std::invalid_argument("Incorrent key length.\n");
	}
	blockBytesLen = 4 * this->Nb * sizeof(uint8_t);
		

}

AES::~AES() { }

void AES::substituteBytes(uint8_t** state)
{
	uint8_t tmp;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp = state[i][j];
			state[i][j] = S_Box[tmp / 16][tmp % 16];
		}
	}

}

void AES::shiftRow(uint8_t** state, int i, int n)
{
	uint8_t* tmp = new uint8_t[Nb];
	for (int j = 0; j < Nb; j++) {
		tmp[j] = state[i][j + n % 4];
		memcpy(state[i], tmp, Nb * sizeof(uint8_t));
	}
	delete[] tmp;
}

void AES::shiftRow(uint8_t** state)
{
	shiftRow(state, 1, 3);
	shiftRow(state, 2, 2);
	shiftRow(state, 3, 1);
}

void AES::mixColumn(uint8_t* m)
{
}

void AES::mixColumn(uint8_t** state)
{
	uint8_t** tmp = new uint8_t *[Nb];
	for (int i = 0; i < Nb; i++) {
		tmp[i] = new uint8_t[Nb];
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp[i][j] = xtime(state[i % 4][j]) ^
				(state[(i + 1) % 4][j] ^ xtime(state[(i + 1) % 4][j])) ^
				(state[(i + 2) % 4][j] ^ xtime(state[(i + 2) % 4][j])) ^
				(state[(i + 3) % 4][j] ^ xtime(state[(i + 3) % 4][j]));
		}
	}

	memcpy(state, tmp, Nb * Nb * sizeof(uint8_t));

	delete[] tmp;
}

void AES::invShiftColum(uint8_t** state)
{
	shiftRow(state, 1, 1);
	shiftRow(state, 2, 2);
	shiftRow(state, 3, 3);
}

void AES::invSubstituteBytes(uint8_t** state)
{
	uint8_t tmp;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp = state[i][j];
			state[i][j] = Inver_S_Box[tmp / 16][tmp % 16];
		}
	}
}

void AES::keyExpansion(uint8_t key[], uint8_t w[])
{
	uint8_t* tmp = new uint8_t[4];
	uint8_t* rcon = new uint8_t[4];
	
	int i = 0;
	while (i < 4 * Nk) {
		w[i] = key[i];
		i ++;
	}
	i = 4 * Nk;
	while (i < 4 * Nb * (Nr + 1)) {
		tmp[0] = w[i - 4 + 0];
		tmp[1] = w[i - 4 + 1];
		tmp[2] = w[i - 4 + 2];
		tmp[3] = w[i - 4 + 3];
		if (i / 4 % Nk == 0) {
			rotWord(tmp);
			subWord(tmp);
			rCon(rcon, i / Nk);
			xorWord(tmp, rcon, tmp);
		}
		else if (Nk > 6 && i / 4 % Nk == 4) {
			subWord(tmp);
		}
		w[i] = w[i - 4 * Nk] ^ tmp[0];
		w[i + 1] = w[i + 1 - 4 * Nk] ^ tmp[1];
		w[i + 2] = w[i + 2 - 4 * Nk] ^ tmp[2];
		w[i + 3] = w[i + 3 - 4 * Nk] ^ tmp[3];
		i += 4;
	}

	delete[] tmp;
	delete[] rcon;
	
}

uint8_t AES::xtime(uint8_t st)
{
	return (st << 1) ^ (st & 0x80 ? 0x1b : 0x00);
}

void AES::subWord(uint8_t* a)
{
	for (int i = 0; i < 4; i++) {
		a[i] = S_Box[a[i] / 16][a[i] % 16];
	}
}

void AES::rotWord(uint8_t* a)
{
	uint8_t cnt;
	cnt = a[0];
	a[0] = a[1];
	a[1] = a[2];
	a[2] = a[3];
	a[3] = cnt;
}

void AES::xorWord(uint8_t* a, uint8_t* b, uint8_t* c)
{
	for (int i = 0; i < 4; i++) {
		c[i] = a[i] ^ b[i];
	}
}

void AES::rCon(uint8_t* a, int n)
{
	uint8_t cnt = 1;
	for (int i = 0; i < n - 1; i++) {
		cnt = xtime(cnt);
	}
	a[0] = cnt;
	a[1] = a[2] = a[3] = 0;
}

void AES::xorBlock(uint8_t* a, uint8_t* b, uint8_t* c, int len)
{
	for (int i = 0; i < len; i++) {
		c[i] = a[i] ^ b[i];
	}
}



void AES::print(uint8_t arr[], uint8_t n)
{
	for (auto i = 0; i < n; i++) {
		printf("%02x", arr[i]);
	}
}
