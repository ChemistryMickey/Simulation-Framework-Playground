#include "LateRef.hpp"
#include <gtest/gtest.h>

TEST(TestLateRef, Empty) {
	LateRef<double> d{};
	ASSERT_FALSE(d.has_value());
}

TEST(TestLateRef, NonEmpty) {
	double d = 42.0;
	LateRef<double> dref{d};
	ASSERT_TRUE(dref.has_value());
	EXPECT_DOUBLE_EQ(*dref, 42.0);
}

TEST(TestLateRef, SimpleObj) {
	struct Foo { int i; };
	Foo f{.i = 42};
	LateRef<Foo> fref{f};

	ASSERT_TRUE(fref.has_value());
	EXPECT_EQ(fref->i, 42);
}
