#include <gtest\gtest.h>
#include <ctime>

#include "core.h"
#include "vartab.h"
#include "common.h"
#include "cmod_windpower.h"
#include "cmod_windpower_test.h"


TEST_F(CMWindPowerTest, IntegrationTest_cmod_windpower){
	clock_t Start = clock();


	compute();
	e = 1000; // scale up epsilon
	float ann_energy = vartab->lookup("annual_energy")->num.at(0);
	EXPECT_NEAR(ann_energy, 32999724, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(0), 2.8027e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(1), 2.53148e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(2), 2.8027e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(3), 2.7123e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(4), 2.8027e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(5), 2.7123e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(6), 2.8027e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(7), 2.8027e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(8), 2.7123e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(9), 2.8027e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(10), 2.7123e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(11), 2.8027e6, e);

	std::cout << "Time Difference: " << clock() - Start << std::endl;
	
}