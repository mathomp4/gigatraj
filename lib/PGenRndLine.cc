
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "gigatraj/PGenRndLine.hh"

using namespace gigatraj;



PGenRndLine::PGenRndLine()
{
   rnd.seed();
}


template< template<class U, class = std::allocator<U> > class Seq>
void PGenRndLine::init( Seq<Parcel>* seq, const Parcel& p
                     , real beglon, real beglat
                     , real endlon, real endlat
                     , real begz, real endz
                   )
{
     PlanetNav *e;
     real lon, lat;
     real dist, totaldist, bear;
     
     if ( seq->size() <= 0 ) {
        throw  (ParcelGenerator :: badparcelcount());
     }

     e = p.getNav();

     // The line stretches form (beglon,beglat) to (endlon,endlat)
     totaldist = e->distance( beglon,beglat, endlon,endlat );
     bear = e->bearing( beglon,beglat, endlon,endlat );

     try {                                       
         typename Seq<Parcel>::iterator it;     
         it = seq->begin();
         while ( it != seq->end() ) {
         
         
             dist = rnd.uniform(0.0, totaldist);
             e->displace( beglon, beglat, dist, bear, lon, lat );
          
             it->setPos( lon, lat );
             it->setZ( rnd.uniform( begz, endz ) );
          
             it++;
         }
    } catch (...) {
         throw (ParcelGenerator :: badgeneration());
    }
                                

}

Parcel * PGenRndLine::create_array(const Parcel& p, int np
                     , real beglon, real beglat
                     , real endlon, real endlat
                     , real begz, real endz
                     )
{
     PlanetNav* e;
     real lon, lat;
     real dist, totaldist, bear;
     Parcel* pa;

     e = p.getNav();

     // The line stretches form (beglon,beglat) to (endlon,endlat)
     totaldist = e->distance( beglon,beglat, endlon,endlat );
     bear = e->bearing( beglon,beglat, endlon,endlat );


     try {
         
         pa = new Parcel[np];
         
         for ( int i=0; i < np; i++ ) {          
         
             dist = rnd.uniform(0.0, totaldist);
             e->displace( beglon, beglat, dist, bear, lon, lat );
          
             pa[i] = p;
             pa[i].setPos( lon, lat );
             pa[i].setZ( rnd.uniform( begz, endz ) );
          
         }
    } catch (...) {
         throw (ParcelGenerator :: badgeneration());
    }
                                
    return pa;
}


std::vector<Parcel>* PGenRndLine::create_vector(const Parcel& p, int np
                    , real beglon, real beglat
                    , real endlon, real endlat
                    , real begz, real endz
                    )
{
    std::vector<Parcel>*bunch;
    
    if ( np <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       bunch = new std::vector<Parcel>;
       bunch->reserve(np);
    
       for (int i=0; i < np; i++ ) {
          bunch->push_back(p);
       }
       
       init(bunch, p, beglon,beglat, endlon,endlat, begz,endz );

    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return bunch;

}

std::list<Parcel>* PGenRndLine::create_list(const Parcel& p, int np
                    , real beglon, real beglat
                    , real endlon, real endlat
                    , real begz, real endz
                    )
{
    std::list<Parcel>*bunch;
    
    if ( np <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       bunch = new std::list<Parcel>;
    
       init(bunch, p, beglon,beglat, endlon,endlat, begz,endz );

    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return bunch;

}

std::deque<Parcel>* PGenRndLine::create_deque(const Parcel& p, int np
                    , real beglon, real beglat
                    , real endlon, real endlat
                    , real begz, real endz
                    )
{
    std::deque<Parcel>*bunch;
    
    if ( np <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       bunch = new std::deque<Parcel>;
    
       init(bunch, p, beglon,beglat, endlon,endlat, begz,endz );

    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return bunch;

}


Flock* PGenRndLine::create_Flock(const Parcel& parcel, int np
                    , real beglon, real beglat
                    , real endlon, real endlat
                    , real begz, real endz
                    , ProcessGrp* pgrp, int r
                    )
{
     // the parcel container
     Flock *flock;
     Flock::iterator ip;         
     real lon,lat,z;
     real totaldist, bear, dist;
     PlanetNav* e;

     int status = 0;

     if ( np <= 0 ) {
        throw (ParcelGenerator :: badparcelcount());
     };  

     e = parcel.getNav();

     // The line stretches form (beglon,beglat) to (endlon,endlat)
     totaldist = e->distance( beglon,beglat, endlon,endlat );
     bear = e->bearing( beglon,beglat, endlon,endlat );

    
     try {
     
        // now create a Flock os that many parels
        flock = new Flock( parcel, pgrp, np, r);

        // sync all the processors before we start loading
        if ( pgrp != NULLPTR ) {
           pgrp->sync();
        }

        for ( ip=flock->begin(); ip != flock->end(); ip++ ) {
            try {
               // generate a random position
               
         
               dist = rnd.uniform(0.0, totaldist);
               e->displace( beglon, beglat, dist, bear, lon, lat );
               
               // load the parcel location into the parcel
               ip->setPos( lon, lat );
          
               ip->setZ( rnd.uniform( begz, endz ));

            } catch (...) {
               throw (ParcelGenerator :: badgeneration());
            }   
        }   
     
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     


    return flock;


}


Swarm* PGenRndLine::create_Swarm(const Parcel& parcel, int np
                    , real beglon, real beglat
                    , real endlon, real endlat
                    , real begz, real endz
                    , ProcessGrp* pgrp, int r
                    )
{
     // the parcel container
     Swarm *swarm;
     Swarm::iterator ip;         
     real lon,lat,z;
     real totaldist, bear, dist;
     PlanetNav* e;

     int status = 0;

     if ( np <= 0 ) {
        throw (ParcelGenerator :: badparcelcount());
     };  

     e = parcel.getNav();

     // The line stretches form (beglon,beglat) to (endlon,endlat)
     totaldist = e->distance( beglon,beglat, endlon,endlat );
     bear = e->bearing( beglon,beglat, endlon,endlat );

    
     try {
     
        // now create a Swarm os that many parels
        swarm = new Swarm( parcel, pgrp, np, r);

        // sync all the processors before we start loading
        if ( pgrp != NULLPTR ) {
           pgrp->sync();
        }

        for ( ip=swarm->begin(); ip != swarm->end(); ip++ ) {
            try {
               // generate a random position
               
         
               dist = rnd.uniform(0.0, totaldist);
               e->displace( beglon, beglat, dist, bear, lon, lat );
               
               // load the parcel location into the parcel
               ip->setPos( lon, lat );
          
               ip->setZ( rnd.uniform( begz, endz ));

            } catch (...) {
               throw (ParcelGenerator :: badgeneration());
            }   
        }   
     
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     


    return swarm;


}


