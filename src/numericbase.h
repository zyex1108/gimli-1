/***************************************************************************
 *   Copyright (C) 2006-2011 by the resistivity.net development team       *
 *   Carsten Rücker carsten@resistivity.net                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _GIMLI_NUMERICBASE__H
#define _GIMLI_NUMERICBASE__H

#include "gimli.h"
#include <cmath>

namespace GIMLI{

#ifndef PI
    #define PI 3.141592653589793238462643383279502884197169399375105820974944592308
    #define PI2 6.2831853071795864769252867665590057683942387987502116419498891846
    #define PI_2 1.5707963267948967384626433682795028841971193993751058209749445923
#endif

template < class ValueType > ValueType round( const ValueType & v, ValueType tol ){ return ::rint( v / tol ) * tol; }

/*! Converts a degree value to radian.*/
template < class ValueType > ValueType degToRad( const ValueType & deg ){ return deg * (2.0 * PI) / 360.0; }

/*! Converts a radian value to degree.*/
template < class ValueType > ValueType radToDeg( const ValueType & rad ){ return rad * 360.0 / (2.0 * PI); }

template < class T > T powInt( const T & a, uint dim ){ 
    switch ( dim ){
        case 0 : return 1.0;
        case 1 : return a;
        case 2 : return a*a;
        case 3 : return a*a*a;
        case 4 : return a*a*a*a; 
        default: return std::pow( a, dim );
    }
}


/*!  Given the lower and upper limits of integration x1 and x2 and given n,
  this routine returns vector x(0..n-1) and w(0..n-1) of length n,
  containing the abscissas and weights of the Gauss-Legendre
  n-point quadrature formula. */
DLLEXPORT void GaussLegendre( double x1, double x2, uint n, RVector & x, RVector & w );

/*!  Given alpha = 0.0, the parameter alpha of the Laguerre polynomials, this routine
  returns vector x[0..n-1] and w[0..n-1] containing the abscissas and weights
  of the n-point Gauss-Laguerre quadrature formula. The smallest abscissa
  is returned in x[ 0 ], the largest in x[ n-1 ]. */
DLLEXPORT void GaussLaguerre( uint n, RVector & x, RVector & w );

//! Caluculate modified Bessel function of the first kind
/*! See Abramowitz: Handbook of math. functions; */
//DLLEXPORT double besselI0( double x );
template < class ValueType > ValueType besselI0( const ValueType & x ) {
    ValueType ax = std::fabs( x ), result = 0.0, y = 0.0;

    if ( ax < 3.75 ) {
        y = x / 3.75, y = y * y ;
        result = 1.0 + y * ( 3.5156229 +
             y * ( 3.0899424 +
                   y * ( 1.2067492 +
                     y * ( 0.2659732 +
                       y * ( 0.360768e-1
                         + y * 0.45813e-2 ) ) ) ) );
    } else {
        y = 3.75 / ax;
        result = ( std::exp( ax ) / std::sqrt( ax ) ) * ( 0.39894228 +
                        y * ( 0.1328592e-1 +
                          y * ( 0.225319e-2 +
                            y * ( -0.157565e-2 +
                                  y * ( 0.916281e-2 +
                                    y * ( -0.2057706e-1 +
                                      y * ( 0.2635537e-1 +
                                        y * ( -0.1647633e-1 +
                                              y * 0.392377e-2 ) ) ) ) ) ) ) );
    }
    return result;
}

//! Caluculate modified Bessel function of the first kind
/*! See Abramowitz: Handbook of math. functions */
//DLLEXPORT double besselI1( double x );
template < class ValueType > ValueType besselI1( const ValueType & x ) {
  ValueType ax = std::fabs( x ), result = 0.0, y = 0.0;

  if ( ax < 3.75 ) {
    y = x / 3.75, y =y * y;
    result = ax * ( 0.5 +
            y * ( 0.87890594 +
              y * ( 0.51498869 +
                y * ( 0.15084934 +
                      y * ( 0.2658733e-1 +
                        y * ( 0.301532e-2 +
                          y * 0.32411e-3) ) ) ) ) );
  } else {
    y = 3.75 / ax;
    result = 0.2282967e-1 + y * ( -0.2895312e-1 + y * ( 0.1787654e-1 -y * 0.420059e-2 ) );
    result = 0.39894228   + y * ( -0.3988024e-1 + y * ( -0.362018e-2 +
                            y * ( 0.163801e-2 + y * ( -0.1031555e-1 + y * result ) ) ) ) ;
    result *= ( std::exp( ax ) / std::sqrt( ax ) );
  }
  return x < 0.0 ? -result : result;
}


//! Caluculate modified Bessel function of the second kind
/*! See Abramowitz: Handbook of math. functions */
//DLLEXPORT double besselK0( double x );
template < class ValueType > ValueType besselK0( const ValueType & x ){
    ValueType y = 0.0, result = 0.0;

    if ( x <= 2.0 ) {
        y = x * x / 4.0;
        result = ( -std::log( x / 2.0 ) * besselI0( x ) ) + ( -0.57721566
                          + y * ( 0.42278420
                              + y * ( 0.23069756
                                  + y * ( 0.3488590e-1
                                      + y * ( 0.262698e-2
                                          + y * ( 0.10750e-3
                                              + y * 0.74e-5 ) ) ) ) ) );
    } else {
        y = 2.0 / x;
        result = ( std::exp( -x ) / std::sqrt( x ) ) * ( 1.25331414
                       + y * ( -0.7832358e-1
                           + y * ( 0.2189568e-1
                               + y * ( -0.1062446e-1
                                   + y * ( 0.587872e-2
                                       + y * ( -0.251540e-2
                                           + y * 0.53208e-3 ) ) ) ) ) );
    }
    return result;
}

//! Caluculate modified Bessel function of the second kind
/*! See Abramowitz: Handbook of math. functions */
//DLLEXPORT double besselK1( double x );
template < class ValueType > ValueType besselK1( const ValueType & x ){
    ValueType y = 0.0, result = 0.0;

    if ( x <= 2.0 ) {
        y = x * x / 4.0;
        result = ( std::log( x / 2.0 ) * besselI1( x ) ) +
        ( 1.0 / x ) * ( 1.0 + y * ( 0.15443144 + y * ( -0.67278579 + y * ( -0.18156897 +
                                     y * ( -0.1919402e-1 +
                                           y * ( -0.110404e-2 +
                                             y * ( -0.4686e-4 ) ) ) ) ) ) ) ;
    } else {
        y = 2.0 / x;
        result = ( std::exp( -x ) / std::sqrt( x ) ) * ( 1.25331414 + y * ( 0.23498619 +
                                  y * ( -0.3655620e-1 +
                                    y * ( 0.1504268e-1 +
                                      y * ( -0.780353e-2 +
                                        y * ( 0.325614e-2 +
                                              y * ( -0.68245e-3 ) ) ) ) ) ) );
    }
    return result;
}

} // namespace GIMLI

#endif // _GIMLI_NUMERICBASE__H
