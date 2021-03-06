/*
 Copyright (c) 2010, The Cinder Project
 All rights reserved.
 
 This code is designed for use with the Cinder C++ library, http://libcinder.org 

 Portions Copyright (c) 2010, The Barbarian Group
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <cmath>
#include <cstring>
#include <iostream>
#include <cassert>
#include <limits>

#include "cinder/CinderMath.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/io.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/rotate_vector.hpp"

namespace cinder {

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::ivec2;
using glm::ivec3;
using glm::ivec4;
using glm::dvec2;
using glm::dvec3;
using glm::dvec4;

//! Returns a vector which is orthogonal to \a vec
template<typename T, glm::precision P>
glm::detail::tvec3<T, P> orthogonal( const glm::detail::tvec3<T, P> &vec )
{
	if( math<T>::abs( vec.y ) < (T)0.99 ) // abs(dot(u, Y)), somewhat arbitrary epsilon
		return glm::detail::tvec3<T, P>( -vec.z, 0, vec.x ); // cross( this, Y )
	else
		return glm::detail::tvec3<T, P>( 0, vec.z, -vec.y ); // cross( this, X )
}

template<typename T, glm::precision P>
glm::detail::tvec3<T, P> slerp( const glm::detail::tvec3<T, P> &a, const glm::detail::tvec3<T, P> &b, T t )
{
	T cosAlpha, alpha, sinAlpha;
	T t1, t2;
	glm::detail::tvec3<T, P> result;

	// get cosine of angle between vectors (-1 -> 1)
	cosAlpha = glm::dot( a, b );

	// get angle (0 -> pi)
	alpha = math<T>::acos( cosAlpha );

	// get sine of angle between vectors (0 -> 1)
	sinAlpha = math<T>::sin( alpha );

	// this breaks down when sinAlpha = 0, i.e. alpha = 0 or pi
	t1 = math<T>::sin( ((T)1 - t) * alpha) / sinAlpha;
	t2 = math<T>::sin( t * alpha ) / sinAlpha;

	// interpolate src vectors
	return a * t1 + b * t2;
}

template<uint8_t DIM,typename T> struct VECDIM { };

template<> struct VECDIM<2,float>	{ typedef vec2	TYPE; };
template<> struct VECDIM<3,float>	{ typedef vec3	TYPE; };
template<> struct VECDIM<4,float>	{ typedef vec4	TYPE; };
template<> struct VECDIM<2,double>	{ typedef dvec2	TYPE; };
template<> struct VECDIM<3,double>	{ typedef dvec3	TYPE; };
template<> struct VECDIM<4,double>	{ typedef dvec4	TYPE; };
template<> struct VECDIM<2,int>		{ typedef ivec2	TYPE; };
template<> struct VECDIM<3,int>		{ typedef ivec3	TYPE; };
template<> struct VECDIM<4,int>		{ typedef ivec4	TYPE; };

} // namespace cinder
