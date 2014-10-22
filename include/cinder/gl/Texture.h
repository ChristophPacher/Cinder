/*
 Copyright (c) 2010, The Cinder Project, All rights reserved.
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

#pragma once

#include "cinder/Cinder.h"
#include "cinder/gl/gl.h"
#include "cinder/Surface.h"
#include "cinder/Rect.h"
#include "cinder/Stream.h"
#include "cinder/Exception.h"
#include "cinder/DataSource.h"
#include "cinder/ImageIo.h"

#include <vector>
#include <utility>
#include <array>

#if defined( CINDER_GL_ES )
#define GL_BLUE		0x1905
#define GL_GREEN	0x1904
#define GL_RED		0x1903
#endif

namespace cinder { namespace gl {

typedef class Texture2d							Texture;
typedef std::shared_ptr<class TextureBase>		TextureBaseRef;
typedef std::shared_ptr<class Texture2d>		Texture2dRef;
typedef Texture2dRef							TextureRef;
typedef std::shared_ptr<Texture2d>				Texture2dRef;
typedef std::shared_ptr<class Texture3d>		Texture3dRef;
typedef std::shared_ptr<class TextureCubeMap>	TextureCubeMapRef;

// Forward-declared from cinder/gl/Pbo.h
class Pbo;
typedef std::shared_ptr<Pbo>					PboRef;

class TextureBase {
  public:
	virtual ~TextureBase();

	//! Determines whether the Texture will call glDeleteTextures() to free the associated texture objects on destruction
	void			setDoNotDispose( bool aDoNotDispose = true ) { mDoNotDispose = aDoNotDispose; }
	//! the Texture's internal format, which is the format that OpenGL stores the texture data in memory. Common values include \c GL_RGB, \c GL_RGBA and \c GL_LUMINANCE
	GLint			getInternalFormat() const;
	//! the ID number for the texture, appropriate to pass to calls like \c glBindTexture()
	GLuint			getId() const { return mTextureId; }
	//! the target associated with texture. Typical values are \c GL_TEXTURE_2D and \c GL_TEXTURE_RECTANGLE_ARB
	GLenum			getTarget() const { return mTarget; }
	//!	Binds the Texture's texture to its target in the multitexturing unit \c GL_TEXTURE0 + \a textureUnit
	void 			bind( uint8_t textureUnit = 0 ) const;
	//!	Unbinds the Texture currently bound in the Texture's target
	void			unbind( uint8_t textureUnit = 0 ) const;

	//! Sets the wrapping behavior when a texture coordinate falls outside the range of [0,1]. Possible values are \c GL_REPEAT, \c GL_CLAMP_TO_EDGE, etc. Default is \c GL_CLAMP_TO_EDGE.
	void			setWrap( GLenum wrapS, GLenum wrapT ) { setWrapS( wrapS ); setWrapT( wrapT ); }
	//! Sets the horizontal wrapping behavior when a texture coordinate falls outside the range of [0,1]. Possible values are \c GL_REPEAT and \c GL_CLAMP_TO_EDGE, etc. Default is \c GL_CLAMP_TO_EDGE.
	void			setWrapS( GLenum wrapS );
	//! Sets the vertical wrapping behavior when a texture coordinate falls outside the range of [0,1]. Possible values are \c GL_REPEAT, \c GL_CLAMP_TO_EDGE, etc. Default is \c GL_CLAMP_TO_EDGE.
	void			setWrapT( GLenum wrapT );
#if ! defined( CINDER_GL_ES )
	//! Sets the depth wrapping behavior when a texture coordinate falls outside the range of [0,1]. Possible values are \c GL_REPEAT, \c GL_CLAMP_TO_EDGE, etc. Default is \c GL_CLAMP_TO_EDGE.
	void			setWrapR( GLenum wrapR );
#endif	
	/** \brief Sets the filtering behavior when a texture is displayed at a lower resolution than its native resolution.
	 * Possible values are \li \c GL_NEAREST \li \c GL_LINEAR \li \c GL_NEAREST_MIPMAP_NEAREST \li \c GL_LINEAR_MIPMAP_NEAREST \li \c GL_NEAREST_MIPMAP_LINEAR \li \c GL_LINEAR_MIPMAP_LINEAR **/
	void			setMinFilter( GLenum minFilter );
	/** Sets the filtering behavior when a texture is displayed at a higher resolution than its native resolution.
	 * Possible values are \li \c GL_NEAREST \li \c GL_LINEAR **/
	void			setMagFilter( GLenum magFilter );
	//! Sets the anisotropic filtering amount. A value greater than 1.0 "enables" anisotropic filtering. Maximum of getMaxMaxAnisotropy();
	void			setMaxAnisotropy( GLfloat maxAnisotropy );
	//! Returns whether the Texture has Mipmapping enabled
	bool			hasMipmapping() const { return mMipmapping; }
	// Specifies the texture comparison mode for currently bound depth textures.
	void			setCompareMode( GLenum compareMode );
	// Specifies the comparison operator used when \c GL_TEXTURE_COMPARE_MODE is set to \c GL_COMPARE_R_TO_TEXTURE
	void			setCompareFunc( GLenum compareFunc );	
	//! Returns the appropriate parameter to glGetIntegerv() for a specific target; ie GL_TEXTURE_2D -> GL_TEXTURE_BINDING_2D. Returns 0 on failure.
	static GLenum	getBindingConstantForTarget( GLenum target );
	//! Returns whether a Surface of \a width, \a rowBytes and \a surfaceChannelOrder would require an intermediate Surface in order to be copied into a GL Texture.
	template<typename T>
	static bool		surfaceRequiresIntermediate( int32_t width, uint8_t pixelBytes, int32_t rowBytes, SurfaceChannelOrder surfaceChannelOrder );
	//! Converts a SurfaceChannelOrder into an appropriate OpenGL dataFormat and type
	template<typename T>
	static void		SurfaceChannelOrderToDataFormatAndType( const SurfaceChannelOrder &sco, GLint *dataFormat, GLenum *type );
	//! calculate the size of mipMap for the corresponding level
	static ivec2	calcMipLevelSize( int level, GLint width, GLint height );
	//! Returns the maximum anisotropic filtering maximum allowed by the hardware
	static GLfloat	getMaxMaxAnisotropy();

	//! Returns the Texture's swizzle mask (corresponding to \c GL_TEXTURE_SWIZZLE_RGBA)	
	std::array<GLint,4>	getSwizzleMask() const { return mSwizzleMask; }
	//! Returns whether this hardware supports texture swizzling (via \c GL_TEXTURE_SWIZZLE_RGBA)
	static bool		supportsHardwareSwizzle();
#if defined( CINDER_GL_ES )
	//! Returns whether this hardware supports shadow sampling.
	static bool		supportsShadowSampler();
#endif
	//! Returns the debugging label associated with the Texture.
	const std::string&	getLabel() const { return mLabel; }
	//! Sets the debugging label associated with the Texture. Calls glObjectLabel() when available.
	void				setLabel( const std::string &label );

	struct Format {			
		//! Specifies the texture's target. The default is \c GL_TEXTURE_2D
		void	setTarget( GLenum target ) { mTarget = target; }
		//! Sets the texture's target to be \c GL_TEXTURE_RECTANGLE. Not available in OpenGL ES.
#if ! defined( CINDER_GL_ES )
		void	setTargetRect() { mTarget = GL_TEXTURE_RECTANGLE; }
#endif
		
		//! Enables or disables mipmapping. Default is disabled.
		void	enableMipmapping( bool enableMipmapping = true ) { mMipmapping = enableMipmapping; mMipmappingSpecified = true; }
		//! Sets the base mipmap level. Default is \c 0. Ignored on ES 2.
		void	setBaseMipmapLevel( GLuint level ) { mBaseMipmapLevel = level; }
		//! Sets the max mipmap level. Default is \c 1000 (per OpenGL). Ignored on ES 2.
		void	setMaxMipmapLevel( GLuint level ) { mMaxMipmapLevel = level; }
			
		//! Sets the Texture's internal format. A value of -1 implies selecting the best format for the context.
		void	setInternalFormat( GLint internalFormat ) { mInternalFormat = internalFormat; }
		//! Sets the Texture's internal format to be automatically selected based on the context.
		void	setAutoInternalFormat() { mInternalFormat = -1; }
		//! Corresponds to the 'format' parameter of glTexImage*(). Defaults to match the internalFormat (a value of \c -1)
		void	setPixelDataFormat( GLenum pixelDataFormat ) { mPixelDataFormat = pixelDataFormat; }
		//! Corresponds to the 'type' parameter of glTexImage*(). Defaults to \c GL_UNSIGNED_BYTE	
		void	setPixelDataType( GLenum pixelDataType ) { mPixelDataType = pixelDataType; }
		// Specifies the texture comparison mode for currently bound depth textures.
		void	setCompareMode( GLenum compareMode ) { mCompareMode = compareMode; }
		// Specifies the comparison operator used when \c GL_TEXTURE_COMPARE_MODE is set to \c GL_COMPARE_R_TO_TEXTURE
		void	setCompareFunc( GLenum compareFunc ) { mCompareFunc = compareFunc; }		
		//! Sets the wrapping behavior when a texture coordinate falls outside the range of [0,1]. Possible values are \c GL_REPEAT, \c GL_CLAMP_TO_EDGE, etc. Default is \c GL_CLAMP_TO_EDGE.
		void	setWrap( GLenum wrapS, GLenum wrapT ) { setWrapS( wrapS ); setWrapT( wrapT ); }
		//! Sets the horizontal wrapping behavior when a texture coordinate falls outside the range of [0,1]. Possible values are \c GL_REPEAT, \c GL_CLAMP_TO_EDGE, etc. Default is \c GL_CLAMP_TO_EDGE.
		void	setWrapS( GLenum wrapS ) { mWrapS = wrapS; }
		//! Sets the vertical wrapping behavior when a texture coordinate falls outside the range of [0,1]. Possible values are \c GL_REPEAT, \c GL_CLAMP_TO_EDGE, etc. Default is \c GL_CLAMP_TO_EDGE.
		void	setWrapT( GLenum wrapT ) { mWrapT = wrapT; }
#if ! defined( CINDER_GL_ES )
		//! Sets the depth wrapping behavior when a texture coordinate falls outside the range of [0,1]. Possible values are \c GL_REPEAT, \c GL_CLAMP_TO_EDGE, etc. Default is \c GL_CLAMP_TO_EDGE.
		void	setWrapR( GLenum wrapR ) { mWrapR = wrapR; }
#endif
		//! Sets the filtering behavior when a texture is displayed at a lower resolution than its native resolution. Default is \c GL_LINEAR unless mipmapping is enabled, in which case \c GL_LINEAR_MIPMAP_LINEAR
		void	setMinFilter( GLenum minFilter ) { mMinFilter = minFilter; mMinFilterSpecified = true; }
		//! Sets the filtering behavior when a texture is displayed at a higher resolution than its native resolution. Default is \c GL_LINEAR.
		void	setMagFilter( GLenum magFilter ) { mMagFilter = magFilter; }
		//! Sets the anisotropic filter amount. A value greater than 1.0 "enables" anisotropic filtering. Maximum of getMaxMaxAnisotropy();
		void    setMaxAnisotropy( GLfloat maxAnisotropy ) { mMaxAnisotropy = maxAnisotropy; }
		
		//! Returns the texture's target
		GLenum	getTarget() const { return mTarget; }
		//! Returns whether the texture has mipmapping enabled
		bool	hasMipmapping() const { return mMipmapping; }
		
		//! Returns the Texture's internal format. A value of -1 implies automatic selection of the internal format based on the context.
		GLint	getInternalFormat() const { return mInternalFormat; }
		//! Returns whether the Texture's internal format will be automatically selected based on the context.
		bool	isAutoInternalFormat() const { return mInternalFormat == -1; }
		
		//! Returns the Texture's pixel format as passed to glTexImage*() calls. A value of -1 implies automatic selection of the pixel format based on the context.
		GLint	getPixelDataFormat() const { return mPixelDataFormat; }
		//! Returns the Texture's data type as passed to glTexImage*() calls.
		GLenum	getPixelDataType() const { return mPixelDataType; }
		
		//! Returns the horizontal wrapping behavior for the texture coordinates.
		GLenum	getWrapS() const { return mWrapS; }
		//! Returns the vertical wrapping behavior for the texture coordinates.
		GLenum	getWrapT() const { return mWrapT; }
		//! Returns the depth wrapping behavior for the texture coordinates.
#if ! defined( CINDER_GL_ES )
		GLenum	getWrapR() const { return mWrapR; }
#endif
		//! Returns the texture minifying function, which is used whenever the pixel being textured maps to an area greater than one texture element.
		GLenum	getMinFilter() const { return mMinFilter; }
		//! Returns the texture magnifying function, which is used whenever the pixel being textured maps to an area less than or equal to one texture element.
		GLenum	getMagFilter() const { return mMagFilter; }
		//! Returns the texture anisotropic filtering amount
		GLfloat getMaxAnisotropy() const { return mMaxAnisotropy; }

#if ! defined( CINDER_GL_ES )
		//! Supplies an intermediate PBO that Texture constructors optionally make use of. A PBO of an inadequate size may result in an exception.
		void			setIntermediatePbo( const PboRef &intermediatePbo ) { mIntermediatePbo = intermediatePbo; }
		//! Returns the optional intermediate PBO that Texture constructors may make use of.
		const PboRef&	getIntermediatePbo() const { return mIntermediatePbo; }
#endif
		//! Sets the texture's border color. Ignored in OpenGL ES.
		void			setBorderColor( const std::array<GLfloat, 4> &border ) { mBorderColor = border; mBorderSpecified = true; }
		//! Sets the texture's border color. Ignored in OpenGL ES.
		void			setBorderColor( const ColorA &color );

		//! Sets the swizzle mask corresponding to \c GL_TEXTURE_SWIZZLE_RGBA. Expects \c GL_RED through \c GL_ALPHA, or \c GL_ONE or \c GL_ZERO
		void	setSwizzleMask( const std::array<GLint,4> &swizzleMask ) { mSwizzleMask = swizzleMask; mSwizzleSpecified = true; }
		//! Sets the swizzle mask corresponding to \c GL_TEXTURE_SWIZZLE_RGBA. Expects \c GL_RED through \c GL_ALPHA, or \c GL_ONE or \c GL_ZERO
		void	setSwizzleMask( GLint r, GLint g, GLint b, GLint a );
		//! Returns the swizzle mask corresponding to \c GL_TEXTURE_SWIZZLE_RGBA.
		const std::array<GLint,4>&	getSwizzleMask() const { return mSwizzleMask; }
		
		//! Returns the debugging label associated with the Texture.
		const std::string&	getLabel() const { return mLabel; }
		//! Sets the debugging label associated with the Texture. Calls glObjectLabel() when available.
		void				setLabel( const std::string &label ) { mLabel = label; }
		//! Sets the debugging label associated with the Texture. Calls glObjectLabel() when available.
		Format&				label( const std::string &label ) { setLabel( label ); return *this; }
		
	protected:
		Format();
	
		GLenum				mTarget;
		GLenum				mWrapS, mWrapT, mWrapR;
		GLenum				mMinFilter, mMagFilter;
		GLint				mCompareMode, mCompareFunc;		
		bool				mMipmapping, mMipmappingSpecified;
		bool				mMinFilterSpecified;
		GLuint				mBaseMipmapLevel;
		GLuint				mMaxMipmapLevel;
		GLfloat				mMaxAnisotropy;
		GLint				mInternalFormat;
		GLint				mPixelDataFormat;
		GLenum				mPixelDataType;
		bool				mSwizzleSpecified;
		std::array<GLint,4>	mSwizzleMask;
		bool				mBorderSpecified;
		std::array<GLfloat,4>	mBorderColor;
		std::string			mLabel; // debug label

#if ! defined( CINDER_GL_ES )		
		PboRef				mIntermediatePbo;
#endif
		friend class TextureBase;
	};

  protected:
	TextureBase();
	TextureBase( GLenum target, GLuint textureId, GLint internalFormat );
	
	void			initParams( Format &format, GLint defaultInternalFormat );

	virtual void	printDims( std::ostream &os ) const = 0;
	
	GLenum				mTarget;
	GLuint				mTextureId;
	mutable GLint		mInternalFormat;
	bool				mMipmapping;
	bool				mDoNotDispose;
	std::array<GLint,4>	mSwizzleMask;
	std::string			mLabel; // debugging label
	
	friend std::ostream& operator<<( std::ostream &os, const TextureBase &rhs );
};

std::ostream& operator<<( std::ostream &os, const TextureBase &rhs );

class TextureData {
  public:
	struct Level {
  		GLsizei						dataSize;
		size_t						offset;
		std::shared_ptr<uint8_t>	dataStore;
		GLsizei						width, height, depth;
	};

	TextureData();
#if ! defined( CINDER_GL_ES )
	//! Binds the pbo if it's not nullptr
	TextureData( const PboRef &pbo );
#endif
	//! Unbinds the pbo if it is not nullptr
	~TextureData();

	GLint				getWidth() const { return mWidth; }
	void				setWidth( GLint width ) { mWidth = width; }
	GLint				getHeight() const { return mHeight; }
	void				setHeight( GLint height ) { mHeight = height; }
	GLint				getDepth() const { return mDepth; }
	void				setDepth( GLint depth ) { mDepth = depth; }
	
	bool				isCompressed() const { return mDataFormat == 0; }
	GLint				getInternalFormat() const { return mInternalFormat; }
	void				setInternalFormat( GLint internalFormat ) { mInternalFormat = internalFormat; }
	GLenum				getDataFormat() const { return mDataFormat; }
	void				setDataFormat( GLenum dataFormat ) { mDataFormat = dataFormat; }
	GLenum				getDataType() const { return mDataType; }
	void				setDataType( GLenum dataType ) { mDataType = dataType; }
	GLint				getUnpackAlignment() const { return mUnpackAlignment; }
	void				setUnpackAlignment( GLint unpackAlignment ) { mUnpackAlignment = unpackAlignment; }

	const std::array<GLint,4>&	getSwizzleMask() const { return mSwizzleMask; }
	void						setSwizzleMask( const std::array<GLint,4> &swizzleMask ) { mSwizzleMask = swizzleMask; }

	size_t						getNumLevels() const { return mLevels.size(); }
	const Level&				getLevel( size_t index ) const { return mLevels.at( index ); }
	const std::vector<Level>&	getLevels() const { return mLevels; }
	Level&						back() { return mLevels.back(); }
	void						push_back( const Level &level ) { mLevels.push_back( level ); }
	void						clear() { mLevels.clear(); }

	void	allocateDataStore( size_t requireBytes );
	size_t	getDataStoreSize() const { return mDataStoreSize; }
	void*	getDataStorePtr( size_t offset ) const;
	void	mapDataStore();
	void	unmapDataStore();

  private:
	void		init();
	
	GLint				mWidth, mHeight, mDepth;
	GLint				mInternalFormat;
	GLenum				mDataFormat, mDataType;
	GLint				mUnpackAlignment;
	std::array<GLint,4>	mSwizzleMask;
	
	std::vector<Level>			mLevels;

  #if ! defined( CINDER_GL_ES )
	PboRef						mPbo;
	void*						mPboMappedPtr;
  #endif
	std::unique_ptr<uint8_t[]>	mDataStoreMem;
	size_t						mDataStoreSize;
};

class Texture2d : public TextureBase {
  public:
	struct Format : public TextureBase::Format {
		//! Default constructor, sets the target to \c GL_TEXTURE_2D, wrap to \c GL_CLAMP, disables mipmapping, the internal format to "automatic"
		Format() : TextureBase::Format(), mLoadTopDown( false ) {}

		//! Chaining functions for Format class.
		Format& target( GLenum target ) { mTarget = target; return *this; }
		Format& mipmap( bool enableMipmapping = true ) { mMipmapping = enableMipmapping; return *this; }
		//! Sets the maximum amount of anisotropic filtering. A value greater than 1.0 "enables" anisotropic filtering. Maximum of getMaxMaxAnisotropy();
		Format& maxAnisotropy( float maxAnisotropy ) { mMaxAnisotropy = maxAnisotropy; return *this; }
		Format& internalFormat( GLint internalFormat ) { mInternalFormat = internalFormat; return *this; }
		Format& wrap( GLenum wrap ) { mWrapS = mWrapT = mWrapR = wrap; return *this; }
		Format& wrapS( GLenum wrapS ) { mWrapS = wrapS; return *this; }
		Format& wrapT( GLenum wrapT ) { mWrapT = wrapT; return *this; }
#if ! defined( CINDER_GL_ES )
		Format& wrapR( GLenum wrapR ) { mWrapR = wrapR; return *this; }
#endif
		Format& minFilter( GLenum minFilter ) { setMinFilter( minFilter ); return *this; }
		Format& magFilter( GLenum magFilter ) { setMagFilter( magFilter ); return *this; }
		//! Corresponds to the 'format' parameter of glTexImage*(). Defaults to match the internalFormat (a value of \c -1)
		Format& pixelDataFormat( GLenum pixelDataFormat ) { mPixelDataFormat = pixelDataFormat; return *this; }
		//! Corresponds to the 'type' parameter of glTexImage*(). Defaults to \c GL_UNSIGNED_BYTE
		Format& pixelDataType( GLenum pixelDataType ) { mPixelDataType = pixelDataType; return *this; }
		//! Specifies the texture comparison mode for currently bound depth textures.
		Format& compareMode( GLenum compareMode ) { mCompareMode = compareMode; return *this; }
		//! Specifies the comparison operator used when \c GL_TEXTURE_COMPARE_MODE is set to \c GL_COMPARE_R_TO_TEXTURE.
		Format& compareFunc( GLenum compareFunc ) { mCompareFunc = compareFunc; return *this; }		Format& swizzleMask( const std::array<GLint,4> &swizzleMask ) { setSwizzleMask( swizzleMask ); return *this; }
		Format& swizzleMask( GLint r, GLint g, GLint b, GLint a ) { setSwizzleMask( r, g, b, a ); return *this; }
		//! Specifies whether the Texture should store scanlines top-down in memory. Default is \c false. Also marks Texture as top-down when \c true.
		Format& loadTopDown( bool loadTopDown = true ) { mLoadTopDown = loadTopDown; return *this; }
#if ! defined( CINDER_GL_ES )
		Format& intermediatePbo( const PboRef &intermediatePbo ) { setIntermediatePbo( intermediatePbo ); return *this; }
#endif		
		//! Sets the debugging label associated with the Texture. Calls glObjectLabel() when available.
		Format&	label( const std::string &label ) { setLabel( label ); return *this; }

	  protected:
		bool	mLoadTopDown;
		friend	Texture;
	};
	
	//! Constructs a texture of size(\a width, \a height) and allocates storage.
	static Texture2dRef	create( int width, int height, Format format = Format() );
	/** \brief Constructs a texture of size(\a width, \a height), storing the data in internal format \a aInternalFormat. Pixel data is provided by \a data and is expected to be interleaved and in format \a dataFormat, for which \c GL_RGB or \c GL_RGBA would be typical values. **/
	static Texture2dRef	create( const unsigned char *data, int dataFormat, int width, int height, Format format = Format() );
	//! Constructs a Texture based on the contents of \a surface.
	static Texture2dRef	create( const Surface8u &surface, Format format = Format() );
	//! Constructs a Texture based on the contents of \a channel. Sets swizzle mask to {R,R,R,1} where supported unless otherwise specified in \a format.
	static Texture2dRef	create( const Channel8u &channel, Format format = Format() );
	//! Constructs a Texture based on the contents of \a surface.
	static Texture2dRef	create( const Surface16u &surface, Format format = Format() );
	//! Constructs a Texture based on the contents of \a channel. Sets swizzle mask to {R,R,R,1} where supported unless otherwise specified in \a format.
	static Texture2dRef	create( const Channel16u &channel, Format format = Format() );
	//! Constructs a Texture based on the contents of \a surface.
	static Texture2dRef	create( const Surface32f &surface, Format format = Format() );
	/** \brief Constructs a texture based on the contents of \a channel. A default value of -1 for \a internalFormat chooses an appropriate internal format automatically. **/
	static Texture2dRef	create( const Channel32f &channel, Format format = Format() );
	//! Constructs a Texture based on \a imageSource. A default value of -1 for \a internalFormat chooses an appropriate internal format based on the contents of \a imageSource. Uses a Format's intermediate PBO when available, which is resized as necessary.
	static Texture2dRef	create( ImageSourceRef imageSource, Format format = Format() );
	//! Constructs a Texture based on an externally initialized OpenGL texture. \a doNotDispose specifies whether the Texture is responsible for disposing of the associated OpenGL resource.
	static Texture2dRef	create( GLenum target, GLuint aTextureID, int width, int height, bool doNotDispose );
	//! Constructs a Texture based on an externally initialized OpenGL texture. \a doNotDispose specifies whether the Texture is responsible for disposing of the associated OpenGL resource. Supports a custom deleter.
	template<typename D>
	static Texture2dRef	create( GLenum target, GLuint aTextureID, int width, int height, bool doNotDispose, D deleter );
	//! Constructs a Texture based on an instance of TextureData
	static Texture2dRef	create( const TextureData &data, const Format &format );
	//! Constructs a Texture from an optionally compressed KTX file. Enables mipmapping if KTX file contains mipmaps and Format has not specified \c false for mipmapping. Uses Format's intermediate PBO if supplied; requires it to be large enough to hold all MIP levels and throws if it is not. (http://www.khronos.org/opengles/sdk/tools/KTX/file_format_spec/)
	static Texture2dRef	createFromKtx( const DataSourceRef &dataSource, const Format &format = Format() );
#if ! defined( CINDER_GL_ES ) || defined( CINDER_GL_ANGLE )
	//! Constructs a Texture from a DDS file. Supports DXT1, DTX3, and DTX5. Supports BC7 in the presence of \c GL_ARB_texture_compression_bptc. Enables mipmapping if DDS contains mipmaps and Format has not specified \c false for mipmapping. ANGLE version requires textures to be a multiple of 4 due to DX limitation.
	static Texture2dRef	createFromDds( const DataSourceRef &dataSource, const Format &format = Format() );
#endif

	//! Allows specification of some size other than the Texture's true size for cases where not all pixels in the Texture are usable / "clean"; common in video decoding pipelines in particular. Specified in pixels, and relative to whichever origin is appropriate to the Texture's "top-downness".
	void			setCleanSize( GLint cleanWidth, GLint cleanHeight );
	
	//! Updates the pixels of a Texture with contents of \a surface. Expects \a surface's size to match the Texture's at \a mipLevel. \a destLowerLeftOffset specifies a texel offset to copy to within the Texture.
	void			update( const Surface8u &surface, int mipLevel = 0, const ivec2 &destLowerLeftOffset = ivec2( 0, 0 ) );
	//! Updates the pixels of a Texture with contents of \a channel. Expects \a channel's size to match the Texture's at \a mipLevel. \a destLowerLeftOffset specifies a texel offset to copy to within the Texture.
	void			update( const Channel8u &channel, int mipLevel = 0, const ivec2 &destLowerLeftOffset = ivec2( 0, 0 ) );
	//! Updates the pixels of a Texture with contents of \a surface. Expects \a surface's size to match the Texture's at \a mipLevel. \a destLowerLeftOffset specifies a texel offset to copy to within the Texture.
	void			update( const Surface16u &surface, int mipLevel = 0, const ivec2 &destLowerLeftOffset = ivec2( 0, 0 ) );
	//! Updates the pixels of a Texture with contents of \a channel. Expects \a channel's size to match the Texture's at \a mipLevel. \a destLowerLeftOffset specifies a texel offset to copy to within the Texture.
	void			update( const Channel16u &channel, int mipLevel = 0, const ivec2 &destLowerLeftOffset = ivec2( 0, 0 ) );
	//! Updates the pixels of a Texture with contents of \a surface. Expects \a surface's size to match the Texture's at \a mipLevel. \a destLowerLeftOffset specifies a texel offset to copy to within the Texture.
	void			update( const Surface32f &surface, int mipLevel = 0, const ivec2 &destLowerLeftOffset = ivec2( 0, 0 ) );
	//! Updates the pixels of a Texture with contents of \a channel. Expects \a channel's size to match the Texture's at \a mipLevel. \a destLowerLeftOffset specifies a texel offset to copy to within the Texture.
	void			update( const Channel32f &channel, int mipLevel = 0, const ivec2 &destLowerLeftOffset = ivec2( 0, 0 ) );
	//! Updates the pixels of a Texture with contents of \a textureData. Inefficient if the bounds of \a textureData don't match those of \a this
	void			update( const TextureData &textureData );
#if ! defined( CINDER_GL_ES )
	//! Updates the pixels of a Texture with the contents of a PBO (whose target must be \c GL_PIXEL_UNPACK_BUFFER) at mipmap level \a mipLevel. \a format and \a type correspond to parameters of glTexSubImage2D, and would often be GL_RGB and GL_UNSIGNED_BYTE respectively. Reads from the PBO starting at \a pboByteOffset.
	void			update( const PboRef &pbo, GLenum format, GLenum type, int mipLevel = 0, size_t pboByteOffset = 0 );
	//! Updates a subregion (measured as origin upper-left) of the pixels of a Texture with the contents of a PBO (whose target must be \c GL_PIXEL_UNPACK_BUFFER) at mipmap level \a mipLevel.  \a format and \a type correspond to parameters of glTexSubImage2D, and would often be GL_RGB and GL_UNSIGNED_BYTE respectively. Reads from the PBO starting at \a pboByteOffset.
	void			update( const PboRef &pbo, GLenum format, GLenum type, const Area &destArea, int mipLevel = 0, size_t pboByteOffset = 0 );
	//! Updates a Texture from a KTX file. Uses \a intermediatePbo if supplied; requires it to be large enough to hold all MIP levels and throws if it is not.
	void			updateFromKtx( const DataSourceRef &dataSource, const PboRef &intermediatePbo = PboRef() );
	//! Updates a Texture from a DDS file. Uses \a intermediatePbo if supplied; requires it to be large enough to hold all MIP levels and throws if it is not.
	void			updateFromDds( const DataSourceRef &dataSource, const PboRef &intermediatePbo = PboRef() );
#else
	//! Updates a Texture from a KTX file. Assumes sizes match.
	void			updateFromKtx( const DataSourceRef &dataSource );
  #if defined( CINDER_GL_ANGLE )
	//! Updates a Texture from a KTX file. Assumes sizes match.
	void			updateFromDds( const DataSourceRef &dataSource );
  #endif
#endif
	//! Replaces the pixels (and data store) of a Texture with contents of \a textureData. Use update() instead if the bounds of \a this match those of \a textureData
	void			replace( const TextureData &textureData );

	//! Returns the number of mip levels the texture bounds require
	GLint			getNumMipLevels() const;
	//! Returns the width of the texture in pixels, ignoring clean bounds.
	GLint			getWidth() const { return mWidth; }
	//! Returns the height of the texture in pixels, ignoring clean bounds.
	GLint			getHeight() const { return mHeight; }
	//! Returns the width of the texture in pixels accounting for its clean bounds - \sa getCleanBounds()
	GLint			getCleanWidth() const;
	//! Returns the height of the texture in pixels accounting for its clean bounds - \sa getCleanBounds()
	GLint			getCleanHeight() const;
	//! Returns size of the texture in pixels, igonring clean bounds
	ivec2			getSize() const { return ivec2( getWidth(), getHeight() ); }
	//! Returns the aspect ratio of the texture (width / height), ignoring clean bounds.
	float			getAspectRatio() const { return getWidth() / (float)getHeight(); }
	//! Returns the Area defining the Texture's bounds in pixels, ignoring clean bounds.
	Area			getBounds() const { return Area( 0, 0, getWidth(), getHeight() ); }
	//! Returns the Area defining the Texture's bounds in pixels, accounting for clean bounds.
	Area			getCleanBounds() const { return Area( 0, 0, getCleanWidth(), getCleanHeight() ); }
	//! Returns the UV coordinates which correspond to the pixels contained in \a area (as expressed with an upper-left origin). Accounts for clean bounds, top-down storage and target (0-1 for \c GL_TEXTURE_2D and pixel for GL_TEXTURE_RECTANGLE)
	Rectf			getAreaTexCoords( const Area &area ) const;
	//! Returns whether the scanlines of the image are stored top-down in memory relative to the base address. Default is \c false.
	bool			isTopDown() const { return mTopDown; }
	//!	Marks whether the scanlines of the image are stored top-down in memory relative to the base address. Default is \c false.
	void			setTopDown( bool topDown = true ) { mTopDown = topDown; }
	
	//! Returns an ImageSource pointing to this Texture
	ImageSourceRef	createSource();
	
  protected:
	virtual void	printDims( std::ostream &os ) const override;

	Texture2d( int width, int height, Format format = Format() );
	Texture2d( const unsigned char *data, int dataFormat, int width, int height, Format format = Format() );
	Texture2d( const Surface8u &surface, Format format = Format() );
	Texture2d( const Surface16u &surface, Format format = Format() );
	Texture2d( const Surface32f &surface, Format format = Format() );
	Texture2d( const Channel8u &channel, Format format = Format() );
	Texture2d( const Channel16u &channel, Format format = Format() );
	Texture2d( const Channel32f &channel, Format format = Format() );
	Texture2d( const ImageSourceRef &imageSource, Format format = Format() );
	Texture2d( GLenum target, GLuint textureId, int width, int height, bool doNotDispose );
	Texture2d( const TextureData &data, Format format );
	
	void	initParams( Format &format, GLint defaultInternalFormat );
	template<typename T>
	void	setData( const SurfaceT<T> &surface, bool createStorage, int mipLevel, const ivec2 &offset );
	template<typename T>
	void	setData( const ChannelT<T> &channel, bool createStorage, int mipLevel, const ivec2 &offset );
	void	initData( const unsigned char *data, GLenum dataFormat, GLenum type, const Format &format );
	void	initData( const float *data, GLint dataFormat, const Format &format );
	void	initData( const ImageSourceRef &imageSource, const Format &format );
#if ! defined( CINDER_GL_ES )
	void	initDataImageSourceWithPboImpl( const ImageSourceRef &imageSource, const Format &format, GLint dataFormat, ImageIo::ChannelOrder channelOrder, bool isGray, const PboRef &pbo );
#endif
	void	initDataImageSourceImpl( const ImageSourceRef &imageSource, const Format &format, GLint dataFormat, ImageIo::ChannelOrder channelOrder, bool isGray );

	mutable GLint	mWidth, mHeight, mCleanWidth, mCleanHeight;
	bool			mTopDown;
	
	friend class Texture2dCache;
};

#ifndef CINDER_GL_ES
class Texture3d : public TextureBase {
  public:
	struct Format : public TextureBase::Format {
		//! Default constructor, sets the target to \c GL_TEXTURE_3D, wrap to \c GL_CLAMP, disables mipmapping, the internal format to "automatic"
		Format() : TextureBase::Format() { mTarget = GL_TEXTURE_3D; }

		//! Chaining functions for Format class.
		//! Sets the target, defaults to \c GL_TEXTURE_3D, also supports \c GL_TEXTURE_2D_ARRAY
		Format& target( GLenum target ) { mTarget = target; return *this; }
		Format& mipmap( bool enableMipmapping = true ) { mMipmapping = enableMipmapping; return *this; }
		//! Sets the maximum amount of anisotropic filtering. A value greater than 1.0 "enables" anisotropic filtering. Maximum of getMaxMaxAnisotropy();
		Format& maxAnisotropy( float maxAnisotropy ) { mMaxAnisotropy = maxAnisotropy; return *this; }
		Format& internalFormat( GLint internalFormat ) { mInternalFormat = internalFormat; return *this; }
		Format& wrap( GLenum wrap ) { mWrapS = mWrapT = mWrapR = wrap; return *this; }
		Format& wrapS( GLenum wrapS ) { mWrapS = wrapS; return *this; }
		Format& wrapT( GLenum wrapT ) { mWrapT = wrapT; return *this; }
#if ! defined( CINDER_GL_ES )
		Format& wrapR( GLenum wrapR ) { mWrapR = wrapR; return *this; }
#endif
		Format& minFilter( GLenum minFilter ) { setMinFilter( minFilter ); return *this; }
		Format& magFilter( GLenum magFilter ) { setMagFilter( magFilter ); return *this; }
		Format& pixelDataFormat( GLenum pixelDataFormat ) { mPixelDataFormat = pixelDataFormat; return *this; }
		Format& pixelDataType( GLenum pixelDataType ) { mPixelDataType = pixelDataType; return *this; }
		//! Sets the debugging label associated with the Texture. Calls glObjectLabel() when available.
		Format&	label( const std::string &label ) { setLabel( label ); return *this; }
		
		friend Texture3d;
	};

	static Texture3dRef create( GLint width, GLint height, GLint depth, Format format = Format() );
	static Texture3dRef create( GLint width, GLint height, GLint depth, GLenum dataFormat, const uint8_t *data, Format format = Format() );	
  
	void	update( const Surface &surface, int depth, int mipLevel = 0 );
	
	//! Returns the width of the texture in pixels
	GLint			getWidth() const { return mWidth; }
	//! Returns the height of the texture in pixels
	GLint			getHeight() const { return mHeight; }
	//! Returns the depth of the texture, which is the number of images in a texture array, or the depth of a 3D texture measured in pixels
	GLint			getDepth() const { return mDepth; }
	//! the aspect ratio of the texture (width / height)
	float			getAspectRatio() const { return getWidth() / (float)getHeight(); }
	//! the Area defining the Texture's 2D bounds in pixels: [0,0]-[width,height]
	Area			getBounds() const { return Area( 0, 0, getWidth(), getHeight() ); }

  protected:
  	Texture3d( GLint width, GLint height, GLint depth, Format format );
	Texture3d( GLint width, GLint height, GLint depth, GLenum dataFormat, const uint8_t *data, Format format );

	virtual void	printDims( std::ostream &os ) const override;

	GLint		mWidth, mHeight, mDepth;
};
#endif

class TextureCubeMap : public TextureBase
{
  public:
  	struct Format : public TextureBase::Format {
		//! Default constructor, sets the target to \c GL_TEXTURE_CUBE_MAP, wrap to \c GL_CLAMP_TO_EDGE, disables mipmapping, the internal format to "automatic"
		Format();

		//! Chaining functions for Format class.
		Format& target( GLenum target ) { mTarget = target; return *this; }
		Format& mipmap( bool enableMipmapping = true ) { mMipmapping = enableMipmapping; return *this; }
		//! Sets the maximum amount of anisotropic filtering. A value greater than 1.0 "enables" anisotropic filtering. Maximum of getMaxMaxAnisotropy();
		Format& maxAnisotropy( float maxAnisotropy ) { mMaxAnisotropy = maxAnisotropy; return *this; }
		Format& internalFormat( GLint internalFormat ) { mInternalFormat = internalFormat; return *this; }
		Format& wrap( GLenum wrap ) { mWrapS = mWrapT = mWrapR = wrap; return *this; }
		Format& wrapS( GLenum wrapS ) { mWrapS = wrapS; return *this; }
		Format& wrapT( GLenum wrapT ) { mWrapT = wrapT; return *this; }
#if ! defined( CINDER_GL_ES )
		Format& wrapR( GLenum wrapR ) { mWrapR = wrapR; return *this; }		
#endif // ! defined( CINDER_GL_ES )
		Format& minFilter( GLenum minFilter ) { setMinFilter( minFilter ); return *this; }
		Format& magFilter( GLenum magFilter ) { setMagFilter( magFilter ); return *this; }
		//! Sets the debugging label associated with the Texture. Calls glObjectLabel() when available.
		Format&	label( const std::string &label ) { setLabel( label ); return *this; }
		
		friend TextureCubeMap;
	};
  
	static TextureCubeMapRef	create( int32_t width, int32_t height, const Format &format = Format() );
	static TextureCubeMapRef	createHorizontalCross( const ImageSourceRef &imageSource, const Format &format = Format() );
	//! Expects images ordered { +X, -X, +Y, -Y, +Z, -Z }
	static TextureCubeMapRef	create( const ImageSourceRef images[6], const Format &format = Format() );
	
  protected:
	TextureCubeMap( int32_t width, int32_t height, Format format );
	template<typename T>
	TextureCubeMap( const SurfaceT<T> images[6], Format format );

	template<typename T>
	static TextureCubeMapRef createHorizontalCrossImpl( const ImageSourceRef &imageSource, const Format &format );

	virtual void	printDims( std::ostream &os ) const override;
	
	GLint		mWidth, mHeight;
};

typedef std::shared_ptr<class Texture2dCache> Texture2dCacheRef;

class Texture2dCache : public std::enable_shared_from_this<Texture2dCache>
{
  public:
	static Texture2dCacheRef create();
	static Texture2dCacheRef create( const Surface8u &prototypeSurface, const Texture2d::Format &format );
	
	Texture2dRef		cache( const Surface8u &data );
  protected:
	Texture2dCache();
	Texture2dCache( const Surface8u &prototypeSurface, const Texture2d::Format &format );
		
	void			markTextureAsFree( int id );
	
	int					mWidth;
	int					mHeight;
	Texture2d::Format	mFormat;
	Surface8u			mIntermediateSurface;

	int										mNextId;
	std::vector<std::pair<int,TextureRef>>	mTextures;
};

template<typename D>
Texture2dRef Texture2d::create( GLenum target, GLuint textureID, int width, int height, bool doNotDispose, D deleter )
{
	return TextureRef( new Texture( target, textureID, width, height, doNotDispose ), deleter );
}

class SurfaceConstraintsGLTexture : public SurfaceConstraints {
  public:
	virtual SurfaceChannelOrder getChannelOrder( bool alpha ) const { return ( alpha ) ? SurfaceChannelOrder::BGRA : SurfaceChannelOrder::BGR; }
	virtual int32_t				getRowBytes( int requestedWidth, const SurfaceChannelOrder &sco, int elementSize ) const { return requestedWidth * elementSize * sco.getPixelInc(); }
};

class KtxParseExc : public Exception {
  public:	
	KtxParseExc( const std::string &description ) : Exception( description )	{}
};

class DdsParseExc : public Exception {
  public:	
	DdsParseExc( const std::string &description ) : Exception( description )	{}
};

class TextureDataExc : public Exception {
  public:	
	TextureDataExc( const std::string &description ) : Exception( description )	{}
};

class TextureResizeExc : public TextureDataExc {
  public:
	TextureResizeExc( const std::string &description, const ivec2 &updateSize, const ivec2 &textureSize );
};

class TextureDataStoreTooSmallExc : public Exception {
  public:
	TextureDataStoreTooSmallExc() {}

	virtual const char* what() const throw()	{ return "The data store allocated for a TextureData is too small"; }
};
	
} } // namespace cinder::gl
