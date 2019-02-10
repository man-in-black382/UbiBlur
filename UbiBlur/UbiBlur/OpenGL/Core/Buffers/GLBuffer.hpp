//
//  GLBuffer.hpp
//  Engine
//
//  Created by Pavlo Muratov on 18.03.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#ifndef GLBuffer_hpp
#define GLBuffer_hpp

#include <vector>
#include <algorithm>
#include <cmath>

#include "GLNamedObject.hpp"
#include "MemoryUtils.hpp"

namespace Engine {

    template<typename DataType>
    class GLBuffer : public GLNamedObject {
    private:
        GLint mBindingPoint;
        GLenum mUsageMode;
        size_t mAlignment = 1;
        size_t mSize = 0;
        size_t mCount = 0;

    public:
        template<template<class...> class ContinuousContainer>
        static auto Create(const ContinuousContainer<DataType> &data, GLint bindingPoint, GLenum usageMode, size_t perObjectAlignment = 1) {
            return GLBuffer<DataType>(data.data(), data.size(), bindingPoint, usageMode, perObjectAlignment);
        }

        GLBuffer(const DataType *data, uint64_t count, GLint bindingPoint, GLenum usageMode, size_t perObjectAlignment = 1)
                : mBindingPoint(bindingPoint), mUsageMode(usageMode), mAlignment(std::max(perObjectAlignment, (size_t)1)) {

            if (count == 0) {
                throw std::invalid_argument("Buffer size cannot be 0, though it can be constructed without any data");
            }

            glGenBuffers(1, &mName);
            bind();

            size_t padding = Utils::Memory::Padding<DataType>(perObjectAlignment);
            size_t objectSize = sizeof(DataType);
            size_t alignedObjectSize = objectSize + padding;
            size_t totalBytes = alignedObjectSize * count;

            mSize = totalBytes;
            mCount = count;

            // Data is already aligned. Lucky!
            if (alignedObjectSize == objectSize || data == nullptr) {
                glBufferData(mBindingPoint, totalBytes, data, mUsageMode);
                return;
            }

            // Not aligned. Have to allocate additional memory chunk.
            unsigned char *alignedStorage = new unsigned char[totalBytes];
            for (int i = 0; i < count; ++i) {
                memcpy(alignedStorage + alignedObjectSize * i, data + i, objectSize);
            }
            glBufferData(mBindingPoint, totalBytes, alignedStorage, mUsageMode);
            delete[] (alignedStorage);
        }

        ~GLBuffer() override {
            glDeleteBuffers(1, &mName);
        }

        GLBuffer(GLBuffer &&that) = default;

        GLBuffer &operator=(GLBuffer &&rhs) = default;


        /// Size in bytes
        /// @return Size of the buffer
        size_t size() const {
            return mSize;
        }

        /// Amount of objects stored in the buffer
        /// @return Number of stored objects
        size_t count() const {
            return mCount;
        }

        size_t alignment() const {
            return mAlignment;
        }


        virtual void bind() const {
            glBindBuffer(mBindingPoint, mName);
        }

    };

}

#endif /* GLBuffer_hpp */
