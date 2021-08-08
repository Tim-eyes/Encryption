#include "gtest/gtest.h"
#include <cstdint>
#include <iostream>
#include "../AES_Encryption/AES.h"
#include "../AES_Encryption/AES.cpp"

const uint8_t BLOCK_BYTES_LENGTH = 16 * sizeof(uint8_t);

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(ECB, EncryptDecrypt)
{

}


int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
	
}