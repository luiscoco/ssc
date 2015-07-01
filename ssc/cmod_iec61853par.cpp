#include "core.h"
#include "lib_iec61853.h"

static var_info vtab_iec61853[] = 
{	
/*   VARTYPE            DATATYPE         NAME                        LABEL                       UNITS     META                                             GROUP          REQUIRED_IF    CONSTRAINTS UI_HINTS*/
	{ SSC_INPUT,        SSC_MATRIX,      "input",                  "IEC-61853 matrix test data", "various",  "[IRR,TC,PMP,VMP,VOC,ISC]",                      "IEC61853",    "*",           "",         "" },
	{ SSC_INPUT,        SSC_NUMBER,      "nser",                   "Number of cells in series",  "",         "",                                              "IEC61853",    "*",           "",         "" },
	{ SSC_INPUT,        SSC_NUMBER,      "type",                   "Cell technology type",       "0..5",     "monoSi,multiSi/polySi,cis,cigs,cdte,amorphous", "IEC61853",    "*",           "",         "" },
	{ SSC_INPUT,        SSC_NUMBER,      "verbose",                "Output solver messages",     "0/1",      "",                                              "IEC61853",    "*",           "",         "" },
																								 											                			   
	{ SSC_OUTPUT,       SSC_NUMBER,      "alphaIsc",               "SC temp coefficient @ STC",  "A/C",      "",                                              "IEC61853",    "*",           "",         "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "betaVoc",                "OC temp coefficient @ STC",  "V/C",      "",                                              "IEC61853",    "*",           "",         "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "gammaPmp",               "MP temp coefficient @ STC",  "%/C",      "",                                              "IEC61853",    "*",           "",         "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "n",                      "Diode factor",               "",         "",                                              "IEC61853",    "*",           "",         "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "Il",                     "Light current",              "A",        "",                                              "IEC61853",    "*",           "",         "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "Io",                     "Saturation current",         "A",        "",                                              "IEC61853",    "*",           "",         "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "C1",                     "Rsh fitting C1",             "",         "",                                              "IEC61853",    "*",           "",         "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "C2",                     "Rsh fitting C2",             "",         "",                                              "IEC61853",    "*",           "",         "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "C3",                     "Rsh fitting C3",             "",         "",                                              "IEC61853",    "*",           "",         "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "D1",                     "Rs fitting D1",              "",         "",                                              "IEC61853",    "*",           "",         "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "D2",                     "Rs fitting D2",              "",         "",                                              "IEC61853",    "*",           "",         "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "D3",                     "Rs fitting D3",              "",         "",                                              "IEC61853",    "*",           "",         "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "Egref",                  "Bandgap voltage",            "eV",       "",                                              "IEC61853",    "*",           "",         "" },

var_info_invalid };

class cm_iec61853par : public compute_module
{
private:
public:
	cm_iec61853par()
	{
		add_var_info( vtab_iec61853 );
	}

	class msg_handler : public Imessage_api
	{
		compute_module &cm;
	public:
		msg_handler(compute_module &_c) :cm(_c) { }
		
		virtual void Printf(const char *fmt, ...) {
			char buf[1024];
			va_list ap;
			va_start(ap, fmt);
#ifdef _MSC_VER
			_vsnprintf(buf, 1024, fmt, ap);
#else
			vsnprintf(buf,1024,fmt,ap);
#endif
			va_end(ap);
			cm.log( buf, SSC_NOTICE );
		}
		virtual void Outln(const char *msg ) {
			cm.log( msg, SSC_NOTICE );
		}
	};

	void exec( ) throw( general_error )
	{
		iec61853_module_t solver;
		msg_handler msgs( *this );
		solver._imsg = &msgs;

		util::matrix_t<double> input = as_matrix("input"), par;
		if ( input.ncols() != iec61853_module_t::COL_MAX )
			throw exec_error( "iec61853", "six data columns required for input matrix: IRR,TC,PMP,VMP,VOC,ISC");

		if (!solver.calculate( input, as_number("nser"), as_number("type"), par, as_boolean("verbose") ))
			throw exec_error( "iec61853", "failed to solve for parameters");

		assign( "n", var_data(solver.n) );
		assign( "alphaIsc", var_data(solver.alphaIsc) );
		assign( "betaVoc", var_data(solver.betaVoc) );
		assign( "gammaPmp", var_data(solver.gammaPmp) );
		assign( "Il", var_data(solver.Il) );
		assign( "Io", var_data(solver.Io) );
		assign( "C1", var_data(solver.C1) );
		assign( "C2", var_data(solver.C2) );
		assign( "C3", var_data(solver.C3) );
		assign( "D1", var_data(solver.D1) );
		assign( "D2", var_data(solver.D2) );
		assign( "D3", var_data(solver.D3) );
		assign( "Egref", var_data(solver.Egref) );
	}
};

DEFINE_MODULE_ENTRY( iec61853par, "Calculate 11-parameter single diode model parameters from IEC-61853 PV module test data.", 1 )
