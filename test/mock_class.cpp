#include <iostream>
#include <gtest\gtest.h>
#include <gmock\gmock.h>

class module{
	int type;
	void func(){
		std::cout << "funking out" << std::endl;
	}
};

class handler{
public:
	module* mod;

	handler(module m){
		mod = &m;
	}

	virtual void handle() = 0;
};

class MockHandler : public handler{
public:
	MockHandler();
	MOCK_METHOD0(handle, void());
};

TEST(MockTest, Test){
	MockHandler mock;
}