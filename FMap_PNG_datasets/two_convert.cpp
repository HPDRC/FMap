#include <cmath>
#include "two_convert.h"
//#include "stdafx.h"
using namespace std;

Gdc_Coord_3d::Gdc_Coord_3d() : 
		longitude(0.0), latitude(0.0), elevation(0.0) {}
Gdc_Coord_3d::Gdc_Coord_3d( double lat, double lon, double e ) :
		longitude(lon), latitude(lat), elevation(e) {}

Utm_Coord_3d::Utm_Coord_3d() : 
	x(0.0), y(0.0), z(0.0), zone(0), hemisphere_north(false)  {}
Utm_Coord_3d::Utm_Coord_3d(double X, double Y, double Z, short Zone, bool hemi_n) :
	x(X), y(Y), z(Z), zone(Zone), hemisphere_north(hemi_n)  {}

const double Gdc_To_Utm_Converter::RADIANS_PER_DEGREE = 0.0174532925199432957692;
const double Gdc_To_Utm_Converter::_PI = 4.0 * atan(1.0);
    
double			  Gdc_To_Utm_Converter::A,
                  Gdc_To_Utm_Converter::F,
                  Gdc_To_Utm_Converter::C,
                  Gdc_To_Utm_Converter::Eps2,
                  Gdc_To_Utm_Converter::Eps25,
                  Gdc_To_Utm_Converter::Epps2,   
                  Gdc_To_Utm_Converter::CScale = .9996,
                  Gdc_To_Utm_Converter::poly1b,
                  Gdc_To_Utm_Converter::poly2b,
                  Gdc_To_Utm_Converter::poly3b,
                  Gdc_To_Utm_Converter::poly4b,
                  Gdc_To_Utm_Converter::poly5b;

void Gdc_To_Utm_Converter::Init()
    {
        CreateConstants(6378137,298.257223563); // default to wgs 84
    }

void Gdc_To_Utm_Converter::CreateConstants(double a, double f)
    {
 
        double polx2b,polx3b,polx4b,polx5b;
 
        A = a;
        F = f;
        
        //  Create the ERM constants.
        
        F      =1/(F);
        C      =(A) * (1-F); 
        Eps2   =(F) * (2.0-F);
        Eps25  =.25 * (Eps2);
        Epps2  =(Eps2) / (1.0 - Eps2);
        polx2b = 1.0 * Eps2 + 1.0/4.0 * pow(Eps2,2) 
                 + 15.0/128.0 * pow(Eps2,3) - 455.0/4096.0 * 
                 pow(Eps2,4);
                 
        polx2b = 3.0/8.0 * polx2b;
  
        polx3b = 1.0 * pow(Eps2,2) + 3.0/4.0 * 
                 pow(Eps2,3) - 77.0/128.0 * pow(Eps2,4); 
                           
        polx3b = 15.0 / 256.0 * polx3b;

        polx4b = pow(Eps2,3) - 41.0/32.0 * pow(Eps2,4);
  
        polx4b = polx4b * 35.0 / 3072.0;

        polx5b =  -315.0/131072.0 * pow(Eps2,4);

        poly1b = 1.0 - (1.0/4.0 * Eps2) - ( 3.0/64.0 * pow(Eps2,2))
                 - (5.0/256.0 * pow(Eps2,3) ) -
                 (175.0/16384.0 * pow(Eps2,4));

        poly2b = polx2b * -2.0 + polx3b * 4.0 - polx4b * 6.0 + polx5b * 8.0;
 
        poly3b = polx3b * -8.0 + polx4b * 32.0 - polx5b * 80.0;
 
        poly4b = polx4b * -32.0 + polx5b * 192.0;
  
        poly5b = polx5b * -128.0;
        
    } // end init

void Gdc_To_Utm_Converter::Convert(Gdc_Coord_3d gdc_coord, Utm_Coord_3d& utm_coord)
    {
        Gdc_Coord_3d gdc[1];
        Utm_Coord_3d utm[1];
        
        gdc[0] = gdc_coord;
        
        Convert(1,gdc,utm);

		utm_coord = utm[0];
    }

void Gdc_To_Utm_Converter::Convert(int count, const Gdc_Coord_3d gdc[], Utm_Coord_3d utm[] )
    {
        double source_lat, source_lon, s1,c1,tx,s12,rn,axlon0,al,al2,sm,tn2,cee,poly1,poly2;

        for( int i=0; i < count; i++)
        {
            
            utm[i].z = gdc[i].elevation;
            
            if (gdc[i].latitude < 0)
                utm[i].hemisphere_north=false;
            else
                utm[i].hemisphere_north=true;

	    //if ( gdc[i].longitude < 0.0 )  // XXX - reddy, 11 Sep 98
	    //  gdc[i].longitude += 360.0;

            source_lat = gdc[i].latitude * RADIANS_PER_DEGREE;
            source_lon = gdc[i].longitude * RADIANS_PER_DEGREE;

            s1 = sin(source_lat);
            c1 = cos(source_lat);
            tx = s1 / c1;
            s12 = s1 * s1;

            /* USE IN-LINE SQUARE ROOT */

            rn = A / ( (.25 - Eps25*s12 + .9999944354799/4) + (.25-Eps25*s12)/(.25 - Eps25*s12 + .9999944354799/4));
    
             
            /* COMPUTE UTM COORDINATES */

            /* Compute Zone */

            utm[i].zone = (short) (source_lon * 30.0 / _PI + 31);

            if(utm[i].zone <=0)
                utm[i].zone = 1;
            else
                if(utm[i].zone >=61) 
                    utm[i].zone=60;

            axlon0 = (utm[i].zone * 6 - 183) * RADIANS_PER_DEGREE;

            al = (source_lon - axlon0) * c1;

            sm = s1 * c1 * (poly2b + s12 * (poly3b + s12 * 
                   (poly4b + s12 * poly5b)));

            sm = A * (poly1b * source_lat + sm);
             
            tn2 = tx * tx;
            cee = Epps2 * c1 * c1;
            al2 = al * al;
            poly1 = 1.0 - tn2 + cee;
            poly2 = 5.0 + tn2 * (tn2 - 18.0)  + cee * (14.0 - tn2 * 58.0);

           /* COMPUTE EASTING */
    
           utm[i].x = CScale * rn * al * (1.0 + al2 *(.166666666666667 * poly1 +.00833333333333333 * al2 * poly2));
             
           utm[i].x += 5.0E5;

            /* COMPUTE NORTHING */
             
            poly1 = 5.0 - tn2 + cee * (cee * 4.0 + 9.0);
            poly2 = 61.0 + tn2 * (tn2 - 58.0) + cee * (270.0 - tn2 * 330.0);

            utm[i].y = CScale * (sm + rn * tx * al2 * (0.5 + al2 * (.0416666666666667 * poly1 + .00138888888888888 * al2 * poly2)));

            if (source_lat < 0.0)
                utm[i].y += 1.0E7;
            } // end for
            
                    
    } // end Convert    

const double Utm_To_Gdc_Converter::DEGREES_PER_RADIAN = 57.2957795130823208768;

double			  Utm_To_Gdc_Converter::A,
                  Utm_To_Gdc_Converter::F,
                  Utm_To_Gdc_Converter::C,
                  Utm_To_Gdc_Converter::Eps2,
                  Utm_To_Gdc_Converter::Eps21,
                  Utm_To_Gdc_Converter::Eps25,
                  Utm_To_Gdc_Converter::Con,
                  Utm_To_Gdc_Converter::Con2,
                  Utm_To_Gdc_Converter::EF,
                  Utm_To_Gdc_Converter::Epsp2,
                  Utm_To_Gdc_Converter::Con6,
                  Utm_To_Gdc_Converter::Con24,
                  Utm_To_Gdc_Converter::Con120,
                  Utm_To_Gdc_Converter::Con720,
                  Utm_To_Gdc_Converter::polx2b,
                  Utm_To_Gdc_Converter::polx3b,
                  Utm_To_Gdc_Converter::polx4b,
                  Utm_To_Gdc_Converter::polx5b,
                  Utm_To_Gdc_Converter::conap;


void Utm_To_Gdc_Converter::Init()
    {
        CreateConstants(6378137,298.257223563); // default to wgs 84
    }

void Utm_To_Gdc_Converter::CreateConstants(double a, double f)
    {
        double polx1a,polx2a,polx4a,polx6a,polx8a;
        
        A = a;
        F = f;

        //  Create the ERM constants.

        F      = 1/(F);
        C      = (A) * (1-F);
        Eps2   = (F) * (2.0-F);
        Eps25  = .25 * (Eps2);
        EF     = F/(2.0-F);
        Con    = (1.0-Eps2);
        Con2   = 2 / (1.0-Eps2);
        Con6   = .166666666666667;
        Con24  = 4 * .0416666666666667/(1-Eps2);
        Con120 = .00833333333333333;
        Con720 = 4 * .00138888888888888/(1-Eps2);
        polx1a = 1.0 - Eps2 / 4.0 - 3.0/64.0 *
                          pow(Eps2,2) - 5.0/256.0 * pow(Eps2,3)
                          - 175.0/16384.0 * pow(Eps2,4);

        conap  = A * polx1a;

        polx2a = 3.0/2.0 * EF - 27.0/32.0 * pow(EF,3);

        polx4a = 21.0/16.0 * pow(EF,2) - 55.0/32.0 * pow(EF,4);

        polx6a = 151.0/96.0 * pow(EF,3);

        polx8a = 1097.0/512.0 * pow(EF,4);

        polx2b = polx2a * 2.0 + polx4a * 4.0 + polx6a * 6.0 + polx8a * 8.0;

        polx3b = polx4a * -8.0 - polx6a * 32.0- 80.0 *polx8a;
        polx4b = polx6a * 32.0 + 192.0*polx8a;
        polx5b = -128.0 *polx8a;


    } // end init

void Utm_To_Gdc_Converter::Convert(Utm_Coord_3d utm_coord, Gdc_Coord_3d& gdc_coord)
    {
        Utm_Coord_3d utm[1];
        Gdc_Coord_3d gdc[1];

        utm[0] = utm_coord;

        Convert(1,utm,gdc);

		gdc_coord = gdc[0];
    }

void Utm_To_Gdc_Converter::Convert(int count, const Utm_Coord_3d utm[], Gdc_Coord_3d gdc[])
    {
        double source_x, source_y,u,su,cu,su2,xlon0,temp,phi1,sp,sp2,cp,cp2,tp,tp2,eta2,top,rn,b3,b4,b5,b6,d1,d2;

        for(int i=0; i < count; i++)
        {

            gdc[i].elevation = utm[i].z;

            source_x = utm[i].x;

            source_x = (source_x - 500000.0)/.9996;

            if (utm[i].hemisphere_north==true)
                source_y = utm[i].y / .9996;
            else
                source_y = (utm[i].y - 1.0E7)/.9996;

            u = source_y / conap;

            /* TEST U TO SEE IF AT POLES */

            su = sin(u);
            cu = cos(u);
            su2 = su * su;

            /* THE SNYDER FORMULA FOR PHI1 IS OF THE FORM
             PHI1=U+POLY2A*SIN(2U)+POLY3A*SIN(4U)+POLY4ASIN(6U)+...
             BY USING MULTIPLE ANGLE TRIGONOMETRIC IDENTITIES AND APPROPRIATE FACTORING
            JUST THE SINE AND COSINE ARE REQUIRED
             NOW READY TO GET PHI1
             */

            xlon0= ( 6.0 * ((double) utm[i].zone) - 183.0) / DEGREES_PER_RADIAN;

            temp = polx2b + su2 * (polx3b + su2 * (polx4b + su2 * polx5b));

            phi1 = u + su * cu * temp;

             /* COMPUTE VARIABLE COEFFICIENTS FOR FINAL RESULT
                COMPUTE THE VARIABLE COEFFICIENTS OF THE LAT AND LON
                EXPANSIONS */

            sp = sin(phi1);
            cp = cos(phi1);
            tp = sp / cp;
            tp2 = tp * tp;
            sp2 = sp * sp;
            cp2 = cp * cp;
            eta2 = Epsp2 * cp2;
            top = .25-(sp2*(Eps2 / 4));
            rn = A / ( (.25 - Eps25 * sp2 + .9999944354799/4) +
                (.25-Eps25 * sp2)/(.25 - Eps25 * sp2 + .9999944354799/4));
            b3 = 1.0 + tp2 + tp2 + eta2;
            b4 = 5 + tp2 * (3 - 9 * eta2) + eta2 * ( 1 - 4 * eta2);
            b5 = 5 + tp2 * (tp2 * 24.0 + 28.0);
            b5 += eta2 * (tp2 * 8.0 + 6.0);
            b6 = 46.0 - 3.0 * eta2 + tp2 * (-252.0 - tp2 * 90.0);
            b6 = eta2 * (b6 + eta2 * tp2 * (tp2 * 225.0 - 66.0));
            b6 += 61.0 + tp2 * (tp2 * 45.0 + 90.0);
            d1 = source_x / rn;
            d2 = d1 * d1;
            gdc[i].latitude = phi1 - tp * top * (d2 * (Con2 + d2 * ((-Con24) * b4 + d2 *
                        Con720 * b6)));
            gdc[i].longitude = xlon0 + d1 * (1.0 + d2 * (-Con6 * b3 + d2 * Con120 * b5)) / cp;
            gdc[i].latitude *= DEGREES_PER_RADIAN;
            gdc[i].longitude *= DEGREES_PER_RADIAN;
        } 
    }
