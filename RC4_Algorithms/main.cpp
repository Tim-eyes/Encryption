#include <iostream>
#include <string>
#include <iomanip>
#include "RC4.h"

int main() {

	std::string key = "I am Key";
	std::string text = "I am text";
	/*std::cout << "Please input 64 bits plaintext and key in hexadcimal.\n";
	std::cout << "plaintext:";
	std::cin >> text;
	std::cout << "key:";
	std::cin >> key;*/
	RC4 rc4((unsigned char*)text.c_str(), (unsigned char*)key.c_str(), key.length());
	rc4.rc4Func();
	std::cout << rc4.getCipher()<< "\n";
	return 0;
}