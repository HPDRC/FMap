#ifndef TWO_CONVERT_H
#define TWO_CONVERT_H

#include <math.h>


class Gdc_Coord_3d {
public: 
	double longitude;
	double latitude;
	double elevation;
	Gdc_Coord_3d();
	Gdc_Coord_3d( double lat, double lon, double e );   
};

class Utm_Coord_3d { 
public: 
	double x;
	double y;
	double z;
	short  zone;
	bool   hemisphere_north; // true == north false == south

  Utm_Coord_3d();
  Utm_Coord_3d(double X, double Y, double Z, short Zone, bool hemi_n);
};

class Utm_To_Gdc_Converter
{
private:
    static const double DEGREES_PER_RADIAN;

    static double A,
                  F,
                  C,
                  Eps2,
                  Eps21,
                  Eps25,
                  Con,
                  Con2,
                  EF,
                  Epsp2,
                  Con6,
                  Con24,
                  Con120,
                  Con720,
                  polx2b,
                  polx3b,
                  polx4b,
                  polx5b,
                  conap;

public: 
	static void Init();
protected: 
	static void CreateConstants(double a, double f);
public: 
	static void Convert(Utm_Coord_3d utm_coord, Gdc_Coord_3d& gdc_coord);
public: 
	static void Convert(int count, const Utm_Coord_3d utm[], Gdc_Coord_3d gdc[] );
};

class Gdc_To_Utm_Converter
{
private:
    static const double RADIANS_PER_DEGREE;
    static const double _PI;
    
    static double A,
                  F,
                  C,
                  Eps2,
                  Eps25,
                  Epps2,   
                  CScale,
                  poly1b,
                  poly2b,
                  poly3b,
                  poly4b,
                  poly5b;
public: 
	static void Init();                   
protected: 
	static void CreateConstants(double a, double f);
public: 
	static void Convert(Gdc_Coord_3d gdc_coord, Utm_Coord_3d& utm_coord);
public: 
	static void Convert(int count, const Gdc_Coord_3d gdc[], Utm_Coord_3d utm[] );    
}; 

#endif 