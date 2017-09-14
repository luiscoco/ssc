/*******************************************************************************************************
*  Copyright 2017 Alliance for Sustainable Energy, LLC
*
*  NOTICE: This software was developed at least in part by Alliance for Sustainable Energy, LLC
*  (“Alliance”) under Contract No. DE-AC36-08GO28308 with the U.S. Department of Energy and the U.S.
*  The Government retains for itself and others acting on its behalf a nonexclusive, paid-up,
*  irrevocable worldwide license in the software to reproduce, prepare derivative works, distribute
*  copies to the public, perform publicly and display publicly, and to permit others to do so.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted
*  provided that the following conditions are met:
*
*  1. Redistributions of source code must retain the above copyright notice, the above government
*  rights notice, this list of conditions and the following disclaimer.
*
*  2. Redistributions in binary form must reproduce the above copyright notice, the above government
*  rights notice, this list of conditions and the following disclaimer in the documentation and/or
*  other materials provided with the distribution.
*
*  3. The entire corresponding source code of any redistribution, with or without modification, by a
*  research entity, including but not limited to any contracting manager/operator of a United States
*  National Laboratory, any institution of higher learning, and any non-profit organization, must be
*  made publicly available under this license for as long as the redistribution is made available by
*  the research entity.
*
*  4. Redistribution of this software, without modification, must refer to the software by the same
*  designation. Redistribution of a modified version of this software (i) may not refer to the modified
*  version by the same designation, or by any confusingly similar designation, and (ii) must refer to
*  the underlying software originally provided by Alliance as “System Advisor Model” or “SAM”. Except
*  to comply with the foregoing, the terms “System Advisor Model”, “SAM”, or any confusingly similar
*  designation may not be used to refer to any modified version of this software or any modified
*  version of the underlying software originally provided by Alliance without the prior written consent
*  of Alliance.
*
*  5. The name of the copyright holder, contributors, the United States Government, the United States
*  Department of Energy, or any of their employees may not be used to endorse or promote products
*  derived from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
*  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER,
*  CONTRIBUTORS, UNITED STATES GOVERNMENT OR UNITED STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR
*  EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
*  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************************************/

#include "core.h"
#include "lib_windfile.h"
#include "lib_windwatts.h"
// for adjustment factors
#include "common.h"
#include "lib_util.h"

static var_info _cm_vtab_windpower[] = {
	//	  VARTYPE           DATATYPE         NAME                                       LABEL                                  UNITS     META     GROUP             REQUIRED_IF                                 CONSTRAINTS                                        UI_HINTS
	{ SSC_INPUT, SSC_STRING, "wind_resource_filename", "local wind data file path", "", "", "WindPower", "?", "LOCAL_FILE", "" },
	{ SSC_INPUT, SSC_TABLE, "wind_resource_data", "wind resouce data in memory", "", "", "WindPower", "?", "", "" },

	{ SSC_INPUT, SSC_NUMBER, "wind_resource_shear", "Shear exponent", "", "", "WindPower", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "wind_resource_turbulence_coeff", "Turbulence coefficient", "%", "", "WindPower", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "system_capacity", "Nameplate capacity", "kW", "", "WindPower", "*", "", "" },


	//	{ SSC_INPUT,        SSC_NUMBER,      "meas_ht",                                 "Height of resource measurement",      "m",      "",      "WindPower",      "*",                                        "INTEGER",                                          "" },
	//	{ SSC_INPUT,        SSC_NUMBER,      "elevation",                               "Elevation",                           "m",      "",      "WindPower",      "*",                                        "",		                                            "" },
	{ SSC_INPUT, SSC_NUMBER, "wind_resource_model_choice", "Hourly or Weibull model", "0/1", "", "WindPower", "*", "INTEGER", "" },
	{ SSC_INPUT, SSC_NUMBER, "weibull_reference_height", "Reference height for Weibull wind speed", "m", "", "WindPower", "?=50", "MIN=0", "" },
	{ SSC_INPUT, SSC_NUMBER, "weibull_k_factor", "Weibull K factor for wind resource", "", "", "WindPower", "wind_resource_model_choice=1", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "weibull_wind_speed", "Average wind speed for Weibull model", "", "", "WindPower", "wind_resource_model_choice=1", "", "" },

	{ SSC_INPUT, SSC_NUMBER, "wind_turbine_rotor_diameter", "Rotor diameter", "m", "", "WindPower", "*", "", "" },
	{ SSC_INOUT, SSC_ARRAY, "wind_turbine_powercurve_windspeeds", "Power curve wind speed array", "m/s", "", "WindPower", "*", "", "" },
	{ SSC_INOUT, SSC_ARRAY, "wind_turbine_powercurve_powerout", "Power curve turbine output array", "kW", "", "WindPower", "*", "LENGTH_EQUAL=wind_turbine_powercurve_windspeeds", "" },
	//	{ SSC_INPUT,        SSC_ARRAY,       "wind_turbine_powercurve_pc_rpm",	        "Turbine RPM curve",                   "rpm",    "",      "WindPower",      "*",                                        "LENGTH_EQUAL=wind_turbine_powercurve_windspeeds",  "" },
	//	{ SSC_INPUT,        SSC_ARRAY,       "wind_turbine_powercurve_hub_efficiency",  "Array of hub efficiencies",		   "%",      "",      "WindPower",      "*",                                        "LENGTH_EQUAL=wind_turbine_powercurve_windspeeds",  "" },
	//	{ SSC_INPUT,        SSC_NUMBER,      "wind_turbine_cutin",                      "Cut-in wind speed",                   "m/s",    "",      "WindPower",      "*",                                        "",                                                 "" },
	{ SSC_INPUT, SSC_NUMBER, "wind_turbine_hub_ht", "Hub height", "m", "", "WindPower", "*", "", "" },
	//	{ SSC_INPUT,        SSC_NUMBER,      "wind_turbine_ctl_mode",                   "Control mode",                        "0/1/2",  "",      "WindPower",      "*",                                        "INTEGER",                                          "" },
	{ SSC_INPUT, SSC_NUMBER, "wind_turbine_max_cp", "Max cp", "", "", "WindPower", "wind_resource_model_choice=1", "", "" },

	{ SSC_INPUT, SSC_ARRAY, "wind_farm_xCoordinates", "Turbine X coordinates", "m", "", "WindPower", "*", "", "" },
	{ SSC_INPUT, SSC_ARRAY, "wind_farm_yCoordinates", "Turbine Y coordinates", "m", "", "WindPower", "*", "LENGTH_EQUAL=wind_farm_xCoordinates", "" },
	{ SSC_INPUT, SSC_NUMBER, "wind_farm_losses_percent", "Percentage losses", "%", "", "WindPower", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "wind_farm_wake_model", "Wake Model", "0/1/2", "", "WindPower", "*", "INTEGER", "" },


	// OUTPUTS ----------------------------------------------------------------------------													annual_energy									                            
	{ SSC_OUTPUT, SSC_ARRAY, "turbine_output_by_windspeed_bin", "Turbine output by wind speed bin", "kW", "", "Power Curve", "", "LENGTH_EQUAL=wind_turbine_powercurve_windspeeds", "" },
	{ SSC_OUTPUT, SSC_ARRAY, "wind_direction", "Wind direction", "deg", "", "Time Series", "*", "", "" },
	{ SSC_OUTPUT, SSC_ARRAY, "wind_speed", "Wind speed", "m/s", "", "Time Series", "*", "", "" },
	{ SSC_OUTPUT, SSC_ARRAY, "temp", "Air temperature", "'C", "", "Time Series", "*", "", "" },
	{ SSC_OUTPUT, SSC_ARRAY, "pressure", "Pressure", "atm", "", "Time Series", "*", "", "" },

	{ SSC_OUTPUT, SSC_ARRAY, "monthly_energy", "Monthly Energy", "kWh", "", "Monthly", "*", "LENGTH=12", "" },

	{ SSC_OUTPUT, SSC_NUMBER, "annual_energy", "Annual Energy", "kWh", "", "Annual", "*", "", "" },
	{ SSC_OUTPUT, SSC_NUMBER, "capacity_factor", "Capacity factor", "%", "", "Annual", "*", "", "" },
	{ SSC_OUTPUT, SSC_NUMBER, "kwh_per_kw", "First year kWh/kW", "kWh/kW", "", "Annual", "*", "", "" },



	var_info_invalid };

class winddata : public winddata_provider
{
	size_t irecord;
	util::matrix_t<float> data;
public:
	winddata(var_data *data_table);

	virtual size_t nrecords();

	ssc_number_t get_number(var_data *v, const char *name);

	ssc_number_t *get_vector(var_data *v, const char *name, size_t *len);

	bool read_line(std::vector<double> &values);
};

class cm_windpower : public compute_module
{
private:
public:

	cm_windpower();

	void exec() throw(general_error);
};

DEFINE_MODULE_ENTRY(windpower, "Utility scale wind farm model (adapted from TRNSYS code by P.Quinlan and openWind software by AWS Truepower)", 2);

