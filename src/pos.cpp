/***************************************************************************
 *   Copyright (C) 2006-2016 by the resistivity.net development team       *
 *   Carsten R�cker carsten@resistivity.net                                *
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

#include "pos.h"
#include "matrix.h"
//#include "vectortemplates.h"

namespace GIMLI{

std::vector < RVector3 > loadRVector3(const std::string & fileName){
    std::vector < RVector3 > l;
    std::fstream file; openInFile(fileName, & file, true);
    std::vector < std::string > row;

    while (! file.eof()){
        row = getNonEmptyRow(file);
        switch (row.size()){
            case 1 : l.push_back(RVector3(toDouble(row[0]), 0.0, 0.0)); break;
            case 2 : l.push_back(RVector3(toDouble(row[0]), toDouble(row[1]), 0.0)); break;
            case 3 : l.push_back(RVector3(toDouble(row[0]), toDouble(row[1]),
                                    toDouble(row[2]))); break;
        }
    }
    file.close();
    return l;
}

void saveRVector3(const std::vector < RVector3 > l, const std::string & fileName){
    std::fstream file; openOutFile(fileName, & file);
    for (uint i = 0; i < l.size(); i ++) file << l[i] << std::endl;
    file.close();
}

RVector3 center(const R3Vector & vPos){
  RVector3 pos(0.0, 0.0, 0.0);

  //  std::cout << WHERE_AM_I << vPos.size() << std::endl;

  if (vPos.size() == 0) {
    pos.setValid(false);
    return pos;
  }

  for (uint i = 0; i < vPos.size(); i ++) pos += vPos[i];
  return pos /= vPos.size();
}

double jacobianDetXY(const RVector3 & p1, const RVector3 & p2, const RVector3 & p3){
  double x1 = p1.x(), x2 = p2.x(), x3 = p3.x();
  double y1 = p1.y(), y2 = p2.y(), y3 = p3.y();

  return (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
}

double angle(const RVector3 & p1, const RVector3 & p2, const RVector3 & p3){
  double angle = p2.angle(p1, p3);
  if (jacobianDetXY(p1, p2, p3) > 0) angle = 2.0 * PI - angle;
  return angle;
}

bool xVari(const R3Vector & electrodeList){
    if (electrodeList.empty()) return false;
    double start = electrodeList[0].x();
    for (size_t i = 1; i < electrodeList.size(); i ++){
        if (std::fabs(electrodeList[i].x() - start) > TOLERANCE) return true;
    }
    return false;
}
bool yVari(const R3Vector & electrodeList){
    if (electrodeList.empty()) return false;
    double start = electrodeList[0].y();
    for (size_t i = 1; i < electrodeList.size(); i ++){
        if (std::fabs(electrodeList[i].y() - start) > TOLERANCE) return true;
    }
    return false;
}
bool zVari(const R3Vector & electrodeList){
    if (electrodeList.empty()) return false;
    double start = electrodeList[0].z();
    for (size_t i = 1; i < electrodeList.size(); i ++){
        if (std::fabs(electrodeList[i].z() - start) > TOLERANCE) return true;
    }
    return false;
}

RVector x(const R3Vector & rv){
    RVector t(rv.size());
    //std::copy(rv.begin(), rv.end(), &t[0], std::mem_fun_ref(&Pos< double >::x));
    for (uint i = 0, imax = rv.size(); i < imax; i ++) t[i] = rv[i].x();
    return t;
}

RVector y(const R3Vector & rv){
    RVector t(rv.size());
    for (uint i = 0, imax = rv.size(); i < imax; i ++) t[i] = rv[i].y();
    return t;
}

RVector z(const R3Vector & rv){
    RVector t(rv.size());
    for (uint i = 0, imax = rv.size(); i < imax; i ++) t[i] = rv[i].z();
    return t;
}
template < class ValueType > void swap(ValueType & v1, ValueType & v2){ ValueType tmp = v1; v1 = v2; v2 = tmp; }

void swapXY(R3Vector & rv){ for (uint i = 0, imax = rv.size(); i < imax; i ++) swap(rv[i][0], rv[i][1]); }
void swapXZ(R3Vector & rv){ for (uint i = 0, imax = rv.size(); i < imax; i ++) swap(rv[i][0], rv[i][2]); }
void swapYZ(R3Vector & rv){ for (uint i = 0, imax = rv.size(); i < imax; i ++) swap(rv[i][1], rv[i][2]); }

RVector toArray(const R3Vector & vec){
    RVector ret(vec.size() * 3);
    for (Index i = 0; i < vec.size(); i ++) {
        std::copy(&vec[i][0], &vec[i][2], &ret[i*3]);
    }
    return ret;
}

RMatrix toMatrix(const R3Vector & vec){
    RMatrix ret(vec.size(), 3);
    for (Index i = 0; i < vec.size(); i ++) {
        std::copy(&vec[i][0], &vec[i][2], &ret[i][0]);
    }
    return ret;
}

R3Vector stdVectorRVector3ToR3Vector(const std::vector < RVector3 > & rv){
    R3Vector ret(rv.size());
    for (Index i = 0; i < rv.size(); i ++) ret[i] = rv[i];
    return ret;
}

std::vector < RVector3 > R3VectorTostdVectorRVector3(const R3Vector & rv){
    std::vector < RVector3 > ret(rv.size());
    for (Index i = 0; i < rv.size(); i ++) ret[i] = rv[i];
    return ret;
}

template <> RVector3 RVector3::cross(const RVector3 & p) const{
//     r[0] = (a2 * b3 - a3 * b2);
//     r[1] = (a3 * b1 - a1 * b3);
//     r[2] = (a1 * b2 - a2 * b1);
  return (RVector3((*this)[1] * p[2] - (*this)[2] * p[1],
                   (*this)[2] * p[0] - (*this)[0] * p[2],
                   (*this)[0] * p[1] - (*this)[1] * p[0]));
}

template <> RVector3 RVector3::norm(const RVector3 & p1, const RVector3 & p2) const {
    RVector3 a(p1 - (*this));
    RVector3 b(p2 - (*this));
    RVector3 r(a.cross(b));
    return r.norm();
}

template <> RVector3 RVector3::normXY(const RVector3 & p) const {
    RVector3 result((*this + p) / 2.0);
    result.setZ(1.0);
    result = result.norm(*this, p);
    result.setZ(0.0);
    return result;
}

// template <> double RVector3::angle(const RVector3 & p) const {
//   double result = acos(this->dot(p) / (this->abs() * p.abs()));
//   if (std::isnan(result) || std::isinf(result)) {
//     result = 0.0;
//   }
//   return result;
// }

// template <> double RVector3::angle(const RVector3 & p1, const RVector3 & p3) const {
//   RVector3 a(p1 - (*this));
//   RVector3 b(p3 - (*this));
//   return (a).angle(b);
// }

} // namespace GIMLI
