#include <gtest\gtest.h>

#include "core.h"
#include "vartab.h"
#include "common.h"
#include "cmod_windpower.h"

class CMWindPowerTest : public ::testing::Test{
private:
	void assign_default_variables(var_table* vt);
	void create_default();

protected:
	cm_windpower* cm;
};

TEST_F(CMWindPowerTest, IntegrationTest_cmod_windpower){
	cm = new cm_windpower();
}