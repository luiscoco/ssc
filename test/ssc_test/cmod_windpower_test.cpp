#include <gtest\gtest.h>
#include <ctime>

#include "core.h"
#include "vartab.h"
#include "common.h"
#include "cmod_windpower.h"
#include "cmod_windpower_test.h"

/// Using Wind Resource Data with various Wake Models
TEST_F(CMWindPowerIntegration, DISABLED_ResourceSimpleWake_cmod_windpower){
	clock_t Start = clock();
	compute();
	float ann_energy = vartab->lookup("annual_energy")->num.at(0);
#ifdef DEBUG
	EXPECT_NEAR(ann_energy, 32999724, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(0), 2.8027e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(11), 2.8027e6, e);
#endif
#ifdef RELEASE
	EXPECT_NEAR(ann_energy, 33224154, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(0), 2.8218e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(11), 2.8218e6, e);
#endif

	std::cout << "Time Difference: " << clock() - Start << std::endl;
	
}

TEST_F(CMWindPowerIntegration, DISABLED_ResourceWAsp_cmod_windpower){
	clock_t Start = clock();
	modify_var(vartab, "wind_farm_wake_model", 1);

	compute();
	float ann_energy = vartab->lookup("annual_energy")->num.at(0);
#ifdef DEBUG
	EXPECT_NEAR(ann_energy, 29522034, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(0), 2.50735e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(11), 2.507353e6, e);
#endif
#ifdef RELEASE
	EXPECT_NEAR(ann_energy, 32346158, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(0), 2.7472e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(11), 2.7472e6, e);
#endif
	std::cout << "Time Difference: " << clock() - Start << std::endl;
}

TEST_F(CMWindPowerIntegration, DISABLED_ResourceEddy_cmod_windpower){
	clock_t Start = clock();
	modify_var(vartab, "wind_farm_wake_model", 2);

	compute();
	float ann_energy = vartab->lookup("annual_energy")->num.at(0);

#ifdef DEBUG
	EXPECT_NEAR(ann_energy, 28813624, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(0), 2.4472e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(11), 2.4472e6, e);
#endif
#ifdef RELEASE
	EXPECT_NEAR(ann_energy, 31081848, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(0), 2.6398e6, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(11), 2.6398e6, e);
#endif

	std::cout << "Time Difference: " << clock() - Start << std::endl;
}

/// Using Weibull Distribution
TEST_F(CMWindPowerIntegration, Weibull_cmod_windpower){
	clock_t Start = clock();
	modify_var(vartab, "wind_resource_model_choice", 1);
	compute();
	float ann_energy = vartab->lookup("annual_energy")->num.at(0);

	EXPECT_NEAR(ann_energy, 180453760, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(0), 15326247, e);
	EXPECT_NEAR(vartab->lookup("monthly_energy")->num.at(11), 15326247, e);

	std::cout << "Time Difference: " << clock() - Start << std::endl;
}


