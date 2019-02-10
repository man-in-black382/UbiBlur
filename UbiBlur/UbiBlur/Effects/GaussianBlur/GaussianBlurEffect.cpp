//
//  GaussianBlurEffect.cpp
//  EARenderer
//
//  Created by Pavel Muratov on 6/14/18.
//  Copyright © 2018 MPO. All rights reserved.
//

#include "GaussianBlurEffect.hpp"
#include "GaussianFunction.hpp"
#include "Drawable.hpp"
#include "GLTexture2D.hpp"

#include <stdexcept>

namespace Engine {

	GaussianBlurEffect::GaussianBlurEffect(const filesystem::path &resourceRoot, const Size2D &rtSize)
		: mBlurShader(resourceRoot.str() + "\\Shaders\\GaussianBlur.vert", resourceRoot.str() + "\\Shaders\\GaussianBlur.frag", ""),
		mFramebuffer(rtSize),
		mIntermediateImage(rtSize) {
	
		mFramebuffer.attachTexture(mIntermediateImage);
	}

    void GaussianBlurEffect::computeWeightsAndOffsetsIfNeeded(const GaussianBlurSettings& settings) {
        if (settings == mSettings && !mWeights.empty()) {
            return;
        }

        bool isOdd = settings.radius % 2 == 1;
        mSettings.radius = isOdd ? settings.radius + 1 : settings.radius; 
        mSettings.sigma = settings.sigma;

        auto weights = GaussianFunction::Produce1DKernel(mSettings.radius, mSettings.sigma);

        mWeights.clear();
        mTextureOffsets.clear();

        // Kernel's center
        mWeights.push_back(weights[0]);
        mTextureOffsets.push_back(0.0);

        // Calculate texture offsets and combined weights to make advantage of hardware interpolation
        for (size_t i = 1; i <= mSettings.radius; i += 2) {
            float weight1 = weights[i];
            float weight2 = weights[i + 1];
            float totalWeight = weight1 + weight2;

            float texOffset1 = i;
            float texOffset2 = i + 1;

            float texOffset = (texOffset1 * weight1 + texOffset2 * weight2) / totalWeight;

            mWeights.push_back(totalWeight);
            mTextureOffsets.push_back(texOffset);
        }
    }

	void GaussianBlurEffect::blur(
		const GLNormalizedTexture2D<GLTexture::Normalized::RGBA> &image,
		const GLFramebuffer &framebuffer,
		const GaussianBlurSettings &settings
	) 
	{
		if (settings.radius == 0) throw std::invalid_argument("Blur radius must be greater than 0");

		computeWeightsAndOffsetsIfNeeded(settings);

		mBlurShader.bind();
		mBlurShader.setUniformVector(ctcrc32("uRenderTargetSize"), glm::vec2(image.size().width, image.size().height));
		mBlurShader.setUniformFloatArray(ctcrc32("uKernelWeights[0]"), mWeights.data(), mWeights.size());
		mBlurShader.setUniformFloatArray(ctcrc32("uTextureOffsets[0]"), mTextureOffsets.data(), mTextureOffsets.size());
		mBlurShader.setUniformInteger(ctcrc32("uKernelSize"), mTextureOffsets.size());

		// Set horizontal direction
		mBlurShader.setUniformVector(ctcrc32("uBlurDirection"), glm::vec2(1.0, 0.0));
		mBlurShader.ensureSamplerValidity([&]() {
			mBlurShader.setUniformTexture(ctcrc32("uTexture"), image);
		});

		mFramebuffer.bind();
		Drawable::TriangleStripQuad::Draw();

		// Set vertical direction
		mBlurShader.setUniformVector(ctcrc32("uBlurDirection"), glm::vec2(0.0, 1.0));
		mBlurShader.ensureSamplerValidity([&]() {
			mBlurShader.setUniformTexture(ctcrc32("uTexture"), mIntermediateImage);
		});

		framebuffer.bind();
		Drawable::TriangleStripQuad::Draw();
	}

}
