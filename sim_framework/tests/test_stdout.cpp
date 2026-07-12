#include <gtest/gtest.h>
#include <iostream>

TEST(TestCompilation, foo){
	ASSERT_TRUE(true);
	std::cout << "Google test seems to work\n";
}
