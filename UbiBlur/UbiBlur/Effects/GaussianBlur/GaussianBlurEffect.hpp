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

#include "GaussianBlurSettings.hpp"

#include <memory>
#include <vector>

namespace Engine {

    class GaussianBlurEffect {
    private:
		GLProgram mBlurShader;
		GLFramebuffer mFramebuffer;
		GLNormalizedTexture2D<GLTexture::Normalized::RGBA> mIntermediateImage;
        std::vector<float> mWeights;
        std::vector<float> mTextureOffsets;
        GaussianBlurSettings mSettings;

        void computeWeightsAndOffsetsIfNeeded(const GaussianBlurSettings &settings);

    public:
		GaussianBlurEffect(const filesystem::path &resourceRoot, const Size2D &rtSize);

		void blur(
			const GLNormalizedTexture2D<GLTexture::Normalized::RGBA> &image,
			const GLFramebuffer &framebuffer,
			const GaussianBlurSettings &settings
		);
	};

}

#endif /* GaussianBlurEffect_hpp */
