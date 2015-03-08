#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include "process.h"
#include <RcppArmadillo.h>

// [[Rcpp::depends(RcppArmadillo)]]

using namespace Rcpp;
using namespace arma;


RcppExport SEXP eyeProcess(SEXP DATAFILE, SEXP TIMEFILE, SEXP OUTPUTNAME, SEXP DURATION, SEXP PRIORPOINT)
{
	BEGIN_RCPP
	std::vector< std::string  > filenames;
	filenames.push_back( as<std::string>(DATAFILE) );
	filenames.push_back( as<std::string>(TIMEFILE) );
	filenames.push_back( as<std::string>(OUTPUTNAME) );
	filenames.push_back( as<std::string>(DURATION) );
	filenames.push_back( as<std::string>(PRIORPOINT) );

	process pr;
	pr.trimmer( filenames );
	return wrap(filenames);
	END_RCPP
}