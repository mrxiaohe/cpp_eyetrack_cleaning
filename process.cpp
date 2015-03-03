#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include "process.h"


struct readline
{
  std::ifstream file;
  std::string line;
  std::vector< std::string > row;
  std::vector< std::string > vnames;
  std::vector< std::vector< std::string > > vec;
  std::string item; 
  int sample_index, sample_message, session_label, trial_start, audio;
} ;

std::vector< std::vector< std::string > > process::ReadTimeFile( std::string filename )
{
	readline rl;
  	rl.file.open( std::string( filename ).c_str() );

  	while( std::getline( rl.file, rl.line, '\r' ))
  	{
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

void process::EyeData( std::string filename, std::vector< std::vector< std::string > >& timefile )
{
	readline rl;
	rl.file.open( std::string( filename.c_str() ) );
	bool header( true );
	bool first( true );
	bool newtrial( true );
	std::string start_temp;
	std::string trial_start;
	double extract_point = 0.0;
	long counter = 0;
	std::ofstream output_file("eyetracking_processed_file.csv");

	while( std::getline( rl.file, rl.line, '\r' ))
  	{
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
			header = false;

			for( int i = 0; i < rl.vnames.size(); i++ )
			{
				output_file << rl.vnames[i];
				if( i == ( rl.vnames.size() - 1 ) )
					output_file << "align" << "\t";
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
					extract_point = check_start( rl.row[ rl.audio ], timefile );
				}
			}
			if( newtrial )
			{
				newtrial =  !check_msg( "Click_screen_start", rl.sample_message, rl.row );

			}
			else
			{
				counter += 2;
				if( counter >=50 && counter >= extract_point )
				{
					for( int i = 0; i < rl.row.size(); i++ )
					{
						output_file << rl.row[i];
						if( i == ( rl.row.size() - 1 ) )
							output_file << counter << "\t";
						//else 
						//	
                    }
				}
			}
			rl.row.clear();
  		}
  	}
  	rl.file.close();
  	output_file.close();
}


void process::trimmer( char** argv )
{
	std::vector< std::vector< std::string > > df = ReadTimeFile( argv[1] );
	EyeData( argv[2], df );

}
