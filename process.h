#ifndef process_h
#define process_h



class process
{
public:
	std::vector< std::vector< std::string > > ReadTimeFile( std::string argv );
	void trimmer( std::vector< std::string >& filenames );
	int findVarIndex( std::string varname, std::vector< std::string >& varnames );
	void EyeData( std::vector< std::string >& filenames, std::vector< std::vector< std::string > >& timefile );
	double check_start( std::string msg, std::vector< std::vector< std::string> > &matrix );
	bool check_msg( std::string msg, int id, std::vector< std::string > &row );
};


#endif