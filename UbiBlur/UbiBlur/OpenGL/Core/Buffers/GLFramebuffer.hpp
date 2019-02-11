//
//  GLFramebuffer.hpp
//  Engine
//
//  Created by Pavlo Muratov on 29.04.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#ifndef GLFramebuffer_hpp
#define GLFramebuffer_hpp

#include <GLNamedObject.hpp>
#include <GLViewport.hpp>
#include <GLTexture2D.hpp>
#include <GLDepthStencilRenderbuffer.hpp>
#include <Size2D.hpp>
#include <StringUtils.hpp>
#include <BitwiseEnum.hpp>

#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace Engine {

    class GLFramebuffer : public GLNamedObject {
    public:
        enum class ColorAttachment {
            Automatic = 0,
            Attachment0 = GL_COLOR_ATTACHMENT0, Attachment1 = GL_COLOR_ATTACHMENT1, Attachment2 = GL_COLOR_ATTACHMENT2, Attachment3 = GL_COLOR_ATTACHMENT3,
            Attachment4 = GL_COLOR_ATTACHMENT4, Attachment5 = GL_COLOR_ATTACHMENT5, Attachment6 = GL_COLOR_ATTACHMENT6, Attachment7 = GL_COLOR_ATTACHMENT7,
            Attachment8 = GL_COLOR_ATTACHMENT8, Attachment9 = GL_COLOR_ATTACHMENT9, Attachment10 = GL_COLOR_ATTACHMENT10, Attachment11 = GL_COLOR_ATTACHMENT11,
            Attachment12 = GL_COLOR_ATTACHMENT12, Attachment13 = GL_COLOR_ATTACHMENT13, Attachment14 = GL_COLOR_ATTACHMENT14, Attachment15 = GL_COLOR_ATTACHMENT15
        };

        enum class UnderlyingBuffer {
            None = 0, Color = GL_COLOR_BUFFER_BIT, Depth = GL_DEPTH_BUFFER_BIT, Stencil = GL_STENCIL_BUFFER_BIT
        };

    private:
        constexpr static int16_t AllLayers = -1;

        struct AttachmentMetadata {
            ColorAttachment colorAttachment = ColorAttachment::Automatic;
            GLenum glColorAttachment = GL_COLOR_ATTACHMENT0;
            uint16_t mipLevel = 0;
            int16_t layer = AllLayers;
        };

        GLint mBindingPoint;
        Size2D mSize;
        GLViewport mViewport;
        GLint mMaximumColorAttachments;
        GLint mMaximumDrawBuffers;
        std::unordered_set<GLenum> mRequestedAttachments;
        std::unordered_set<GLenum> mAvailableAttachments;
        std::unordered_map<GLint, AttachmentMetadata> mTextureAttachmentMap;
        std::vector<GLenum> mDrawBuffers;

        void obtainHardwareLimits();

        void setRequestedDrawBuffers();

        void attachTextureToDepthAttachment(const GLTexture &texture, uint16_t mipLevel = 0, int16_t layer = -1);

        void attachTextureToColorAttachment(const GLTexture &texture, ColorAttachment colorAttachment, uint16_t mipLevel = 0, int16_t layer = -1);

    public:

        GLFramebuffer(const Size2D &size);

        GLFramebuffer(GLFramebuffer &&that) = default;

        GLFramebuffer &operator=(GLFramebuffer &&rhs) = default;

        ~GLFramebuffer() override;

        void bind() const;

        const Size2D &size() const;

        const GLViewport &viewport() const;

        bool isComplete() const;

        size_t maximumColorAttachmentsCount() const;

        template<class Format, Format F>
        void attachTexture(const GLTexture2D<Format, F> &texture,
                uint16_t mipLevel = 0,
                ColorAttachment colorAttachment = ColorAttachment::Automatic) {
			attachTextureToColorAttachment(texture, colorAttachment, mipLevel);
		}

        void attachDepthTexture(const GLDepthTexture2D &texture, uint16_t mipLevel = 0);

		void attachRenderbuffer(const GLDepthStencilRenderbuffer &renderbuffer);

        void detachTexture(const GLTexture &texture);

        void detachAllColorAttachments();

        template<class Texture>
        void activateDrawBuffers(const Texture &texture);

        template<class Texture, class... Textures>
        void activateDrawBuffers(const Texture &head, const Textures &... tail);

        void activateAllDrawBuffers();

        void blit(const GLTexture &fromTexture, const GLTexture &toTexture, bool useLinearFilter = true);

        void clear(UnderlyingBuffer bufferMask);
    };

}

ENABLE_BITMASK_OPERATORS(Engine::GLFramebuffer::UnderlyingBuffer);

#endif /* GLFramebuffer_hpp */
