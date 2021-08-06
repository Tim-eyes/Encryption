#ifndef _DES_H_
#define _DES_H_
#include <string>

const static int ROUND = 16;

class DES 
{
private:
	std::string plaintext;	 //64 bits 
	std::string ciphertext;	 //64 bits output by Inverse Persmutation
	std::string initialPermu64;// 64 output Initial Permutation 
	std::string lText;	 //32 bits
	std::string rText;	 //32 bits
	std::string expansPermu48;  //48 bits output by Expansion Permutation
	std::string xor48;	//48 bit output by XOR
	std::string xor32;	//32 bit output by XOR
	std::string sBoxs32;//32 bits output by S-Boxs
	std::string permuFunc32; //32 bits output by Permutation Function
	std::string key;	//input 64 bits key
	std::string key56;  //56 bits key after Permuted Chioce 1(64->56)
	std::string lKey28;	//28 bits
	std::string rKey28;	//28 bits
	std::string key48;  //48 bits key after Permuted Chioce 2(56->48) 
	std::string inverKey64; //64 bits key after swaping left and right halves

	//procodure for every round of DES algorithm 
	void expansPermu( std::string& in32);
	void SBoxes( std::string& in48);
	void permuFunc( std::string& in32);
	
	//procedure for DES algorithm
	void splitText( std::string& in64);
	void initialPermu( std::string& in64);
	void desFunc( std::string& in48,  std::string& inKey48);
	void inverseInitialPermu( std::string& in64);

	//procedure for key generation
	void permutedChoice1( std::string& inKey64);
	void splitKey( std::string& inKey56);
	void leftShift( std::string& inLKey28,  std::string& inRKey28, int round);
	void permutedChoice2( std::string& inKey56);
	//main box
	void keyFunc( std::string& inKey56, int round);

public:
	DES(std::string plain, std::string key);
	virtual ~DES();

	//void setPlaintext( std::string& str) { plaintext = str; }
	std::string& getPlaintext() { return plaintext; } //***
	std::string& getCiphertext() { return ciphertext; }
	std::string& getLText() { return lText; }
	std::string& getRText() { return rText; }
	std::string& getExpansPermu() { return expansPermu48; }
	std::string& getXor48() { return xor48; }
	std::string& getXor32() { return xor32; }
	std::string& getSBoxes32() { return sBoxs32; }
	std::string& getPermuFunc32() { return permuFunc32; }
	std::string& getKey64() { return key; }//***
	std::string& getKey56() { return key56; }
	std::string& getLKey28() { return lKey28; }
	std::string& getRKey28() { return rKey28; }
	std::string& getKey48() { return key48; }
	std::string& getInverKey64() { return inverKey64; }
	
	//secondary tool
	const std::string bin2hex(const std::string& binStr);
	const std::string hex2bin(const std::string& hexStr);
	const int bin2dec(const std::string& binStr);


	void desEncrption( std::string& plaintext,  std::string& key);

	



};
#endif //_DES_H_