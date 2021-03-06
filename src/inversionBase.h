/***************************************************************************
 *   Copyright (C) 2006-2014 by the resistivity.net development team       *
 *   Carsten Rücker carsten@resistivity.net                                *
 *   Thomas Günther thomas@resistivity.net                                 *
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

#ifndef _GIMLI_INVERSIONBASE__H
#define _GIMLI_INVERSIONBASE__H

#include "gimli.h"

namespace GIMLI{

//! Inversion base template
/*! Inversion base template, provide inversion interface. */
template < class ModelValType > class InversionBase {
public:
    
    typedef Vector < ModelValType > ModelVector;
    
    /*! Default constructor*/
    InversionBase(){
    }

    /*! Default destructor */
    virtual ~InversionBase(){
    }

    /*! Abstract method for running the whole inversion. */
    virtual const ModelVector & run() = 0;

    virtual void setModel(const ModelVector & model) = 0;

    virtual void setReferenceModel(const ModelVector & model) = 0;

    /*! Abstract method for returning a const reference to the model vector. */
    virtual const ModelVector & model() const = 0;

    virtual void setData(const ModelVector & data)   = 0;

    virtual void setError(const ModelVector & err)   = 0;

    virtual void setTransData(Trans< ModelVector > & t)   = 0;

    virtual void setTransModel(Trans< ModelVector > & t)   = 0;

    virtual void setLambda(double l) = 0;

    virtual void setMaxIter(int maxiter) = 0;

    virtual ModellingBase * forwardOperator() = 0;

    virtual void setForwardOperator(ModellingBase & fop) = 0;

    virtual const ModelVector & cWeight() const = 0;

    virtual void abort() = 0;

    virtual uint iter() const = 0;

    virtual double chi2() const = 0;

    virtual bool isRunning() const = 0;


protected:

};

} // namespace GIMLI

#endif // _GIMLI_INVERSIONBASE__H
