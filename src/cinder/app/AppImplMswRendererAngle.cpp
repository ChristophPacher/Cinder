/*
 Copyright (c) 2014, The Cinder Project, All rights reserved.

 This code is intended for use with the Cinder C++ library: http://libcinder.org

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

#if defined( CINDER_GL_ANGLE )

#include "cinder/app/AppImplMswRendererAngle.h"
#include "cinder/app/RendererGl.h"

#include "cinder/app/App.h"
#include "cinder/gl/Environment.h"
#include <signal.h>

#define DEBUG_GL 1

#if ! defined( CI_SCREENSAVER )
#define CI_BREAK() { __debugbreak(); }
//#define CI_BREAK() { __asm int 3; }
#else
#define CI_BREAK() 
#endif

namespace cinder { namespace app {

void checkGlStatus();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AppImplMswRendererAngle
AppImplMswRendererAngle::AppImplMswRendererAngle( App *app, RendererGl *renderer )
	: AppImplMswRenderer( app ), mRenderer( renderer )
{
	
}

bool AppImplMswRendererAngle::initialize( HWND wnd, HDC dc, RendererRef sharedRenderer )
{
	mWnd = wnd;
	
	std::vector<EGLint> configAttribs;
	configAttribs.push_back( EGL_RED_SIZE ); configAttribs.push_back( 8 );
	configAttribs.push_back( EGL_GREEN_SIZE ); configAttribs.push_back( 8 );
	configAttribs.push_back( EGL_BLUE_SIZE ); configAttribs.push_back( 8 );
	configAttribs.push_back( EGL_ALPHA_SIZE ); configAttribs.push_back( EGL_DONT_CARE );
	configAttribs.push_back( EGL_DEPTH_SIZE ); configAttribs.push_back( mRenderer->getOptions().getDepthBufferDepth() );
	configAttribs.push_back( EGL_STENCIL_SIZE ); configAttribs.push_back( mRenderer->getOptions().getStencil() ? 8 : EGL_DONT_CARE );
// multisampling doesn't currently appear to work
//	configAttribs.push_back( EGL_SAMPLE_BUFFERS ); configAttribs.push_back( 1 );
	configAttribs.push_back( EGL_NONE );

	EGLint surfaceAttribList[] =
	{
		EGL_NONE, EGL_NONE
	};

	PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT = reinterpret_cast<PFNEGLGETPLATFORMDISPLAYEXTPROC>( eglGetProcAddress("eglGetPlatformDisplayEXT") );
	if( ! eglGetPlatformDisplayEXT )
		return false;

	const EGLint displayAttributes[] = { EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE, EGL_NONE };

	mDisplay = eglGetPlatformDisplayEXT( EGL_PLATFORM_ANGLE_ANGLE, dc, displayAttributes );
	if( mDisplay == EGL_NO_DISPLAY)
		return false;

	EGLint majorVersion, minorVersion;
	if( ! eglInitialize( mDisplay, &majorVersion, &minorVersion ) )
		return false;

	eglBindAPI( EGL_OPENGL_ES_API );
	if( eglGetError() != EGL_SUCCESS )
		return false;

	EGLint configCount;
	EGLConfig config;
	if( ! eglChooseConfig( mDisplay, configAttribs.data(), &config, 1, &configCount ) || (configCount != 1) )
		return false;

	mSurface = eglCreateWindowSurface( mDisplay, config, wnd, NULL );
 
	auto err = eglGetError();
	if( err != EGL_SUCCESS )
		return false;

    EGLint contextAttibutes[] = {
#if defined( CINDER_GL_ES_3 )
		EGL_CONTEXT_CLIENT_VERSION, 3,
#else
		EGL_CONTEXT_CLIENT_VERSION, 2,
#endif
		EGL_NONE
    };

    mContext = eglCreateContext( mDisplay, config, NULL, contextAttibutes );
    if( eglGetError() != EGL_SUCCESS )
		return false;
	checkGlStatus();

	eglMakeCurrent( mDisplay, mSurface, mSurface, mContext );
	if( eglGetError() != EGL_SUCCESS )
		return false;
	
	checkGlStatus();

	gl::Environment::setEs();
	checkGlStatus();

	std::shared_ptr<gl::PlatformDataAngle> platformData( new gl::PlatformDataAngle( mContext, mDisplay, mSurface, config ) );
	platformData->mObjectTracking = mRenderer->getOptions().getObjectTracking();

	mCinderContext = gl::Context::createFromExisting( platformData );
	checkGlStatus();

	mCinderContext->makeCurrent();

	checkGlStatus();

	eglSwapInterval( mDisplay, 0 );
	checkGlStatus();

	return true;
}

void AppImplMswRendererAngle::kill()
{
}

void AppImplMswRendererAngle::prepareToggleFullScreen()
{
//	mImpl->prepareToggleFullScreen();
}

void AppImplMswRendererAngle::finishToggleFullScreen()
{
//	mImpl->finishToggleFullScreen();
}

void AppImplMswRendererAngle::defaultResize() const
{
	checkGlStatus();

	::RECT clientRect;
	::GetClientRect( mWnd, &clientRect );
	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;

	gl::viewport( 0, 0, width, height );
	gl::setMatricesWindow( width, height );
}

void AppImplMswRendererAngle::swapBuffers() const
{
	auto status = ::eglMakeCurrent( mDisplay, mSurface, mSurface, mContext );
	assert( status );
	EGLBoolean result = ::eglSwapBuffers( mDisplay, mSurface );
	assert( result );
}

void AppImplMswRendererAngle::makeCurrentContext()
{
	mCinderContext->makeCurrent();
}

EGLint getEglError()
{
	return eglGetError();
}

std::string getEglErrorString( EGLint err )
{
	switch( err ) {
		case EGL_SUCCESS: return "EGL_SUCCESS";
		case EGL_NOT_INITIALIZED: return "EGL_NOT_INITIALIZED";
		case EGL_BAD_ACCESS: return "EGL_BAD_ACCESS";
		case EGL_BAD_ALLOC: return "EGL_BAD_ALLOC";
		case EGL_BAD_ATTRIBUTE: return "EGL_BAD_ATTRIBUTE";
		case EGL_BAD_CONTEXT: return "EGL_BAD_CONTEXT";
		case EGL_BAD_CONFIG: return "EGL_BAD_CONFIG";
		case EGL_BAD_CURRENT_SURFACE: return "EGL_BAD_CURRENT_SURFACE";
		case EGL_BAD_DISPLAY: return "EGL_BAD_DISPLAY";
		case EGL_BAD_SURFACE: return "EGL_BAD_SURFACE";
		case EGL_BAD_MATCH: return "EGL_BAD_MATCH";
		case EGL_BAD_PARAMETER: return "EGL_BAD_PARAMETER";
		case EGL_BAD_NATIVE_PIXMAP: return "EGL_BAD_NATIVE_PIXMAP";
		case EGL_BAD_NATIVE_WINDOW: return "EGL_BAD_NATIVE_WINDOW";
		case EGL_CONTEXT_LOST: return "EGL_CONTEXT_LOST";
		default: return "unknown error code";
	}
}

void checkGlStatus()
{
#if DEBUG_GL
	EGLint lastEglError = getEglError();
	if( lastEglError != EGL_SUCCESS ) {
		ci::app::console() << "EGL ERROR: " << getEglErrorString( lastEglError ) << std::endl;
		CI_BREAK();
	}

	GLenum lastGlError = ci::gl::getError();
	if( lastGlError != GL_NO_ERROR ) {
		ci::app::console() << "GL ERROR: " << ci::gl::getErrorString( lastGlError ) << std::endl;
	}
#endif // DEBUG_GL
}

} } // namespace cinder::app


#endif // defined( CINDER_GL_ANGLE )
