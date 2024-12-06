/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "math.h"
#include "stdlib.h"

#include "gigatraj/MERRA_Directory.hh"

using namespace gigatraj;

MERRA_Directory::MVarLoc::MVarLoc(const std::string Baseurl, const std::string Categ, const std::string Tag, const int Hgrid, const int Vcoord, const int Tspace, const int Tavg, const int Tbase )
{
     baseurl = Baseurl;
     categ = Categ;
     tag = Tag;
     hgrid = Hgrid;
     vcoord = Vcoord;
     tavg = Tavg;
     tspace = Tspace;
     tbase = Tbase;

}

MERRA_Directory::MVarLoc::~MVarLoc()
{

}

MERRA_Directory::MVarDesc::MVarDesc(const std::string Quantity, const int Ndims, const std::string Stdname, const std::string Longname, const std::string Units, const std::string loclist )
{
     char locsep = '|';
     char partsep = ',';
     size_t grppos;
     size_t grpstrt;
     size_t locpos;
     size_t locstrt;
     std::string locstring;
     std::string baseurl, categ, tag;
     int hgrid, vcoord, tspace, tavg, tbase;
     const char *numbr;
     MVarLoc *newloc;
     int more;
     int locknt;

     name = Quantity;
     stdname = Stdname;
     longname = Longname;
     units = Units;
     ndims = Ndims;
     
     hgrid = -1;
     vcoord = -1;
     tspace = -1;
     tavg = -1;
     tbase = -1;

     //std::cerr << "Quantity=" << Quantity << std::endl;
     //std::cerr << "Stdname=" << Stdname << std::endl;
     //std::cerr << "Longname=" << Longname << std::endl;
     //std::cerr << "Units=" << Units << std::endl;
     //std::cerr << "loclist=" << loclist << std::endl;

     grpstrt = 0;
     
     locknt = 0;
     more = 1;
     while ( more ) {
     
         // split the loclist string into individual locations
         grppos = loclist.find(locsep, grpstrt);     
         //std::cerr << "looked for '" << locsep << "' in <<" << loclist << ">> and got " << grpstrt << "  " << grppos << " < " << std::string::npos << std::endl;
         if ( grppos != std::string::npos || grpstrt == 0 ) {
            //std::cerr << "........." << grpstrt << "  " << grppos << " < " << std::string::npos << std::endl;
            locstring = loclist.substr( grpstrt, grppos - grpstrt );

            //std::cerr << "locstring=" << locstring << std::endl;

            locstrt = 0;
            if ( (locpos = locstring.find(partsep,locstrt)) != std::string::npos ) {

               baseurl = locstring.substr(locstrt,locpos-locstrt);
               //std::cerr << "baseurl=" << baseurl << std::endl;                  

               locstrt = locpos+1;
               if ( (locpos = locstring.find(partsep,locstrt)) != std::string::npos ) {

                  categ = locstring.substr(locstrt,locpos-locstrt);
                  //std::cerr << "categ=" << categ << std::endl;

                  locstrt = locpos+1;
                  if ( (locpos = locstring.find(partsep,locstrt)) != std::string::npos ) {

                     tag = locstring.substr(locstrt,locpos-locstrt);
                     //std::cerr << "tag=" << tag << std::endl;

                     locstrt = locpos+1;
                     if ( (locpos = locstring.find(partsep,locstrt)) != std::string::npos ) {

                        hgrid = atoi( (locstring.substr(locstrt,locpos-locstrt)).data()  );
                        //std::cerr << "hgrid=" << hgrid << std::endl;

                        locstrt = locpos+1;
                        if ( (locpos = locstring.find(partsep,locstrt)) != std::string::npos ) {

                           vcoord = atoi( (locstring.substr(locstrt,locpos-locstrt)).data()  );
                           //std::cerr << "vcoord=" << vcoord << std::endl;

                           locstrt = locpos+1;
                           if ( (locpos = locstring.find(partsep,locstrt)) != std::string::npos ) {

                              tspace = atoi( (locstring.substr(locstrt)).data()  );
                              //std::cerr << "tspace=" << tspace << std::endl;

                              locstrt = locpos+1;
                              if ( (locpos = locstring.find(partsep,locstrt)) != std::string::npos ) {

                                 tavg = atoi( (locstring.substr(locstrt)).data()  );
                                 //std::cerr << "tavg=" << tavg << std::endl;

                                 locstrt = locpos+1;

                                 tbase = atoi( (locstring.substr(locstrt)).data()  );                             
                                 //std::cerr << "tbase=" << tbase << std::endl;

                                 newloc = new MVarLoc( baseurl, categ, tag, hgrid, vcoord, tspace, tavg, tbase );
                                 locs.push_back( *newloc );
                                 locknt++;
                                 delete newloc;
                                 //std::cerr << "pushed locs" << std::endl;
                              }   
                           }
                        }
                     }
                  }
               }
            }
            if ( grppos != std::string::npos ) {
               grpstrt = grpstrt + grppos + 1;
               //std::cerr << "--------- " << grpstrt << "  " << grppos << " < " << std::string::npos << std::endl;
            } else {
                more = 0;
            }    
        } else {
           more = 0;
        }
     }

     //std::cout << "Got " << Quantity << "(" << Longname << ") w/ " << locknt << " locations " << std::endl;

     
}

MERRA_Directory::MVarDesc::~MVarDesc()
{

}



MERRA_Directory::MERRA_Directory()
{

   gmao_vars.clear();
   gmao_names.clear();
   cf_names.clear();
   
      //cf_names["Q250"] = "specific_humidity_at_250_hPa";
      //gmao_names["specific_humidity_at_250_hPa"] = "Q250";
      //gmao_vars["Q250"] = new MVarDesc( "Q250", 0, "specific_humidity_at_250_hPa", "Specific humidity at 250 hPa", "kg/kg", "http://goldsmr2.gesdisc.eosdis.nasa.gov/opendap/MERRA/,MAT1NXSLV.5.2.0,MERRA100.prod.assim.tavg1_2d_slv_Nx,0,0,1" );
       
// The code included from this file is generated
// by a script (catalog_merra.py).       
#include "MERRA_Directory_Data.hh"
       
       
}

MERRA_Directory::~MERRA_Directory( )
{
   std::map<std::string, MVarDesc *>::iterator i;
 
   for (i=gmao_vars.begin(); i!=gmao_vars.end(); i++ ) {
       delete i->second;
   }

}

std::string MERRA_Directory::CFName(const std::string &merraName ) const
{     
     try {
        return cf_names.at(merraName);
     } catch (const std::out_of_range& oor) {
        throw(badName());
     }   
}

std::string MERRA_Directory::MERRAName(const std::string &cfName ) const
{
     try {
        return gmao_names.at(cfName);
     } catch (const std::out_of_range& oor) {
        throw(badName());
     }   
}

int MERRA_Directory::LookUp(const std::string merraName, const std::string date
                , const int vertgrid, const int horizgrid, const int timespace, const int timeavg 
                , std::string **LongName, std::string **Units, int *ndims
                , int *actualVertGrid, int *actualHorizGrid, int *actualTimeSpace, int *actualTimeAvg, int *actualBaseTime
                , std::string **url 
                 ) const   
{
    MVarDesc *item;
    int result = 1;
    int year;
    std::string *url0;

    // Grab the information about the desired quantity
    // If we don't have it, then throw an error
    item = NULLPTR;
    //try {
    if ( gmao_vars.count( merraName ) > 0 ) {
       item = gmao_vars.at(merraName);
    //} catch (const std::out_of_range& cor) {
    } else {
       std::cerr << "Unknown MERRA quantity " << merraName << std::endl;
       throw (badNotFound());
    }   

    // Copy these into place, if the user desires
    // We test for NULL, so that the user can
    // put null pointers into the calling sequence if he or she
    // does not want to receive this information
    if ( LongName != NULL ) {
       *LongName = new std::string( item->longname );
    }
    if ( Units != NULL ) {
       *Units = new std::string( item->units );
    }
    if ( ndims != NULL ) {
       // the number in the table includes the TIME dimension,
       // which we want to omit
       *ndims = item->ndims - 1;
    } 
    // Go through each location, testing its attributes
    // against the desired, and returning on first match
    for ( std::vector<MVarLoc>::iterator loc=item->locs.begin(); loc != item->locs.end(); loc++ ) {

        if (  ((vertgrid == -1) || (vertgrid == loc->vcoord) ) 
        && ((horizgrid == -1) || (horizgrid == loc->hgrid) )
        && ((timespace == -1 ) || (timespace == loc->tspace) ) 
        && ((timeavg == -1 ) || (timeavg == loc->tavg) ) ) {
         
           if ( url != NULL ) {
              // start with the base URL of the location
              url0 = new std::string( loc->baseurl );
              // add in the category
              url0->append( loc->categ );
              // add in the year directory 
              url0->append( "/" );
              url0->append( date.substr(0,4) );
              url0->append( "/" );
              // add in the month subdirectory
              url0->append( date.substr(5,2) );
              url0->append( "/" );
              // add in the file base name
              url0->append( "MERRA" );
              // the stream number depends on the date
              year = atoi( date.substr(0,4).c_str() );
              if ( year < 1993 ) {
                 url0->append( "1" );
              } else if ( year < 2001 ) {
                 url0->append( "2" );
              } else {
                 url0->append( "3" );
              }         
              url0->append( loc->tag.substr(6) );
              // add in the date part of the file name
              url0->append( "." );
              url0->append( date.substr(0,4) );
              url0->append( date.substr(5,2) );
              url0->append( date.substr(8,2) );
              // finish up with the file name extension
              url0->append( ".hdf" );
              
              *url = url0;
              
           }
           
           if ( actualVertGrid != NULL ) {
              *actualVertGrid = loc->vcoord;
           }
           if ( actualHorizGrid != NULL ) {
              *actualHorizGrid = loc->hgrid;
           }
           if ( actualTimeAvg != NULL ) {
               *actualTimeAvg = loc->tavg;
           }
           if ( actualTimeSpace != NULL ) {
               *actualTimeSpace = loc->tspace;
           }
           if ( actualBaseTime != NULL ) {
               *actualBaseTime = loc->tbase;
           }
           
           result = 0;

           break;            
           
        }
    }


    return result;
}
   