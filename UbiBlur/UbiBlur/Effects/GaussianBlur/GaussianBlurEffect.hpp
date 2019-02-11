//
//  GaussianBlurEffect.hpp
//  EARenderer
//
//  Created by Pavel Muratov on 6/14/18.
//  Copyright © 2018 MPO. All rights reserved.
//

#ifndef GaussianBlurEffect_hpp
#define GaussianBlurEffect_hpp

#include <GLTexture2D.hpp>
#include <Size2D.hpp>
#include <GLProgram.hpp>
#include <filesystem/path.h>
#include <GLFramebuffer.hpp>
#include <GLDepthStencilRenderbuffer.hpp>

#include "GaussianBlurSettings.hpp"

#include <memory>
#include <vector>

namespace Engine {

    class GaussianBlurEffect {
    private:
		GLProgram mHalfBlurShader;
		GLProgram mFullBlurShader;
		GLProgram mHalfQuadShader;
		GLFramebuffer mFramebuffer;
		GLDepthStencilRenderbuffer mDepthStencilRenderbuffer;
		GLNormalizedTexture2D<GLTexture::Normalized::RGBA> mIntermediateImage;
        std::vector<float> mWeights;
        std::vector<float> mTextureOffsets;
        GaussianBlurSettings mSettings;

        void computeWeightsAndOffsetsIfNeeded(const GaussianBlurSettings &settings);

		void produceStencilMask(GLFramebuffer &fbo);

		void blur(
			GLNormalizedTexture2D<GLTexture::Normalized::RGBA> &image,
			GLFramebuffer &framebuffer,
			GLProgram &blurShader,
			const GaussianBlurSettings &settings
		);

    public:
		GaussianBlurEffect(const filesystem::path &resourceRoot, const Size2D &rtSize);

		void blurWithStencilMask(
			GLNormalizedTexture2D<GLTexture::Normalized::RGBA> &image,
			GLFramebuffer &framebuffer,
			const GaussianBlurSettings &settings
		);

		void blurWithVertexMask(
			GLNormalizedTexture2D<GLTexture::Normalized::RGBA> &image,
			GLFramebuffer &framebuffer,
			const GaussianBlurSettings &settings
		);
	};

}

#endif /* GaussianBlurEffect_hpp */
