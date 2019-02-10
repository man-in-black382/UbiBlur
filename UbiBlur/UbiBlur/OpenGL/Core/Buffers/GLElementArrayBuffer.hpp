//
//  GLElementArrayBuffer.hpp
//  Engine
//
//  Created by Pavlo Muratov on 18.03.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#ifndef GLElementArrayBuffer_hpp
#define GLElementArrayBuffer_hpp

#include "GLBuffer.hpp"

namespace Engine {

    class GLElementArrayBuffer : public GLBuffer<GLushort> {
    public:
        template<template<class...> class ContinuousContainer>
        static auto Create(const ContinuousContainer<GLushort> &indices) {
            return GLElementArrayBuffer(indices.data(), indices.size());
        }

        GLElementArrayBuffer(const GLushort *indices, uint64_t count)
                : GLBuffer<GLushort>(indices, count, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW) {}
    };

}

#endif /* GLElementArrayBuffer_hpp */
