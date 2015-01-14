#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/Shader.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/VboMesh.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace ci::app;

class RotatingCubeApp : public AppNative {
  public:	
	void	setup() override;
	void	resize() override;
	void	update() override;
	void	draw() override;
	
	CameraPersp			mCam;
	gl::BatchRef		mBatch;
	gl::TextureRef		mTexture;
	gl::GlslProgRef		mGlsl;
	mat4				mCubeRotation;
};

void RotatingCubeApp::setup()
{
	mCam.lookAt( vec3( 3, 2, 4 ), vec3( 0 ) );
	
	mTexture = gl::Texture::create( loadImage( loadAsset( "texture.jpg" ) ), gl::Texture::Format().mipmap() );
	mTexture->bind();

#if defined( CINDER_GL_ES )
	mGlsl = gl::GlslProg::create( loadAsset( "shader_es2.vert" ), loadAsset( "shader_es2.frag" ) );
#else
	mGlsl = gl::GlslProg::create( loadAsset( "shader.vert" ), loadAsset( "shader.frag" ) );
#endif
	mBatch = gl::Batch::create( geom::Cube(), mGlsl );

	gl::enableDepthWrite();
	gl::enableDepthRead();
}

void RotatingCubeApp::resize()
{
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	gl::setMatrices( mCam );
}

void RotatingCubeApp::update()
{
	// Rotate the cube by 0.2 degrees around the y-axis
	mCubeRotation *= rotate( toRadians( 0.2f ), normalize( vec3( 0, 1, 0 ) ) );
}

void RotatingCubeApp::draw()
{
	gl::clear();

	gl::setMatrices( mCam );

	gl::ScopedModelMatrix modelScope;
	gl::multModelMatrix( mCubeRotation );
	mBatch->draw();
}

CINDER_APP_NATIVE( RotatingCubeApp, RendererGl )