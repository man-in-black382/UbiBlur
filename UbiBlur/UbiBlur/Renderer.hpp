#pragma once

#include <Mesh.hpp>
#include <GLFramebuffer.hpp>
#include <GLProgram.hpp>
#include <Camera.hpp>
#include <GLVertexArray.hpp>
#include <Drawable.hpp>
#include <GaussianBlur/GaussianBlurEffect.hpp>
#include <filesystem/path.h>
#include <memory>

namespace Engine {

	class Renderer {
	public:
		enum class ShadingModel: int8_t { CookTorrance, BlinnPhong };

	private:
		Camera mCamera;
		GLFramebuffer mFramebuffer;
		GLNormalizedTexture2D<Engine::GLTexture::Normalized::RGBA> mRenderTarget;
		GLDepthTexture2D mDepthBuffer;

		GLProgram mMeshShader;
		GLProgram mRTOutputShader;
		GLProgram mBackgroundPatternShader;
		GLVertexArray<Vertex1P1N2UV1T1BT> mVAO;

		std::unique_ptr<GLNormalizedTexture2D<GLTexture::Normalized::RGBACompressedRGBAInput>> mAlbedoMap;
		std::unique_ptr<GLNormalizedTexture2D<GLTexture::Normalized::RGBCompressedRGBAInput>> mNormalMap;
		std::unique_ptr<GLNormalizedTexture2D<GLTexture::Normalized::RCompressedRGBAInput>> mRoughnessMap;

		GaussianBlurEffect mBlurEffect;

		bool mBlurEnabled = true;
		ShadingModel mShadingModel = ShadingModel::CookTorrance;

		GLVertexArray<Vertex1P1N2UV1T1BT> constructMeshVAO(const filesystem::path &resourceRoot);

		void renderBackground();

		void renderMesh();

		void renderFinalImage();

	public:
		Renderer(const filesystem::path &resourceRoot, const Size2D &rtSize);

		bool isBlurEnabled() const;

		void setBlurEnabled(bool enabled);

		void setShadingModel(ShadingModel model);

		void render();
	};

}



