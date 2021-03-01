#include <iostream>
#include <iomanip>
#include <string>
#include "DES.h"
#include "Tables.h"

int main(int argc,char** argv)
{
	if (argc > 1) {
		std::cout << "Please input 64 bits plaintext and key in hexadcimal.\n";
		DES des(argv[0], argv[1]);
		std::cout << std::setw(6) << "Round" << std::setw(20) << "Text" << std::setw(20) << "Key" << "\n";
		des.desEncrption(des.getPlaintext(), des.getKey64());
		std::cout << std::setw(6) << "plaintext:" << std::setw(20) << des.bin2hex(des.getPlaintext()) << "\n";
		std::cout << std::setw(6) << "key:" << std::setw(20) << des.bin2hex(des.getKey64()) << "\n";
		std::cout << std::setw(6) << "ciphertext:" << std::setw(20) << des.bin2hex(des.getCiphertext()) << "\n";
		return 0;
	}

	//std::string plaintext="0123456789ABCDEF", key="133457799BBCDFF1";
	std::string plaintext, key;
	std::cout << "Please input 64 bits plaintext and key in hexadcimal.\n";
	std::cout << "plaintext:";
	std::cin >> plaintext; 
	std::cout << "key:";
	std::cin >> key; 
	DES des(plaintext, key);
	std::cout << std::setw(6) << "Round" << std::setw(20) << "Text" << std::setw(20) << "Key" << "\n";
	des.desEncrption(des.getPlaintext(), des.getKey64());
	std::cout << std::setw(6) << "plaintext:" << std::setw(20) << des.bin2hex(des.getPlaintext()) << "\n";
	std::cout << std::setw(6) << "key:" << std::setw(20) << des.bin2hex(des.getKey64()) << "\n";
	std::cout << std::setw(6) << "ciphertext:" << std::setw(20) << des.bin2hex(des.getCiphertext()) << "\n";
	return 0;
}