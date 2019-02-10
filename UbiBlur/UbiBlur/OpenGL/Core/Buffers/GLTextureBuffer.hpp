//
//  GLBufferTexture.hpp
//  Engine
//
//  Created by Pavel Muratov on 12/18/17.
//  Copyright © 2017 MPO. All rights reserved.
//

#ifndef GLTextureBuffer_hpp
#define GLTextureBuffer_hpp

#include "GLBuffer.hpp"

namespace Engine {

    template<typename DataType>
    class GLTextureBuffer : public GLBuffer<DataType> {
    private:
        GLenum mInternalFormat;

    public:
        template<template<class...> class ContinuousContainer>
        static auto Create(const ContinuousContainer<DataType> &data, GLenum internalFormat) {
            return GLTextureBuffer(data.data(), data.size(), internalFormat);
        }

        GLTextureBuffer(const DataType *data, uint64_t count, GLenum internalFormat)
                : GLBuffer<DataType>(data, count, GL_TEXTURE_BUFFER, GL_STATIC_DRAW),
                  mInternalFormat(internalFormat) {}

        void bind() const {
            GLBuffer<DataType>::bind();
            glTexBuffer(GL_TEXTURE_BUFFER, mInternalFormat, GLBuffer<DataType>::mName);
        }
    };

}

#endif /* GLTextureBuffer_hpp */
