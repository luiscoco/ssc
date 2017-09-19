#include <gtest\gtest.h>

#include "core.h"
#include "vartab.h"
#include "common.h"
#include "cmod_windpower.h"
#include "cmod_windpower_test.h"


TEST_F(CMWindPowerTest, IntegrationTest_cmod_windpower){
	cm = new cm_windpower();
	create_default();
	float ann_energy = vartab->lookup("annual_energy")->num.at(0);
	EXPECT_EQ(ann_energy, 142561184, e*ann_energy);
}