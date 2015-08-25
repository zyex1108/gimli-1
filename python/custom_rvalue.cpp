#include "boost/python/object.hpp"  //len function
#include "boost/python/ssize_t.hpp" //ssize_t type definition
#include "boost/python/detail/none.hpp"
#include <boost/mpl/int.hpp>
#include <boost/mpl/next.hpp>
#include "tuples.hpp"

// #define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include <Python.h>

#include "gimli.h"
#include "pos.h"
#include "vector.h"

namespace bpl = boost::python;

// #define __DC(str) ;

#define __DC(str) if (GIMLI::debug()) __MS(str)

// ** TODO check if we need a delete somewhere for all the new stuff

namespace r_values_impl{
 
template < class ValueType > void * checkConvertibleSequenz(PyObject * obj){
    //     import_array2("Cannot import numpy c-api from pygimli hand_make_wrapper2", NULL);
    // is obj is a sequence
    if(!PySequence_Check(obj)){
        __DC(obj << "!Object")
        return NULL;
    }

    // has the obj a len method
    if(!PyObject_HasAttrString(obj, "__len__")){
        __DC(obj << "!len")
        return NULL;
    }

    bpl::object py_sequence(bpl::handle<>(bpl::borrowed(obj)));
    //         std::cout << "here am i 1 " << len(py_sequence) << std::endl;

    if (len(py_sequence) > 0) {
        
        bpl::object element = py_sequence[0];
        bpl::extract< ValueType > type_checker(element);
        
        if(type_checker.check()){
            __DC(obj << "->construct: " << len(py_sequence))
            return obj;
        } else {
            std::cout << WHERE_AM_I << "element cannot converted " << std::endl;
        }
        
    } else {
        __DC(obj << " len == 0")
        return NULL;
    }
    // check if there is a valid converter
    //         if(convertible_impl(py_sequence, boost::mpl::int_< 0 >(), length_type())){
    //             return obj;
    //         } else{
    __DC(obj << " fail")
    return NULL;
}

struct PyTuple2RVector3{

    typedef boost::tuples::tuple< double, double > xy_type;
    typedef boost::tuples::tuple< double, double, double > xyz_type;
    typedef bpl::from_py_sequence< xy_type > xy_converter_type;
    typedef bpl::from_py_sequence< xyz_type > xyz_converter_type;
    
    typedef GIMLI::RVector3 xyz_t;

    static void * convertible(PyObject * obj){
        if (xy_converter_type::convertible(obj) || 
             xyz_converter_type::convertible(obj)){
            return obj;
        } else{
            return NULL;
        }
    }

    static void construct(PyObject* obj, bpl::converter::rvalue_from_python_stage1_data * data){

        typedef bpl::converter::rvalue_from_python_storage< xyz_t > xyz_storage_t;
        
        xyz_storage_t * the_storage = reinterpret_cast< xyz_storage_t * >(data);
        void * memory_chunk = the_storage->storage.bytes;

        double x(0.0), y(0.0), z(0.0);
        
        bpl::tuple py_tuple(bpl::handle<>(bpl::borrowed(obj)));
        
        if (3 == bpl::len(py_tuple)){
            boost::tuples::tie(x, y, z) = xyz_converter_type::to_c_tuple(obj);
        } else if (2 == bpl::len(py_tuple)){
            boost::tuples::tie(x, y) = xy_converter_type::to_c_tuple(obj);
        }        
                
        //** don't know where this will be deleted but it is necessary
        new (memory_chunk) xyz_t(x, y, z);
        data->convertible = memory_chunk;
    }
};

struct PySequence2RVector{

    /*! Check if the object is convertible */
    static void * convertible(PyObject * obj){
        __DC(obj << " -> RVector")
        return checkConvertibleSequenz<double>(obj);
    }

    /*! Convert List[] or ndarray into RVector */
    static void construct(PyObject* obj, bpl::converter::rvalue_from_python_stage1_data * data){
       __DC(obj << " construct ..")
       
        typedef bpl::converter::rvalue_from_python_storage< GIMLI::Vector< double > > storage_t;
        storage_t* the_storage = reinterpret_cast<storage_t*>(data);
        void* memory_chunk = the_storage->storage.bytes;
        
        bpl::object py_sequence(bpl::handle<>(bpl::borrowed(obj)));
        GIMLI::Vector< double > * vec = new (memory_chunk) GIMLI::Vector< double >(len(py_sequence));
        data->convertible = memory_chunk;
        
       __DC("isvector<>: " << PyObject_HasAttrString(obj, "singleCalcCount"))
       __DC("isndarray<>: " << PyObject_HasAttrString(obj, "flatten"))
       __DC("len :   " << len(py_sequence))
       __DC("PyList: " << PyList_Check(obj))
       __DC("PyTup:  " << PyTuple_Check(obj))
       __DC("OneS:   " << PyArray_ISONESEGMENT(obj))
        
        if (PyObject_HasAttrString(obj, "flatten")){ // probably numpy ndarray
            if (PyArray_TYPE(obj) == 12 && PyArray_ISONESEGMENT(obj)){
                // convert from numpy array
                __DC(obj << " ** from array")
//                 GIMLI::Vector< double > * vec = new (memory_chunk) GIMLI::Vector< double >(PyArray_DIM(obj,0));
//                 data->convertible = memory_chunk;
                void * arrData = PyArray_DATA(obj);

                std::memcpy(&(*vec)[0], arrData, vec->size() * sizeof(double));
                return;
            }
            __DC("fixme: type=" << PyArray_TYPE(obj))
        }
            
        // convert from list
        __DC(obj << " ** from sequence ")
                        
        for (GIMLI::Index i = 0; i < vec->size(); i ++){
            (*vec)[i]= bpl::extract< double >(py_sequence[i]);
        }
    }
private:    
};

struct PySequence2IndexArray{

    /*! Check if the object is convertible */
    static void * convertible(PyObject * obj){
        __DC(obj << " -> IndexArray")
        return checkConvertibleSequenz<GIMLI::Index>(obj);
    }

    /*! Convert obj into IndexArray */
    static void construct(PyObject* obj, bpl::converter::rvalue_from_python_stage1_data * data){
        __DC(obj << " construct ..")
        bpl::object py_sequence(bpl::handle<>(bpl::borrowed(obj)));

        typedef bpl::converter::rvalue_from_python_storage< GIMLI::IndexArray > storage_t;
         
        storage_t* the_storage = reinterpret_cast<storage_t*>(data);
        void* memory_chunk = the_storage->storage.bytes;
 
        GIMLI::IndexArray * vec = new (memory_chunk) GIMLI::IndexArray(len(py_sequence));
        data->convertible = memory_chunk;
        __DC(obj << " from list")
        for (GIMLI::Index i = 0; i < vec->size(); i ++){
            (*vec)[i] = bpl::extract< GIMLI::Index >(py_sequence[i]);
        }
    }
private:    
};

struct PySequence2StdVectorRVector3{

    /*! Check if the object is convertible */
    static void * convertible(PyObject * obj){
         __DC(obj << " -> StdVectorRVector3")
        return checkConvertibleSequenz<GIMLI::Pos< double > >(obj);
    }

    /*! Convert obj into RVector */
    static void construct(PyObject* obj, bpl::converter::rvalue_from_python_stage1_data * data){
        
        bpl::object py_sequence(bpl::handle<>(bpl::borrowed(obj)));

        typedef bpl::converter::rvalue_from_python_storage< std::vector < GIMLI::Pos< double > > > storage_t;
         
        storage_t* the_storage = reinterpret_cast<storage_t*>(data);
        void* memory_chunk = the_storage->storage.bytes;
 
        std::vector < GIMLI::Pos < double > > *vec = new (memory_chunk) std::vector < GIMLI::Pos < double > >(len(py_sequence));
        data->convertible = memory_chunk;

        for (GIMLI::Index i = 0; i < vec->size(); i ++){
            (*vec)[i] = bpl::extract< GIMLI::Pos < double > >(py_sequence[i]);
        }
    }
private:    
};

struct PySequence2R3Vector{

    /*! Check if the object is convertible */
    static void * convertible(PyObject * obj){
        __DC(obj << " -> R3Vector")
        return checkConvertibleSequenz<GIMLI::Pos< double > >(obj);
    }

    /*! Convert obj into RVector */
    static void construct(PyObject* obj, bpl::converter::rvalue_from_python_stage1_data * data){
        
        bpl::object py_sequence(bpl::handle<>(bpl::borrowed(obj)));

        typedef bpl::converter::rvalue_from_python_storage< GIMLI::R3Vector > storage_t;
         
        storage_t* the_storage = reinterpret_cast<storage_t*>(data);
        void* memory_chunk = the_storage->storage.bytes;
 
        GIMLI::R3Vector *vec = new (memory_chunk) GIMLI::R3Vector(len(py_sequence));
        data->convertible = memory_chunk;

        for (GIMLI::Index i = 0; i < vec->size(); i ++){
            (*vec)[i] = bpl::extract< GIMLI::Pos < double > >(py_sequence[i]);
        }
    }
private:    
};
} //r_values_impl

void register_pysequence_to_indexvector_conversion(){
    bpl::converter::registry::push_back(& r_values_impl::PySequence2IndexArray::convertible, 
                                        & r_values_impl::PySequence2IndexArray::construct, 
                                        bpl::type_id< GIMLI::IndexArray >());
}
void register_pysequence_to_rvector_conversion(){
    bpl::converter::registry::push_back(& r_values_impl::PySequence2RVector::convertible, 
                                        & r_values_impl::PySequence2RVector::construct, 
                                        bpl::type_id< GIMLI::Vector< double > >());
}
// void register_pysequence_to_bvector_conversion(){
//     bpl::converter::registry::push_back(& r_values_impl::PySequence2BVector::convertible, 
//                                         & r_values_impl::PySequence2BVector::construct, 
//                                         bpl::type_id< GIMLI::Vector< bool > >());
// }
void register_pysequence_to_StdVectorRVector3_conversion(){
    bpl::converter::registry::push_back(& r_values_impl::PySequence2StdVectorRVector3::convertible, 
                                        & r_values_impl::PySequence2StdVectorRVector3::construct, 
                                        bpl::type_id< std::vector< GIMLI::Pos< double > > >());
}
void register_pysequence_to_r3vector_conversion(){
    bpl::converter::registry::push_back(& r_values_impl::PySequence2R3Vector::convertible, 
                                        & r_values_impl::PySequence2R3Vector::construct, 
                                        bpl::type_id< GIMLI::R3Vector >());
}
void register_pytuple_to_rvector3_conversion(){
    bpl::converter::registry::push_back(& r_values_impl::PyTuple2RVector3::convertible, 
                                        & r_values_impl::PyTuple2RVector3::construct, 
                                        bpl::type_id< GIMLI::Pos< double > >());
}
