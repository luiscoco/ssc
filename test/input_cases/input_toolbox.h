#ifndef _INPUT_TOOLBOX_
#define _INPUT_TOOLBOX_

#include "vartab.h"
#include "core.h"
#include <string>

/**
* Custom Testing Handler that does nothing
*/
static ssc_bool_t my_handler(ssc_module_t, ssc_handler_t, int, float, float, const char *, const char *, void *){
	return true;
}

static void var(var_table* vt, std::string name, std::string value){
	var_data* vd = new var_data(value);
	vt->assign(name, *vd);
}

static void var(var_table* vt, std::string name, double value){
	var_data* vd = new var_data((ssc_number_t)value);
	vt->assign(name, *vd);
}

static void var(var_table* vt, std::string name, float* array, int length){
	var_data* vd = new var_data(array, length);
	vt->assign(name, *vd);
}

static void var(var_table* vt, std::string name, float* matrix, int nr, int nc){
	var_data* vd = new var_data(matrix, nr, nc);
	vt->assign(name, *vd);
}

#endif