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

uint8_t* AES::encryptECB(uint8_t in[], u_int inLen, uint8_t key[], u_int& outLen)
{
	outLen = getBlockLen(inLen);
	uint8_t* _in = setPaddingArr(in, inLen, outLen);
	
	uint8_t* out = new uint8_t[outLen];
	uint8_t* roundKeys = new uint8_t[4 * Nb * (Nr + 1)];
	
	keyExpansion(key, roundKeys);
	for (u_int i = 0; i < inLen; i += blockBytesLen) {
		aesEncrypt(_in + i, out + i, roundKeys);
	}
	
	delete[] _in;
	delete[] roundKeys;
	return out;
}

uint8_t* AES::decryptECB(uint8_t in[], u_int inLen, uint8_t key[])
{
	uint8_t* out = new uint8_t[inLen];
	uint8_t* roundKeys = new uint8_t[4 * Nb * (Nr + 1)];
	
	keyExpansion(key, roundKeys);
	
	for (u_int i = 0; i < inLen; i += blockBytesLen) {
		aesDecrypt(in + i, out + i, roundKeys);
	}

	delete[] roundKeys;
	return out;	
}

uint8_t* AES::encryptCBC(uint8_t in[], u_int inLen, uint8_t key[], uint8_t* iv, u_int& outLen)
{

	outLen = getBlockLen(inLen);
	uint8_t* _in = setPaddingArr(in, inLen, outLen);

	uint8_t* out = new uint8_t[outLen];
	uint8_t* roundKeys = new uint8_t[4 * Nb * (Nr + 1)];
	uint8_t* block = new uint8_t[blockBytesLen];

	keyExpansion(key, roundKeys);
	memcpy(block, iv, blockBytesLen);

	for (u_int i = 0; i < inLen; i += blockBytesLen) {
		xorBlock(block, _in + i, block, blockBytesLen);
		aesEncrypt(_in + i, out + i, roundKeys);
		memcpy(block, out + i, blockBytesLen);
	}

	delete[] _in;
	delete[] roundKeys;
	delete[] block;

	return out;
}

uint8_t* AES::decryptCBC(uint8_t in[], u_int inLen, uint8_t key[], uint8_t* iv)
{

	uint8_t* out = new uint8_t[inLen];
	uint8_t* roundKeys = new uint8_t[4 * Nb * (Nr + 1)];
	uint8_t* block = new uint8_t[blockBytesLen];

	keyExpansion(key, roundKeys);
	memcpy(block, iv, blockBytesLen);

	for (u_int i = 0; i < inLen; i += blockBytesLen) {
		aesDecrypt(in + i, out + i, roundKeys);
		xorBlock(block, out + i, out + i, blockBytesLen);
		memcpy(block, in + i, blockBytesLen);
	}

	delete[] roundKeys;
	delete[] block;
	return out;
}

uint8_t* AES::encryptCFB(uint8_t in[], u_int inLen, uint8_t key[], uint8_t* iv, u_int& outLen)
{
	return nullptr;
}

uint8_t* AES::decryptCFB(uint8_t in[], u_int inLen, uint8_t key[], uint8_t* iv)
{
	return nullptr;
}

uint8_t* AES::encryptOFB(uint8_t in[], u_int inLen, uint8_t key[], uint8_t* iv, u_int& outLen)
{
	return nullptr;
}

uint8_t* AES::decryptOFB(uint8_t in[], u_int inLen, uint8_t key[], uint8_t* iv)
{
	return nullptr;
}

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
	}

	memcpy(state[i], tmp, Nb * sizeof(uint8_t));
	delete[] tmp;
}

void AES::shiftRow(uint8_t** state)
{
	shiftRow(state, 1, 3);
	shiftRow(state, 2, 2);
	shiftRow(state, 3, 1);
}


void AES::mixColumn(uint8_t** state)
{
	uint8_t** tmp = new uint8_t *[4];
	for (int i = 0; i < 4; i++) {
		tmp[i] = new uint8_t[4];
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

	for (int i = 0; i < 4; i++) {
		delete[] tmp[i];
	}
	delete[] tmp;
}

void AES::addAoundKey(uint8_t** state, uint8_t* key)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[i][j] = state[i][j] ^ key[i + 4 * j];
		}
	}
}

void AES::aesEncrypt(uint8_t in[], uint8_t out[], uint8_t* roundKeys)
{
	uint8_t** state = new uint8_t *[4];
	/*state[0] = new uint8_t[4 * Nb];
	for (int i = 0; i < 4; i++) {
		state[i] = state[0] + Nb * i;
	}*/
	for (int i = 0; i < 4; i++) {
		state[i] = new uint8_t[4];
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[i][j] = in[i + 4 * j];
		}
	}

	addAoundKey(state, roundKeys);
	for (int r = 1; r <= Nr - 1; r++) {
		substituteBytes(state);
		shiftRow(state);
		mixColumn(state);
		addAoundKey(state, roundKeys + r * 4 * Nb);
	}
	substituteBytes(state);
	shiftRow(state);
	addAoundKey(state, roundKeys + Nr * 4 * Nb);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			out[i + 4 * j] = state[i][j];
		}
	}
	
	delete[] state;

}

void AES::invMixColumn(uint8_t** state)
{
	uint8_t s[4], s_inv[4];
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			s[i] = state[i][j];
		}

		s_inv[0] = xtime(0x0e, s[0]) ^ xtime(0x0b, s[1]) ^ xtime(0x0d, s[2]) ^ xtime(0x09, s[3]);
		s_inv[1] = xtime(0x09, s[0]) ^ xtime(0x0e, s[1]) ^ xtime(0x0b, s[2]) ^ xtime(0x0d, s[3]);
		s_inv[2] = xtime(0x0d, s[1]) ^ xtime(0x09, s[1]) ^ xtime(0x0e, s[2]) ^ xtime(0x0b, s[3]);
		s_inv[3] = xtime(0x0b, s[1]) ^ xtime(0x0d, s[1]) ^ xtime(0x09, s[2]) ^ xtime(0x0e, s[3]);

		for (int i = 0; i < 4; i++) {
			state[i][j] = s_inv[i];
		}
	}
}

void AES::invShiftRow(uint8_t** state)
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

void AES::aesDecrypt(uint8_t in[], uint8_t out[], uint8_t* roundKeys)
{
	uint8_t** state = new uint8_t * [4];
	/*for (int i = 0; i < 4; i++) {
		state[i] = new uint8_t[4];
	}*/

	for (int i = 0; i < 4; i++) {
		state[i] = new uint8_t[4];
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[i][j] = in[i + 4 * j];
		}
	}

	addAoundKey(state, roundKeys + Nr * 4 * Nb);
	for (int r = Nr - 1; r >= 1 ; r--) {
		invSubstituteBytes(state);
		invShiftRow(state);
		addAoundKey(state, roundKeys + r * 4 * Nb);
		invMixColumn(state);	
	}
	invSubstituteBytes(state);	
	invShiftRow(state);
	addAoundKey(state, roundKeys);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			out[i + 4 * j] = state[i][j];
		}
	}


	delete[] state;
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
		for (int k = 0; k < 4; k++) {
			tmp[k] = w[i - 4 + k];
		}
		if (i / 4 % Nk == 0) {
			rotWord(tmp);
			subWord(tmp);
			rCon(rcon, i / Nk);
			xorWord(tmp, rcon, tmp);
		}
		else if (Nk > 6 && i / 4 % Nk == 4) {
			subWord(tmp);
		}
		for (int k = 0; k < 4; k++) {
			w[i + k] = w[i + k - 4 * Nk] ^ tmp[k];
		}
		i += 4;
	}

	delete[] tmp;
	delete[] rcon;
	
}

uint8_t AES::xtime(uint8_t st)
{
	return (st << 1) ^ (st & 0x80 ? 0x1b : 0x00);
}

uint8_t AES::xtime(uint8_t a, uint8_t b)
{
	uint8_t res = 0;
	uint8_t high_bit_mask = 0x80;
	uint8_t high_bit = 0;
	uint8_t modulo = 0x1B; /* x^8 + x^4 + x^3 + x + 1 */

	for (int i = 0; i < 8; i++) {
		if (b & 1) {
			res ^= a;
		}
		high_bit = a & high_bit_mask;
		a <<= 1;
		if (high_bit) {
			a ^= modulo;
		}
		b >>= 1;
	}
	return res;
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

void AES::xorBlock(uint8_t* a, uint8_t* b, uint8_t* c, u_int len)
{
	for (int i = 0; i < len; i++) {
		c[i] = a[i] ^ b[i];
	}
}

uint8_t* AES::setPaddingArr(uint8_t in[], u_int inLen, u_int _len)
{
	uint8_t* _in = new uint8_t[_len];
	memcpy(_in, in, inLen);
	memset(_in + inLen, 0x00, _len - inLen);
	return _in;
}


u_int AES::getBlockLen(u_int len)
{
	u_int _len = (len / blockBytesLen);
	if (len % blockBytesLen) {
		_len++;

	}
	_len *= blockBytesLen;
	return _len;
}


void AES::print(uint8_t arr[], uint8_t n)
{
	for (auto i = 0; i < n; i++) {
		printf("%02x", arr[i]);
	}
}
