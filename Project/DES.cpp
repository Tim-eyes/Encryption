#include <iostream>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <bitset>
#include <cmath>
#include "DES.h"
#include "Tables.h"

DES::DES(std::string plain, std::string key) :plaintext(hex2bin(plain)), key(hex2bin(key)) {}

DES::~DES(){}

const std::string DES::bin2hex(const std::string& binStr)
{
	std::string hexStr{};
	std::string bin;

	for(int i=0;i<(int)binStr.length();i+=4)
	{
		bin = binStr.substr(i, 4);
		if (bin == "0000") {
			hexStr.append("0");
		}
		else if (bin == "0001") {
			hexStr.append("1");
		}
		else if (bin == "0010") {
			hexStr.append("2");
		}
		else if (bin == "0011") {
			hexStr.append("3");
		}
		else if (bin == "0100") {
			hexStr.append("4");
		}
		else if (bin == "0101") {
			hexStr.append("5");
		}
		else if (bin == "0110") {
			hexStr.append("6");
		}
		else if (bin == "0111") {
			hexStr.append("7");
		}
		else if (bin == "1000") {
			hexStr.append("8");
		}
		else if (bin == "1001") {
			hexStr.append("9");
		}
		else if (bin == "1010") {
			hexStr.append("A");
		}
		else if (bin == "1011") {
			hexStr.append("B");
		}
		else if (bin == "1100") {
			hexStr.append("C");
		}
		else if (bin == "1101") {
			hexStr.append("D");
		}
		else if (bin == "1110") {
			hexStr.append("E");
		}
		else if (bin == "1111") {
			hexStr.append("F");
		}
		else {
			throw std::invalid_argument("ERROR! bin2hex binary string must be multiple of 4.\n");
			exit(0);
		}
	}
	return hexStr;
}

const std::string DES::hex2bin(const std::string& hexStr)
{
	std::string binStr{};
	for (auto c = hexStr.begin(); c < hexStr.end(); c++) {
		switch (*c)
		{
		case '0':
			binStr.append("0000");
			break;
		case '1':
			binStr.append("0001");
			break;
		case '2':
			binStr.append("0010");
			break;
		case '3':
			binStr.append("0011");
			break;
		case '4':
			binStr.append("0100");
			break;
		case '5':
			binStr.append("0101");
			break;
		case '6':
			binStr.append("0110");
			break;
		case '7':
			binStr.append("0111");
			break;
		case '8':
			binStr.append("1000");
			break;
		case '9':
			binStr.append("1001");
			break;
		case 'A':
		case 'a':
			binStr.append("1010");
			break;
		case 'B':
		case 'b':
			binStr.append("1011");
			break;
		case 'C':
		case 'c':
			binStr.append("1100");
			break;
		case 'D':
		case 'd':
			binStr.append("1101");
			break;
		case 'E':
		case 'e':
			binStr.append("1110");
			break;
		case 'F':
		case 'f':
			binStr.append("1111");
			break;
		default:
			throw std::invalid_argument("ERROR!Invaild input, could't convert to binary.\n");
			break;
		}
	}
	return binStr;
}

const int DES::bin2dec(const std::string& binStr)
{
	int dec = 0;
	for (auto it = binStr.rbegin(); it < binStr.rend(); it++) {
		if (*it == '1') {
			dec += pow(2, std::distance(binStr.rbegin(),it));
		}
	}
	return dec;
}

void DES::desEncrption( std::string& plaintext,  std::string& key)
{
	initialPermu(plaintext);
	splitText(initialPermu64);
	permutedChoice1(key);
	for (int i = 0; i < ROUND; i++) {
		keyFunc(key56, i);
		desFunc(rText, key48);
		std::cout << std::setw(6) << i + 1 << std::setw(20) << bin2hex(getLText() + getRText())<< std::setw(20) << bin2hex(getKey56())<<"\n";
	}
	//std::swap(lText, rText);
	lText.swap(rText);
	inverKey64 = lText + rText;
	inverseInitialPermu(inverKey64);
}

void DES::splitText( std::string& in64)
{
	lText = in64.substr(0, 32);
	rText = in64.substr(32, 32);
}

void DES::initialPermu( std::string& in64)
{
	/*std::bitset<64> out64;
	int index = 0;
	
	for (auto it = in64.begin(); it < in64.end(); it++) {
		if (*it == '1') {
			int pos = std::distance(in64.begin(), it) + 1;
			for (int i = 0; i < 64; i++) {
				if (pos == IP[i]) {
					index = pos;
					out64.set((64 - index - 1), 1);
				}
			}
		}
	}
	initialPermu64 = out64.to_string();*/
	if (in64.length() != 64) {
		throw std::invalid_argument("ERROR! Initial Permutation, invaild length.\n");
		exit(0);
	}

	std::string out64{};
	for (int i = 0; i < 64; i++) {
		out64 += in64[IP[i] - 1];
	}
	initialPermu64 = out64;

}

void DES::desFunc( std::string& in32,  std::string& inKey48)
{
	expansPermu(in32);
	//xor 48 bits 
	std::bitset<48> temp48, a(expansPermu48), b(inKey48);
	temp48 = a ^ b;
	xor48 = temp48.to_string();
	SBoxes(xor48);
	permuFunc(sBoxs32);
	//xor 32 bits 
	std::bitset<32> temp32, c(permuFunc32), d(lText);
	temp32 = c ^ d;
	xor32 = temp32.to_string();
	lText = in32; //this step must after xor!!!
	rText = xor32;
}

void DES::inverseInitialPermu( std::string& in64)
{
	if (in64.length() != 64) {
		throw std::invalid_argument("ERROR! Inverse Initial Permutation, invaild length.\n");
		exit(0);
	}

	std::string out64{};
	for (int i = 0; i < 64; i++) {
		out64 += in64[Inver_IP[i] - 1];
	}
	ciphertext = out64;
}

void DES::permutedChoice1( std::string& inKey64)
{
	if (inKey64.length() != 64) {
		throw std::invalid_argument("ERROR! Permutation Choice 1, invaild length.\n");
		exit(0);
	}

	std::string out56{};
	for (int i = 0; i < 56; i++) {
		out56 += inKey64[PC1[i] - 1];
	}
	key56 = out56;
}

void DES::splitKey( std::string& inKey56)
{
	lKey28 = inKey56.substr(0, 28);
	rKey28 = inKey56.substr(28, 28);
}

void DES::leftShift( std::string& inLKey28,  std::string& inRKey28, int round)
{
	/*if (round > ROUND) {
		throw std::invalid_argument("ERROR! Left Shift, invaild shift.\n");
		exit(0);
	}*/
	//no loop
	//std::bitset<28> outL28(inLKey28), outR28(inRKey28);
	//lKey28 = (outL28 << round).to_string();
	//rKey28 = (outR28 << round).to_string();
	std::rotate(inLKey28.begin(), inLKey28.begin() + Rotate[round], inLKey28.end());
	std::rotate(inRKey28.begin(), inRKey28.begin() + Rotate[round], inRKey28.end());
}

void DES::permutedChoice2( std::string& inKey56)
{
	if (inKey56.length() != 56) {
		throw std::invalid_argument("ERROR! Permutation Choice 2, invaild length.\n");
		exit(0);
	}
	std::string out48{};
	for (int i = 0; i < 48; i++) {
		out48 += inKey56[PC2[i] - 1];
	}
	key48 = out48;
}

void DES::keyFunc( std::string& inKey56,int round)
{
	splitKey(inKey56);
	leftShift(lKey28, rKey28, round);
	key56 = lKey28 + rKey28;
	permutedChoice2(key56);
}


void DES::expansPermu( std::string& in32)
{
	if (in32.length() != 32) {
		throw std::invalid_argument("ERROR! Expansion Permutation, invaild length.\n");
		exit(0);
	}
	
	std::string out48{};
	for (int i = 0; i < 48; i++) {
		out48 += in32[EP[i] - 1];
	}
	expansPermu48 = out48;
}

void DES::SBoxes( std::string& in48)
{
	if (in48.length() != 48) {
		throw std::invalid_argument("ERROR! Subsitution, invaild length.\n");
		exit(0);
	}

	sBoxs32.clear(); //Don't forget!!!!!
	std::bitset<4> bin4{};

	int row{}, col{};
	for (int i = 0; i < 48; i += 6) {
		row = bin2dec(in48.substr(i, 1) + in48.substr(i + 5, 1));
		col = bin2dec(in48.substr(i + 1, 4));
		bin4 = S_Boxes[i / 6][row][col];
		sBoxs32 += bin4.to_string();
	}
}

void DES::permuFunc( std::string& in32)
{
	if (in32.length() != 32) {
		throw std::invalid_argument("ERROR! Permutation Function, invaild length.\n");
		exit(0);
	}

	std::string out32{};
	for (int i = 0; i < 32; i++) {
		out32 += in32[PF[i] - 1];
	}
	permuFunc32 = out32;
}
