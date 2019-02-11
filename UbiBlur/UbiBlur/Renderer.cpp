#include "Renderer.hpp"

#include <WavefrontMeshLoader.hpp>
#include <GLTextureFactory.hpp>

namespace Engine {

	Renderer::Renderer(const filesystem::path &resourceRoot, const Size2D &rtSize)
		: mCamera(90.0, 10.0, 200.0),
		mFramebuffer(rtSize),
		mRenderTarget(rtSize),
		mDepthStencilRenderbuffer(rtSize),
		mMeshShader(resourceRoot.str() + "\\Shaders\\Mesh.vert", resourceRoot.str() + "\\Shaders\\Mesh.frag", ""),
		mRTOutputShader(resourceRoot.str() + "\\Shaders\\FullScreenQuad.vert", resourceRoot.str() + "\\Shaders\\FullScreenQuad.frag", ""),
		mBackgroundPatternShader(resourceRoot.str() + "\\Shaders\\FullScreenQuad.vert", resourceRoot.str() + "\\Shaders\\BackgroundPattern.frag", ""),
		mVAO(constructMeshVAO(resourceRoot)), 
		mAlbedoMap(GLTextureFactory::LoadLDRImage<GLTexture::Normalized::RGBACompressedRGBAInput>(resourceRoot.str() + "\\Textures\\albedo.png")),
		mNormalMap(GLTextureFactory::LoadLDRImage<GLTexture::Normalized::RGBCompressedRGBAInput>(resourceRoot.str() + "\\Textures\\normal.png")),
		mRoughnessMap(GLTextureFactory::LoadLDRImage<GLTexture::Normalized::RCompressedRGBAInput>(resourceRoot.str() + "\\Textures\\roughness.png")),
		mBlurEffect(resourceRoot, rtSize) {
		 
		mFramebuffer.attachRenderbuffer(mDepthStencilRenderbuffer);
		mFramebuffer.attachTexture(mRenderTarget);

		mCamera.setViewportAspectRatio(mFramebuffer.size().width / mFramebuffer.size().height);
		mCamera.moveTo(glm::vec3(0.0, 10.0, 100.0));
		mCamera.lookAt(glm::vec3(0.0, 0.0, 0.0));
		 
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClearDepth(1.0);

		mAlbedoMap->generateMipMaps();
		mNormalMap->generateMipMaps();
		mRoughnessMap->generateMipMaps();
	}

	GLVertexArray<Vertex1P1N2UV1T1BT> Renderer::constructMeshVAO(const filesystem::path &resourceRoot) {
		WavefrontMeshLoader loader(resourceRoot.str() + "\\Models\\Teapot.obj");

		std::vector<Engine::Mesh> meshes;
		std::string meshName;
		AxisAlignedBox3D boundingBox;
		loader.load(meshes, meshName, boundingBox); 

		auto &mesh = meshes.back();
		std::array<Engine::GLVertexAttribute, 6> attributes{
					Engine::GLVertexAttribute::UniqueAttribute(sizeof(glm::vec4), glm::vec4::length()),
					Engine::GLVertexAttribute::UniqueAttribute(sizeof(glm::vec3), glm::vec3::length()),
					Engine::GLVertexAttribute::UniqueAttribute(sizeof(glm::vec2), glm::vec2::length()),
					Engine::GLVertexAttribute::UniqueAttribute(sizeof(glm::vec3), glm::vec3::length()),
					Engine::GLVertexAttribute::UniqueAttribute(sizeof(glm::vec3), glm::vec3::length()),
					Engine::GLVertexAttribute::UniqueAttribute(sizeof(glm::vec3), glm::vec3::length()) 
		}; 

		return GLVertexArray<Vertex1P1N2UV1T1BT>(
			mesh.vertices().data(), mesh.vertices().size(), 
			attributes.data(), attributes.size());
	}

	bool Renderer::isBlurEnabled() const {
		return mBlurEnabled;
	}

	void Renderer::setBlurEnabled(bool enabled) {
		mBlurEnabled = enabled;
	}

	void Renderer::setShadingModel(ShadingModel model) {
		mShadingModel = model;
	}

	void Renderer::renderBackground() {
		glDisable(GL_DEPTH_TEST);
		mBackgroundPatternShader.bind();
		Drawable::TriangleStripQuad::Draw();
		glEnable(GL_DEPTH_TEST);
	}

	void Renderer::renderMesh() {
		glm::mat4 modelMatrix(1.0);
		glm::mat4 normalMatrix(glm::transpose(glm::inverse(modelMatrix)));

		auto model = std::underlying_type<ShadingModel>::type(mShadingModel);

		mMeshShader.bind();
		mMeshShader.setUniformInteger(ctcrc32("uLightingModel"), model);
		mMeshShader.setUniformMatrix(ctcrc32("uModelMat"), modelMatrix);
		mMeshShader.setUniformMatrix(ctcrc32("uNormalMat"), normalMatrix);
		mMeshShader.setUniformMatrix(ctcrc32("uCameraViewMat"), mCamera.viewMatrix());
		mMeshShader.setUniformMatrix(ctcrc32("uCameraProjectionMat"), mCamera.projectionMatrix());
		mMeshShader.setUniformVector(ctcrc32("uCameraPosition"), mCamera.position());
		mMeshShader.ensureSamplerValidity([&] {
			mMeshShader.setUniformTexture(ctcrc32("uMaterial.albedoMap"), *mAlbedoMap);
			mMeshShader.setUniformTexture(ctcrc32("uMaterial.normalMap"), *mNormalMap);
			mMeshShader.setUniformTexture(ctcrc32("uMaterial.roughnessMap"), *mRoughnessMap);
		});

		mFramebuffer.clear(GLFramebuffer::UnderlyingBuffer::Depth);
		Drawable::TriangleMesh::Draw(mVAO.vertexBuffer()->count());
	}

	void Renderer::renderFinalImage() {
		mRTOutputShader.bind();
		mRTOutputShader.ensureSamplerValidity([&] {
			mRTOutputShader.setUniformTexture(ctcrc32("uImage"), mRenderTarget);
		});

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Drawable::TriangleStripQuad::Draw();
	}

	void Renderer::render() {
		mFramebuffer.bind();
		mFramebuffer.viewport().apply();

		renderBackground();
		renderMesh();

		if (mBlurEnabled) {
			mBlurEffect.blurWithVertexMask(mRenderTarget, mFramebuffer, { 30, 15 });
			//mBlurEffect.blurWithStencilMask(mRenderTarget, mFramebuffer, { 30, 15 });
		}

		renderFinalImage();
	}

}

