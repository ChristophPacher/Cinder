/*
 Copyright (c) 2010, The Barbarian Group
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

#include "cinder/Vector.h"
#include "cinder/Camera.h"

namespace cinder {

class MayaCamUI {
 public:
	MayaCamUI() { mInitialCam = mCurrentCam = CameraPersp(); }
	MayaCamUI( const CameraPersp &aInitialCam ) { mInitialCam = mCurrentCam = aInitialCam; }
	
	void mouseDown( const ivec2 &mousePos )
	{
		mInitialMousePos = mousePos;
		mInitialCam = mCurrentCam;
		mLastAction = ACTION_NONE;
	}
	
	void mouseDrag( const ivec2 &mousePos, bool leftDown, bool middleDown, bool rightDown )
	{
		int action;
		if( rightDown || ( leftDown && middleDown ) )
			action = ACTION_ZOOM;
		else if( middleDown )
			action = ACTION_PAN;
		else if( leftDown )
			action = ACTION_TUMBLE;
		else
			return;
		
		if( action != mLastAction ) {
			mInitialCam = mCurrentCam;
			mInitialMousePos = mousePos;
		}
		
		mLastAction = action;
	
		if( action == ACTION_ZOOM ) { // zooming
			int mouseDelta = ( mousePos.x - mInitialMousePos.x ) + ( mousePos.y - mInitialMousePos.y );

			float newCOI = powf( 2.71828183f, -mouseDelta / 500.0f ) * mInitialCam.getCenterOfInterest();
			vec3 oldTarget = mInitialCam.getCenterOfInterestPoint();
			vec3 newEye = oldTarget - mInitialCam.getViewDirection() * newCOI;
			mCurrentCam.setEyePoint( newEye );
			mCurrentCam.setCenterOfInterest( newCOI );
		}
		else if( action == ACTION_PAN ) { // panning
			float deltaX = ( mousePos.x - mInitialMousePos.x ) / 1000.0f * mInitialCam.getCenterOfInterest();
			float deltaY = ( mousePos.y - mInitialMousePos.y ) / 1000.0f * mInitialCam.getCenterOfInterest();
			vec3 mW = normalize( mInitialCam.getViewDirection() );
			vec3 mU = normalize( cross( vec3( 0, 1, 0 ), mW ) );
			vec3 mV = normalize( cross( mW, mU ) );
			mCurrentCam.setEyePoint( mInitialCam.getEyePoint() + mU * deltaX + mV * deltaY );
		}
		else { // tumbling
			float deltaY = ( mousePos.y - mInitialMousePos.y ) / 100.0f;
			float deltaX = ( mousePos.x - mInitialMousePos.x ) / -100.0f;
			vec3 mW = normalize( mInitialCam.getViewDirection() );
			bool invertMotion = ( mInitialCam.getOrientation() * glm::vec3( 0, 1, 0 ) ).y < 0.0f;
			vec3 mU = normalize( cross( vec3( 0, 1, 0 ), mW ) );

			if( invertMotion ) {
				deltaX = -deltaX;
				deltaY = -deltaY;
			}

			glm::vec3 rotatedVec = glm::angleAxis( deltaY, mU ) * ( mInitialCam.getEyePoint() - mInitialCam.getCenterOfInterestPoint() );
			rotatedVec = glm::angleAxis( deltaX, glm::vec3( 0, 1, 0 ) ) * rotatedVec;

			mCurrentCam.setEyePoint( mInitialCam.getCenterOfInterestPoint() + rotatedVec );
			mCurrentCam.setOrientation( glm::angleAxis( deltaX, glm::vec3( 0, 1, 0 ) ) * glm::angleAxis( deltaY, mU ) * mInitialCam.getOrientation() );
		}
	}	
	
	const CameraPersp& getCamera() const { return mCurrentCam; }
	void setCurrentCam( const CameraPersp &aCurrentCam ) { mCurrentCam = aCurrentCam; }
	
 private:
	enum		{ ACTION_NONE, ACTION_ZOOM, ACTION_PAN, ACTION_TUMBLE };
 
	ivec2		mInitialMousePos;
	CameraPersp	mCurrentCam, mInitialCam;
	int			mLastAction;
};

}; // namespace cinder
