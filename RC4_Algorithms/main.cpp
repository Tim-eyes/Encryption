#include <iostream>
#include <string>
#include <iomanip>
#include "RC4.h"

int main() {

	std::string key = "1236";
	std::string text = "1222";
	/*std::cout << "Please input 64 bits plaintext and key in hexadcimal.\n";
	std::cout << "plaintext:";
	std::cin >> text;
	std::cout << "key:";
	std::cin >> key;*/
	RC4 rc4((uint8_t*)text.c_str(), (uint8_t*)key.c_str(), key.length());
	rc4.rc4Func();
	/*std::cout << rc4.getCipher()<< "\n";*/
	printf("%02x\n", rc4.getCipher());
	return 0;
}