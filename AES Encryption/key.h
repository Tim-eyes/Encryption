#ifndef _KEY_H_
#define _KEY_H_
#include "AES.h"
#include <array>
#include <vector>
#include <cstring>
/*
* key 16 bytes
*/
constexpr int BOX_SIZE = TEXT_LEN * TEXT_LEN;

using boxType = std::array<uint8_t, BOX_SIZE>;

struct key
{
private:
	std::vector<blockType> roundKey; //every round key
public:
	key();
	blockType& round(const uint8_t r) { return roundKey[r]; }
	//void keyRound0(blockType& k, const uint8_t* K);
};

#endif // !_KEY_H_
