/***************************************************************************
 *   Copyright (C) 2006-2012 by the resistivity.net development team       *
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

#include "integration.h"

#include "shape.h"
#include "pos.h"

namespace GIMLI{

template <typename IntegrationRules> IntegrationRules * Singleton < IntegrationRules>::pInstance_ = NULL;

IntegrationRules::IntegrationRules(){
    initGau_();
    initTriGL_();

    initEdg_();
    initTri_();
    initTet_();
    initQua_();
    initHex_();
}

IntegrationRules::~IntegrationRules(){
// std::cout << " delete IntegrationRules" << std::endl;
}

const std::vector < RVector3 > & IntegrationRules::abscissa( const Shape & shape, uint order ) const {
    switch( shape.rtti() ){
        case MESH_SHAPE_EDGE_RTTI: return edgAbscissa_[ order ];
        case MESH_SHAPE_TRIANGLE_RTTI: return triGLAbscissa_[ order ];
        case MESH_SHAPE_QUADRANGLE_RTTI: return quaAbscissa_[ order ];
        case MESH_SHAPE_TETRAHEDRON_RTTI: return tetAbscissa_[ order ];
        case MESH_SHAPE_HEXAHEDRON_RTTI: return hexAbscissa_[ order ];
        default:
            break;
    }
    return gauAbscissa_[ order ];
}

const RVector & IntegrationRules::weights( const Shape & shape, uint order ) const {
    switch( shape.rtti() ){
        case MESH_SHAPE_EDGE_RTTI: return edgWeights_[ order ];
        case MESH_SHAPE_TRIANGLE_RTTI: return triGLWeights_[ order ];
        case MESH_SHAPE_QUADRANGLE_RTTI: return quaWeights_[ order ];
        case MESH_SHAPE_TETRAHEDRON_RTTI: return tetWeights_[ order ];
        case MESH_SHAPE_HEXAHEDRON_RTTI: return hexWeights_[ order ];
        default:
            break;
    }
    return gauWeights_[ order ];
}

void IntegrationRules::initGau_(){
    //** 0.Order, n=1 -- just placeholder
    gauAbscissa_.push_back( std::vector < RVector3 > ( 0 ) );
    gauWeights_.push_back( RVector( 0, 0.0 ) );
    //** 1.Order, n=1
    gauAbscissa_.push_back( std::vector < RVector3 > ( 1 ) );
    gauAbscissa_.back()[ 0 ] = RVector3( 0.0, 0.0 );
    gauWeights_.push_back( RVector( 1, 2.0 ) );

    //** 2.Order, n=2
    gauAbscissa_.push_back( std::vector < RVector3 > ( 2 ) );
    gauAbscissa_.back()[ 0 ] = RVector3( -::sqrt( 1.0 / 3.0 ), 0.0 ); // ~ -0.577350269189626
    gauWeights_.push_back( RVector( 2, 1.0 ) );

    //** 3.Order, n=3
    gauAbscissa_.push_back( std::vector < RVector3 > ( 3 ) );
    gauAbscissa_.back()[ 0 ] = RVector3( -::sqrt( 3.0 / 5.0 ), 0.0 ); // -0.774596669241483
    gauAbscissa_.back()[ 1 ] = RVector3(  0.0, 0.0 );
    gauWeights_.push_back( RVector( 3, 5.0 / 9.0 ) );
    gauWeights_.back()[ 1 ] = 8.0 / 9.0;

    //** 4.Order, n=4
    gauAbscissa_.push_back( std::vector < RVector3 > ( 4 ) );
    gauAbscissa_.back()[ 0 ] = RVector3( -::sqrt( 3.0 / 7.0 + 2.0 / 7.0 * ::sqrt( 6.0 / 5.0 ) ), 0.0 ); // -0.861136311594053
    gauAbscissa_.back()[ 1 ] = RVector3( -::sqrt( 3.0 / 7.0 - 2.0 / 7.0 * ::sqrt( 6.0 / 5.0 ) ), 0.0 ); // -0.339981043584856
    
    gauWeights_.push_back( RVector( 4, 0.0 ) );
    gauWeights_.back()[ 0 ] = ( 18.0 - ::sqrt( 30.0 ) ) / 36.0; // 0.347854845137454;
    gauWeights_.back()[ 1 ] = ( 18.0 + ::sqrt( 30.0 ) ) / 36.0; // 0.652145154862546;
    
    //** 5.Order, n=5
    gauAbscissa_.push_back( std::vector < RVector3 > ( 5 ) );
    gauAbscissa_.back()[ 0 ] = RVector3( -1.0 / 3.0 * ::sqrt( 5.0 + 2.0 * ::sqrt( 10.0 / 7.0 ) ), 0.0 ); // -0.906179845938664
    gauAbscissa_.back()[ 1 ] = RVector3( -1.0 / 3.0 * ::sqrt( 5.0 - 2.0 * ::sqrt( 10.0 / 7.0 ) ), 0.0 );
    gauAbscissa_.back()[ 2 ] = RVector3(  0.000000000000000, 0.0 );
    gauWeights_.push_back( RVector( 5, 0.0 ) );
    gauWeights_.back()[ 0 ] = ( 322.0 - 13.0 * ::sqrt( 70 ) ) / 900.0;
    gauWeights_.back()[ 1 ] = ( 322.0 + 13.0 * ::sqrt( 70 ) ) / 900.0; //0.478628670499366;
    gauWeights_.back()[ 2 ] = 128.0 / 225.0;//0.568888888888889;

    //** 6.Order, n=6
    gauAbscissa_.push_back( std::vector < RVector3 > ( 6 ) );
    gauAbscissa_.back()[ 0 ] = RVector3( -0.9324695142031520278123016, 0.0 );
    gauAbscissa_.back()[ 1 ] = RVector3( -0.6612093864662645136613996, 0.0 );
    gauAbscissa_.back()[ 2 ] = RVector3( -0.2386191860831969086305017, 0.0 );
    gauWeights_.push_back( RVector( 6, 0.0 ) );
    gauWeights_.back()[ 0 ] = 0.1713244923791703450402961;
    gauWeights_.back()[ 1 ] = 0.3607615730481386075698335;
    gauWeights_.back()[ 2 ] = 0.4679139345726910473898703;

    //** 7.Order, n=7
    gauAbscissa_.push_back( std::vector < RVector3 > ( 7 ) );
    gauAbscissa_.back()[ 0 ] = RVector3( -0.9491079123427585245261897, 0.0 );
    gauAbscissa_.back()[ 1 ] = RVector3( -0.7415311855993944398638648, 0.0 );
    gauAbscissa_.back()[ 2 ] = RVector3( -0.4058451513773971669066064, 0.0 );
    gauAbscissa_.back()[ 3 ] = RVector3( 0.000000000000000, 0.0 );
    gauWeights_.push_back( RVector( 7, 0.0 ) );
    gauWeights_.back()[ 0 ] = 0.1294849661688696932706114;
    gauWeights_.back()[ 1 ] = 0.2797053914892766679014678;
    gauWeights_.back()[ 2 ] = 0.3818300505051189449503698;
    gauWeights_.back()[ 3 ] = 0.4179591836734693877551020;

    //** 8.Order, n=8
    gauAbscissa_.push_back( std::vector < RVector3 > ( 8 ) );
    gauAbscissa_.back()[ 0 ] = RVector3( -0.9602898564975362316835609, 0.0 );
    gauAbscissa_.back()[ 1 ] = RVector3( -0.7966664774136267395915539, 0.0 );
    gauAbscissa_.back()[ 2 ] = RVector3( -0.5255324099163289858177390, 0.0 );
    gauAbscissa_.back()[ 3 ] = RVector3( -0.1834346424956498049394761, 0.0 );
    gauWeights_.push_back( RVector( 8, 0.0 ) );
    gauWeights_.back()[ 0 ] = 0.1012285362903762591525314;
    gauWeights_.back()[ 1 ] = 0.2223810344533744705443560;
    gauWeights_.back()[ 2 ] = 0.3137066458778872873379622;
    gauWeights_.back()[ 3 ] = 0.3626837833783619829651504;
    
    //** 9.Order, n=9
    gauAbscissa_.push_back( std::vector < RVector3 > ( 9 ) );
    gauAbscissa_.back()[ 0 ] = RVector3( -0.9681602395076260898355762, 0.0 );
    gauAbscissa_.back()[ 1 ] = RVector3( -0.8360311073266357942994298, 0.0 );
    gauAbscissa_.back()[ 2 ] = RVector3( -0.6133714327005903973087020, 0.0 );
    gauAbscissa_.back()[ 3 ] = RVector3( -0.3242534234038089290385380, 0.0 );
    gauAbscissa_.back()[ 4 ] = RVector3( 0.000000000000000, 0.0 );
    gauWeights_.push_back( RVector( 9, 0.0 ) );
    gauWeights_.back()[ 0 ] = 0.0812743883615744119718922;
    gauWeights_.back()[ 1 ] = 0.1806481606948574040584720;
    gauWeights_.back()[ 2 ] = 0.2606106964029354623187429;
    gauWeights_.back()[ 3 ] = 0.3123470770400028400686304;
    gauWeights_.back()[ 4 ] = 0.3302393550012597631645251;

    for ( uint i = 2; i < gauAbscissa_.size(); i ++ ){
        uint n = gauAbscissa_[ i ].size();
        uint start = ::ceil( n/2.0 );
//         std::cout << "n = " << n << " s: " << start << " " << std::endl;
        for ( uint j = start; j < n; j ++ ){
            //std::cout << j << " <-> " << n - j -1<< std::endl;
            gauWeights_[ i ][ j ] = gauWeights_[ i ][ n - j -1 ];
            gauAbscissa_[ i ][ j ] = -gauAbscissa_[ i ][ n - j -1 ];
        }
    }

}

void IntegrationRules::initEdg_(){
    /* just transform \int_-1^1 to \int_0^1 */
    edgAbscissa_.push_back( std::vector < RVector3 > ( 0 ) );
    edgWeights_.push_back( RVector( 0, 0.0 ) );

    for ( uint i = 1; i < gauAbscissa_.size(); i ++ ){
        edgAbscissa_.push_back( std::vector < RVector3 > ( gauAbscissa_[ i ].size() ) );
        edgWeights_.push_back( 0.5 * gauWeights_[ i ] );

        for ( uint j = 0; j < gauAbscissa_[ i ].size(); j ++ ){
            edgAbscissa_[ i ][ j ] = gauAbscissa_[ i ][ j ] / 2.0 + RVector3( 0.5, 0.0 );
        }
    }
}

void IntegrationRules::initTriGL_(){
    
    //** 0.Order, n=1, Error: O(h1) -- just placeholder
    triGLAbscissa_.push_back( std::vector< RVector3 >( 0 ) );
    triGLWeights_.push_back( RVector( 0, 0.0 ) );

    for ( uint i = 1; i < 10; i ++ ){
//         std::cout << "i" << i << std::endl;
        triGLAbscissa_.push_back( std::vector< RVector3 >( i*i ) );
         triGLWeights_.push_back( RVector( i*i, 0.0 ) );
        
        for ( uint j = 0; j < i; j ++ ){
            for ( uint k = 0; k < i; k ++ ){
                double w = ( ( 1.0 - gauAbscissa_[ i ][ j ][ 0 ] ) / 8.0 ) * gauWeights_[ i ][ j ] * gauWeights_[ i ][ k ];
                double x =   ( 1.0 + gauAbscissa_[ i ][ j ][ 0 ] ) / 2.0;
                double y = ( ( 1.0 - gauAbscissa_[ i ][ j ][ 0 ] ) * ( 1.0 + gauAbscissa_[ i ][ k ][ 0 ] ) ) / 4.0;

                 triGLWeights_.back()[ j * i + k ] = w;
                triGLAbscissa_.back()[ j * i + k ] = RVector3( x, y );
            }
        }
    }
}
    
void IntegrationRules::initTri_(){
    //** 0.Order, n=1, Error: O(h1) -- just placeholder
    triAbscissa_.push_back( std::vector< RVector3 >( 0 ) );
    triWeights_.push_back( RVector( 0, 0.0 ) );

    //** 1.Order, n=1, Error: O(h2)
    triAbscissa_.push_back( std::vector< RVector3 >( 1 ) );
    triAbscissa_.back()[ 0 ] = RVector3( 1.0/3.0, 1.0/3.0  );
    triWeights_.push_back( RVector( 1, 1.0 ) );

        //** 2.Order, n=3, Error: O(h3)
    triAbscissa_.push_back( std::vector< RVector3 >( 3 ) );
    triAbscissa_.back()[ 0 ] = RVector3( 0.5, 0.0 );
    triAbscissa_.back()[ 1 ] = RVector3( 0.5, 0.5 );
    triAbscissa_.back()[ 2 ] = RVector3( 0.0, 0.5 );
    triWeights_.push_back( RVector( 3, 1.0/3.0 ) );

    //** 3.Order, n=4, Error: O(h4)
    triAbscissa_.push_back( std::vector< RVector3 >( 4 ) );
    triAbscissa_.back()[ 0 ] = RVector3( 1.0/3.0, 1.0/3.0 ) ;
    triAbscissa_.back()[ 1 ] = RVector3( 0.2, 0.2 );
    triAbscissa_.back()[ 2 ] = RVector3( 0.6, 0.2 );
    triAbscissa_.back()[ 3 ] = RVector3( 0.2, 0.6 );
    triWeights_.push_back( RVector( 4, 25.0 / 48.0 ) );
    triWeights_.back()[ 0 ] = -27.0/48.0;

    //** 4.Order, n=6, Error: O(h5)
    //**    Joseph E. Flaherty -- Finite Element Analysis CSCI-6860 / MATH-6860
    triAbscissa_.push_back( std::vector< RVector3 >( 6 ) );
    double a = 0.816847572980459, b = 0.091576213509771;
    triAbscissa_.back()[ 0 ] = RVector3( b, b );
    triAbscissa_.back()[ 1 ] = RVector3( a, b );
    triAbscissa_.back()[ 2 ] = RVector3( b, a );
    a = 0.108103018168070, b = 0.445948490915965;
    triAbscissa_.back()[ 3 ] = RVector3( b, b );
    triAbscissa_.back()[ 4 ] = RVector3( a, b );
    triAbscissa_.back()[ 5 ] = RVector3( b, a );
    triWeights_.push_back( RVector( 6, 0.109951743655322 ) );
    triWeights_.back()[ 3 ] = triWeights_.back()[ 4 ] = triWeights_.back()[ 5 ] = 0.223381589678011;

    //** 5.Order, n=7, Error: O(h6)
    //** pls check this
    triAbscissa_.push_back( std::vector< RVector3 >( 7 ) );
    double sqrt15 = std::sqrt( 15.0 );
    triAbscissa_.back()[ 0 ] = RVector3( 1.0/3.0, 1.0/3.0 );
    b = 2.0 / 7.0 + sqrt15 / 21.0, a = 1.0 - 2.0 * b;
    triAbscissa_.back()[ 1 ] = RVector3( b, b );
    triAbscissa_.back()[ 2 ] = RVector3( a, b );
    triAbscissa_.back()[ 3 ] = RVector3( b, a );
    b = 2.0 / 7.0 - sqrt15 / 21.0, a = 1.0 - 2.0 * b;
    triAbscissa_.back()[ 4 ] = RVector3( b, b );
    triAbscissa_.back()[ 5 ] = RVector3( a, b );
    triAbscissa_.back()[ 6 ] = RVector3( b, a );

    triWeights_.push_back( RVector( 7, 270.0 ) );
    triWeights_.back()[ 1 ] = triWeights_.back()[ 2 ] = triWeights_.back()[ 3 ] = 155.0 + sqrt15;
    triWeights_.back()[ 4 ] = triWeights_.back()[ 5 ] = triWeights_.back()[ 6 ] = 155.0 - sqrt15;
    triWeights_.back() /= 1200.0;

   
}

void IntegrationRules::initTet_(){
    //**    Joseph E. Flaherty -- Finite Element Analysis CSCI-6860 / MATH-6860
    //** 0.Order, n=1, Error: O(h0) -- just placeholder
    tetAbscissa_.push_back( std::vector< RVector3 >( 0 ) );
    tetWeights_.push_back( RVector( 0, 0.0 ) );

    //** 1.Order, n=1, Error: O(h2)
    tetAbscissa_.push_back( std::vector< RVector3 >( 1 ) );
    tetAbscissa_.back()[ 0 ] = RVector3( 0.25, 0.25, 0.25 );
    tetWeights_.push_back( RVector( 1, 1.0 ) );

    //** 2.Order, n=4, Error: O(h3)
    tetAbscissa_.push_back( std::vector< RVector3 >( 4 ) );
    double a = 0.585410196624969, b = 0.138196601125011;
    tetAbscissa_.back()[ 0 ] = RVector3( b, b, b );
    tetAbscissa_.back()[ 1 ] = RVector3( a, b, b );
    tetAbscissa_.back()[ 2 ] = RVector3( b, a, b );
    tetAbscissa_.back()[ 3 ] = RVector3( b, b, a );
    tetWeights_.push_back( RVector( 4, 0.25 ) );

    //** 3.Order, n=5, Error: O(h4)
    tetAbscissa_.push_back( std::vector< RVector3 >( 5 ) );
    tetAbscissa_.back()[ 0 ] = RVector3( 0.25, 0.25, 0.25 );
    a = 0.5, b = 1.0 / 6.0;
    tetAbscissa_.back()[ 1 ] = RVector3( b, b, b );
    tetAbscissa_.back()[ 2 ] = RVector3( a, b, b );
    tetAbscissa_.back()[ 3 ] = RVector3( b, a, b );
    tetAbscissa_.back()[ 4 ] = RVector3( b, b, a );
    tetWeights_.push_back( RVector( 5, 9.0 / 20.0 ) );
    tetWeights_.back( )[ 0 ] = -4.0 / 5.0;

    //** 4.Order, n=11, Error: O(h?)
    tetAbscissa_.push_back( std::vector< RVector3 >( 11 ) );
    tetAbscissa_.back()[ 0 ] = RVector3( 0.25, 0.25, 0.25 );
    a = 0.785714285714286; b = 0.071428571428571;
    tetAbscissa_.back()[ 1 ] = RVector3( b, b, b );
    tetAbscissa_.back()[ 2 ] = RVector3( a, b, b );
    tetAbscissa_.back()[ 3 ] = RVector3( b, a, b );
    tetAbscissa_.back()[ 4 ] = RVector3( b, b, a );
    a = 0.399403576166799; b = 0.100596423833201;
    tetAbscissa_.back()[ 5 ] = RVector3( a, b, b );
    tetAbscissa_.back()[ 6 ] = RVector3( b, b, a );
    tetAbscissa_.back()[ 7 ] = RVector3( b, a, a );
    tetAbscissa_.back()[ 8 ] = RVector3( a, a, b );
    tetAbscissa_.back()[ 9 ] = RVector3( b, a, b );
    tetAbscissa_.back()[10 ] = RVector3( a, b, a );

    tetWeights_.push_back( RVector( 11, -0.01315555555555555555555556 ) );
    for ( uint i = 1; i < 5; i ++ ) tetWeights_.back( )[ i ] = 0.007622222222222222222;
    for ( uint i = 5; i < 11; i ++ ) tetWeights_.back( )[ i ] = 0.02488888888888888888888;
    tetWeights_.back() *= 6.0;

    //** 5.Order, n=15, Error: O(h?)
    tetAbscissa_.push_back( std::vector< RVector3 >( 15 ) );
    tetAbscissa_.back()[ 0 ] = RVector3( 0.25, 0.25, 0.25 );
    a = 0.0; b = 1.0 / 3.0;
    tetAbscissa_.back()[ 1 ] = RVector3( b, b, b );
    tetAbscissa_.back()[ 2 ] = RVector3( a, b, b );
    tetAbscissa_.back()[ 3 ] = RVector3( b, a, b );
    tetAbscissa_.back()[ 4 ] = RVector3( b, b, a );
    a = 0.7272727272727272727; b = 0.090909090909090909;
    tetAbscissa_.back()[ 5 ] = RVector3( b, b, b );
    tetAbscissa_.back()[ 6 ] = RVector3( a, b, b );
    tetAbscissa_.back()[ 7 ] = RVector3( b, a, b );
    tetAbscissa_.back()[ 8 ] = RVector3( b, b, a );
    a = 0.066550153573664; b = 0.433449846426336;
    tetAbscissa_.back()[ 9 ] = RVector3( a, b, b );
    tetAbscissa_.back()[10 ] = RVector3( b, b, a );
    tetAbscissa_.back()[11 ] = RVector3( b, a, a );
    tetAbscissa_.back()[12 ] = RVector3( a, a, b );
    tetAbscissa_.back()[13 ] = RVector3( b, a, b );
    tetAbscissa_.back()[14 ] = RVector3( a, b, a );

    tetWeights_.push_back( RVector( 15, 0.030283678097089 ) );
    for ( uint i = 1; i < 5; i ++ ) tetWeights_.back( )[ i ] = 0.006026785714286;
    for ( uint i = 5; i < 9; i ++ ) tetWeights_.back( )[ i ] = 0.011645249086029;
    for ( uint i = 9; i < 15; i ++ ) tetWeights_.back( )[ i ] = 0.010949141561386;
    tetWeights_.back() *= 6.0;
}

void IntegrationRules::initQua_(){

    quaAbscissa_.push_back( std::vector < RVector3 > ( 0 ) );
    quaWeights_.push_back( RVector( 0, 0.0 ) );

    for ( uint order = 1; order < gauAbscissa_.size(); order ++ ){
        uint nK = gauAbscissa_[ order ].size();
        quaAbscissa_.push_back( std::vector < RVector3 > ( nK * nK ) );
        quaWeights_.push_back( RVector( gauAbscissa_[ order ].size() * gauAbscissa_[ order ].size() ) );

        for ( uint i = 0; i < nK; i ++ ){
            for ( uint j = 0; j < nK; j ++ ){
                uint k = i * nK + j;
                quaAbscissa_[ order ][ k ] = RVector3( edgAbscissa_[ order ][ i ][ 0 ], edgAbscissa_[ order ][ j ][ 0 ] );
                quaWeights_[ order ][ k ] = edgWeights_[ order ][ i ] * edgWeights_[ order ][ j ];
            }
        }
    }
}

void IntegrationRules::initHex_(){
}

} // namespace GIMLI
