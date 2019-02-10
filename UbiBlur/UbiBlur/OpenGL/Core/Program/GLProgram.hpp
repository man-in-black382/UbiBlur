//
//  GLProgram.hpp
//  Engine
//
//  Created by Pavlo Muratov on 11.03.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#ifndef GLProgram_hpp
#define GLProgram_hpp

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <array>
#include <type_traits>
#include <stdexcept>
#include <memory>
#include <functional>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLNamedObject.hpp"
#include "GLVertexAttribute.hpp"
#include "GLUniform.hpp"
#include "GLUniformBlock.hpp"
#include "GLShader.hpp"
#include "GLSampler.hpp"
#include "GLTexture2D.hpp"
#include "GLTexture2D.hpp"
#include "CRC32.hpp"

namespace Engine {

    class GLProgram : public GLNamedObject {
    protected:
        using VertexAttributeName = std::string;
        using CRC32 = uint32_t;

    private:
        const GLShader *mVertexShader = nullptr;
        const GLShader *mFragmentShader = nullptr;
        const GLShader *mGeometryShader = nullptr;

        std::unordered_map<VertexAttributeName, GLVertexAttribute> mVertexAttributes;
        std::unordered_map<CRC32, GLUniform> mUniforms;

        GLint mAvailableTextureUnits = 0;

        bool isModifyingUniforms = false;

        void link();

        void obtainVertexAttributes();

        void obtainUniforms();

    protected:
        const GLVertexAttribute &vertexAttributeByName(const std::string &name);

        const GLUniform &uniformByNameCRC32(CRC32 crc32);

        const GLUniformBlock &uniformBlockByNameCRC32(CRC32 crc32);

    public:
        using UniformModifierClosure = const std::function<void()> &;

		GLProgram(const std::string &vertexSourcePath, const std::string &fragmentSourcePath, const std::string &geometrySourcePath);

        GLProgram(const GLProgram &rhs) = delete;

        GLProgram &operator=(const GLProgram &that) = delete;

        GLProgram(GLProgram &&that) = default;

        GLProgram &operator=(GLProgram &&that) = default;

        virtual ~GLProgram();

        void swap(GLProgram &);

        void bind() const;

        bool validateState() const;

        /**
         You should use this function for setting up uniform sampler values.
         If GLSL code for a particular program requires optional samplers,
         (no normal or gloss map provided for a particular object, etc.)
         it ensures 1-to-1 sampler - texture unit mapping for all unused samplers

         @param closure function object in which uniform sampler modifications should be performed
         */
        void ensureSamplerValidity(UniformModifierClosure closure);

		void setUniformTexture(CRC32 uniformNameCRC32, const GLTexture &texture, const GLSampler *sampler = nullptr);

		void setUniformMatrix(CRC32 uniformNameCRC32, const glm::mat4 &matrix);
		
		void setUniformVector(CRC32 uniformNameCRC32, const glm::vec2 &vector);

		void setUniformVector(CRC32 uniformNameCRC32, const glm::vec3 &vector);

		void setUniformVector(CRC32 uniformNameCRC32, const glm::vec4 &vector);

		void setUniformFloat(CRC32 uniformNameCRC32, float value);

		void setUniformFloatArray(CRC32 uniformNameCRC32, float *array, size_t count);

		void setUniformInteger(CRC32 uniformNameCRC32, int32_t value);

		void setUniformIntegerArray(CRC32 uniformNameCRC32, int32_t *array, size_t count);
    };

    void swap(GLProgram &, GLProgram &);

}

#endif /* GLProgram_hpp */
