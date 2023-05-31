
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "gigatraj/PlanetNav.hh"

using namespace gigatraj;


/*! 
The constructor initializes planet-related constants:

If WRAP_0 is set, then the longitudes will range from 0 to 360.
If WRAP_180 is set, then the longitudes will range from -180 to 180.
If neither is set, then WRAP_180 is assumed.

*/
PlanetNav :: PlanetNav()
{

#ifdef WRAP_0
   wraplon = 0.0;
#endif
#ifdef WRAP_180
   wraplon = -180.0;
#endif

   confml = 0;

}


PlanetNav :: ~PlanetNav()
{

}


void PlanetNav :: wrappingLongitude( real limit )
{
     wraplon = limit;
}

real PlanetNav :: wrappingLongitude() const
{
     return wraplon;
}


real PlanetNav :: wrap( real longitude ) 
{
   while ( longitude < wraplon ) {
       longitude += fullcircle;
   }
   while ( longitude >= (wraplon + fullcircle) ) {    
       longitude -= fullcircle;
   }    

   return longitude;
}


void PlanetNav :: checkpos( real longitude, real latitude )
{
    if ( ! isfinite(latitude) || ! isfinite(longitude) ) {
       throw badlocation();   
    }

    if ( latitude < -90.0 || latitude > 90.0 ) {
       throw badlocation();   
    }
    
}


void PlanetNav :: deltapos( real *longitude, real *latitude, real deltalon, real deltalat, real factor )
{

   try {
   
      *latitude += deltalat*factor;
      if ( *latitude > 90.0 ) {
         // adjust for displacement over the North pole
         *latitude = 90.0 - (*latitude - 90.0);
         *longitude += 180.0;   
      } else {
         if ( *latitude < -90.0 ) {
         // adjust for displacement over the South pole
            *latitude = -90.0 - (*latitude + 90.0);
            *longitude += 180.0;   
         }
      }
      checkpos(*longitude, *latitude);

      *longitude = PlanetNav::wrap( *longitude + deltalon*factor );

   } catch ( badlocation badloc ) {
       throw badincrement();
   }
}

void PlanetNav :: deltapos( int n, real *longitude, real *latitude, const real *deltalon, const real *deltalat, real factor)
{   
   for ( int i=0; i<n; i++ ) { 

       try {
   
          if ( FINITE(deltalat[i]) && FINITE(deltalon[i]) ) {
             latitude[i] += deltalat[i]*factor;
             if ( latitude[i] > 90.0 ) {
                // adjust for displacement over the North pole
                latitude[i] = 90.0 - (latitude[i] - 90.0);
                longitude[i] += 180.0;   
             } else {
                if ( latitude[i] < -90.0 ) {
                // adjust for displacement over the South pole
                   latitude[i] = -90.0 - (latitude[i] + 90.0);
                   longitude[i] += 180.0;   
                }
             }
             checkpos(longitude[i], latitude[i]);

             longitude[i] = PlanetNav::wrap( longitude[i] + deltalon[i]*factor );
             }

       } catch ( badlocation badloc ) {
           throw badincrement();
       }
   }

}
  
void PlanetNav :: conformal( int mode )
{
    confml = mode;
} 

int PlanetNav :: conformal()
{
    return confml;
}

