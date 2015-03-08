#include <string>
#include <vector>
#include "process.h"

#include <RcppArmadillo.h>

// [[Rcpp::depends(RcppArmadillo)]]

using namespace Rcpp;
using namespace arma;


struct readline
{
  std::ifstream file;
  std::string line;
  std::vector< std::string > row;
  std::vector< std::string > vnames;
  std::vector< std::vector< std::string > > vec;
  std::string item; 
  double stoptime;
  double priorpoint;
  int sample_index, sample_message, session_label, trial_start, audio;
} ;

std::vector< std::vector< std::string > > process::ReadTimeFile( std::string filename )
{
	readline rl;
  	rl.file.open( std::string( filename ).c_str() );

  	while( std::getline( rl.file, rl.line, '\r' ))
  	{
  		R_CheckUserInterrupt();
  		std::stringstream linestream( rl.line );
  		
  		while( std::getline( linestream, rl.item, '\t'))
  		{
  			rl.row.push_back( rl.item );
  		}

  		rl.vec.push_back( rl.row );
  		rl.row.clear();
  	}
  	rl.file.close();
  	return rl.vec;
}

int process::findVarIndex( std::string varname, std::vector< std::string >& varnames )
{
	int index;

	for( int i = 0; i < varnames.size(); i++ )
    {
        if( varnames[i].compare( varname ) == 0 )
        {
        	index = i;
        }
    }
    return index;
}

double process::check_start( std::string msg, std::vector< std::vector< std::string> > &matrix )
{
    double save_time = 0.0;
    long int nrow = matrix.size();
    
    for( int i = 0; i < nrow; i++ )
    {
    	R_CheckUserInterrupt();
        if( msg == matrix[i][0] )
            save_time = atof( matrix[i][1].c_str() ) * 1000.0;
    }
    return save_time;
}

bool process::check_msg( std::string msg, int id, std::vector< std::string > &row )
{
    bool match(false);
    if( row[id].compare( msg ) ==0 )
    {
        match = true;
    }
    return match;
}

void process::EyeData( std::vector< std::string >& filename, std::vector< std::vector< std::string > >& timefile )
{
	readline rl;
	rl.file.open( std::string( filename[0].c_str() ) );
	std::ofstream output_file( std::string( filename[2].c_str() ) );
	rl.stoptime = atof(filename[3].c_str() ) ;
	rl.priorpoint = atof(filename[4].c_str() ) ;
	//Rprintf("stoptime: %f", rl.stoptime);
	bool header( true );
	bool first( true );
	bool newtrial( true );
	std::string start_temp;
	std::string trial_start;
	double extract_point = 0.0;
	long counter = 0;
	
	while( std::getline( rl.file, rl.line, '\r' ))
  	{
  		R_CheckUserInterrupt();
  		std::stringstream linestream( rl.line );
  		
  		if( header )
  		{
			while( std::getline( linestream, rl.item, '\t'))
			{
				rl.vnames.push_back( rl.item );
			}
			rl.sample_index = findVarIndex( "SAMPLE_INDEX", rl.vnames );
			rl.sample_message = findVarIndex( "SAMPLE_MESSAGE", rl.vnames );
			rl.session_label = findVarIndex( "RECORDING_SESSION_LABEL", rl.vnames );
			rl.trial_start = findVarIndex( "TRIAL_START_TIME", rl.vnames );
			rl.audio = findVarIndex( "audio", rl.vnames );
			std::string temp = rl.vnames[0];
			//Rprintf("MaxIter argument   : %s \n", temp.c_str() );

			//Rprintf("pts: \n", rl.sample_index);
			header = false;

			for( int i = 0; i < rl.vnames.size(); i++ )
			{
				R_CheckUserInterrupt();
				output_file << rl.vnames[i] << "\t";;
				if( i == ( rl.vnames.size() - 1 ) )
					output_file << "align";
			}

  		}
  		else
  		{
  			while( std::getline( linestream, rl.item, '\t'))
			{
				rl.row.push_back( rl.item );
			}
			
			start_temp = rl.row[ rl.trial_start ];

			if( first ){
				trial_start = rl.row[ rl.trial_start ];
				extract_point = check_start( rl.row[ rl.audio ], timefile );
				first = false;
			}
			
			else
			{
				if( trial_start != start_temp )
				{
					trial_start = start_temp;
					newtrial = true;
					counter = 0;
				}
			}
			
			if( newtrial )
			{
				newtrial =  !check_msg( "Click_screen_start", rl.sample_message, rl.row );

			}
			else
			{
				counter += 2;
				if( counter >=50 && (counter + rl.priorpoint)  >= extract_point &&
					counter <= (extract_point + rl.stoptime ))
				{
					for( int i = 0; i < rl.row.size(); i++ )
					{
						output_file << rl.row[i] << "\t";
						if( i == ( rl.row.size() - 1 ) )
							output_file << counter;
                    }
				}
			}
		
		
  		}
  		rl.row.clear();
  	}
  	rl.file.close();
  	output_file.close();
}


void process::trimmer( std::vector< std::string > &filenames )
{
	std::vector< std::vector< std::string > > df = ReadTimeFile( filenames[1] );
	EyeData( filenames, df );

}
