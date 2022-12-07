
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"


#include "gigatraj/PGenNetcdf.hh"

using namespace gigatraj;

PGenNetcdf::PGenNetcdf()
{
     dbug = 0;
     fname = "";
     vcoord = "";
     caltime = "";
     time0 = 0.0;
     atend = false;
     n = 0;
     
}

PGenNetcdf::~PGenNetcdf()
{

}

void PGenNetcdf::open( const std::string &file)
{
     if ( ncIn.isOpen() ) {
        ncIn.close();
     }
     
     if ( file != "" ) {
        fname = file;
     }
     
     if ( fname != "" ) {
        if (vcoord != "" ) {
           ncIn.vertical( vcoord );
        }
        ncIn.at_end( atend );
        ncIn.debug( dbug );
     
        ncIn.open( fname );
     
        n = ncIn.n_parcels();
        vcoord = ncIn.vertical();
        caltime = ncIn.cal();
        time0 = ncIn.time();
     }   
}

bool PGenNetcdf::isOpen()
{
     return ncIn.isOpen();
}

Parcel * PGenNetcdf::create_array(const Parcel& p, size_t *np
                     , const std::string &file
                     ) 
{
     Parcel *result;
     
     if ( ! ncIn.isOpen() ) {
        open( file );
     }
     
     
     result = pgen.create_array( p, n );
     
     ncIn.apply( result, n );
     
     ncIn.close();
     
     *np = n;
     return result;
}

std::vector<Parcel>* PGenNetcdf::create_vector(const Parcel& p
                    , const std::string &file
                    )

{
    std::vector<Parcel>* result;
    
     if ( ! ncIn.isOpen() ) {
        open( file );
     }
     
     result = pgen.create_vector( p, n );
     
     ncIn.apply( *result );
     
     ncIn.close();
     
     return result;

}



std::list<Parcel>* PGenNetcdf::create_list(const Parcel& p
                    , const std::string &file 
                    )
{
    std::list<Parcel>* result;
    
     if ( ! ncIn.isOpen() ) {
        open( file );
     }
     
     result = pgen.create_list( p, n );
     
     ncIn.apply( *result );
     
     ncIn.close();
     
     return result;


}
      


std::deque<Parcel>* PGenNetcdf::create_deque(const Parcel& p
                    , const std::string &file
                    )
{
    std::deque<Parcel>* result;
    
     if ( ! ncIn.isOpen() ) {
        open( file );
     }
     
     result = pgen.create_deque( p, n );
     
     ncIn.apply( *result );
     
     ncIn.close();
     
     return result;

}
      


Flock* PGenNetcdf::create_Flock(const Parcel& p
                    , const std::string &file, ProcessGrp* pgrp, int r
                    )
{
    Flock* result;
    
     if ( ! ncIn.isOpen() ) {
        open( file );
     }
     
     result = pgen.create_Flock( p, n, pgrp, r );
     
     ncIn.apply( *result );
     
     ncIn.close();
     
     return result;

}
      
            

Swarm* PGenNetcdf::create_Swarm(const Parcel& p
                    , const std::string &file, ProcessGrp* pgrp, int r
                    )
{
    Swarm* result;
    
     if ( ! ncIn.isOpen() ) {
        open( file );
     }
     
     result = pgen.create_Swarm( p, n, pgrp, r );
     
     ncIn.apply( *result );
     
     ncIn.close();
     
     return result;

}



std::string& PGenNetcdf::filename()
{
    return fname;
}




void PGenNetcdf::vertical( std::string vert )
{
    vcoord = vert;
    //ncIn.vertical( vert );
}
   
std::string& PGenNetcdf::vertical()
{
   return vcoord;
}

   
std::string& PGenNetcdf::cal()
{
    return caltime;
}
   
double PGenNetcdf::time()
{
    return time0;
}

void PGenNetcdf::at_end( bool select )
{
     atend = select;
     //ncIn.at_end( select );
}
     
bool PGenNetcdf::at_end()
{
    return atend;
}
     
size_t PGenNetcdf::n_parcels()
{
    return n;
}
        
void PGenNetcdf::debug( int mode )
{
     dbug = mode;
     //ncIn.debug( mode );
}
    
int PGenNetcdf::debug()
{
    return dbug;
}

void PGenNetcdf::readFlags( int mode )
{
    ncIn.readFlags( mode );
}

int PGenNetcdf::readFlags()
{
    return ncIn.readFlags();
}


void PGenNetcdf::readStatus( int mode )
{
    ncIn.readStatus( mode );
}


int PGenNetcdf::readStatus()
{
    return ncIn.readStatus();
}


void PGenNetcdf::readTag( int mode )
{
    ncIn.readTag( mode );
}


int PGenNetcdf::readTag()
{
    return ncIn.readTag();
}


void PGenNetcdf::format( std::string fmt )
{
    ncIn.format( fmt );
}

  
std::string PGenNetcdf::format()
{
    return ncIn.format();
}

      
void PGenNetcdf::setTimeTransform( double scale, double offset )
{
     ncIn.setTimeTransform( scale, offset );
}

void PGenNetcdf::getTimeTransform( double* scale, double* offset )
{
     ncIn.getTimeTransform( scale, offset );
}
    