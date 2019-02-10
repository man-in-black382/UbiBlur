//
//  GLProgram.cpp
//  Engine
//
//  Created by Pavlo Muratov on 11.03.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#include "GLProgram.hpp"
#include "StringUtils.hpp"
#include "GLTextureUnitManager.hpp"

#include <sstream>
#include <regex>

#include <glm/gtc/type_ptr.hpp>

namespace Engine {

    GLProgram::GLProgram(const std::string &vertexSourcePath, const std::string &fragmentSourcePath, const std::string &geometrySourcePath)
            : GLNamedObject(glCreateProgram()),
              mVertexShader(vertexSourcePath.empty() ? nullptr : new GLShader(vertexSourcePath, GL_VERTEX_SHADER)),
              mFragmentShader(fragmentSourcePath.empty() ? nullptr : new GLShader(fragmentSourcePath, GL_FRAGMENT_SHADER)),
              mGeometryShader(geometrySourcePath.empty() ? nullptr : new GLShader(geometrySourcePath, GL_GEOMETRY_SHADER)) {

        link();
        bind();
        obtainUniforms();
    }

    GLProgram::~GLProgram() {
        glDeleteProgram(mName);

        delete mVertexShader;
        delete mGeometryShader;
        delete mFragmentShader;
    }

    void GLProgram::link() {
        glAttachShader(mName, mVertexShader->name());

        if (mFragmentShader) {glAttachShader(mName, mFragmentShader->name());}
        if (mGeometryShader) {glAttachShader(mName, mGeometryShader->name());}

        glLinkProgram(mName);

        GLint isLinked = 0;
        glGetProgramiv(mName, GL_LINK_STATUS, &isLinked);

        if (isLinked) {
            return;
        }

        GLint infoLength = 0;
        glGetProgramiv(mName, GL_INFO_LOG_LENGTH, &infoLength);

        if (infoLength > 1) {
            std::vector<char> infoChars(infoLength);
            glGetProgramInfoLog(mName, infoLength, nullptr, infoChars.data());
            std::string infoLog(infoChars.data());

            if (!isLinked) {
                throw std::runtime_error(string_format("Failed to link program: %s", infoLog.c_str()));
            }
        }

        if (!isLinked) {
            throw std::runtime_error("Failed to link program");
        }
    }

    void GLProgram::obtainUniforms() {
        GLint count = 0;
        glGetProgramiv(mName, GL_ACTIVE_UNIFORMS, &count);

        GLint textureUnit = 0;
        for (GLuint index = 0; index < count; index++) {
            std::vector<GLchar> uniformNameChars(128);
            GLint size;
            GLenum type;
            glGetActiveUniform(mName, index, static_cast<GLsizei>(uniformNameChars.size()), nullptr, &size, &type, uniformNameChars.data());
            std::string name(uniformNameChars.data());
            GLint location = glGetUniformLocation(mName, &name[0]);

            GLUniform uniform = GLUniform(location, size, type, name);

            if (uniform.isSampler()) {
                if (textureUnit >= GLTextureUnitManager::Shared().maximumTextureUnits()) {
                    throw std::runtime_error(string_format("Exceeded the number of available texture units (%d)", mAvailableTextureUnits));
                }
                glUniform1i(uniform.location(), textureUnit);
                uniform.setTextureUnit(textureUnit);

                textureUnit++;
            }

            uint32_t crc = ctcrc32(name);
            mUniforms.insert(std::make_pair(crc, uniform));
        }
    }

    void GLProgram::swap(GLProgram &that) {
        GLNamedObject::swap(that);
        std::swap(mVertexShader, that.mVertexShader);
        std::swap(mFragmentShader, that.mFragmentShader);
        std::swap(mGeometryShader, that.mGeometryShader);
        std::swap(mUniforms, that.mUniforms);
    }

    void swap(GLProgram &lhs, GLProgram &rhs) {
        lhs.swap(rhs);
    }

    void GLProgram::bind() const {
        glUseProgram(mName);
    }

    const GLVertexAttribute &GLProgram::vertexAttributeByName(const std::string &name) {
        auto it = mVertexAttributes.find(name);
        if (it == mVertexAttributes.end()) {
            throw std::invalid_argument(string_format("Vertex attribute '%s' couldn't be found", name.c_str()));
        }
        return it->second;
    }

    const GLUniform &GLProgram::uniformByNameCRC32(CRC32 crc32) {
        auto it = mUniforms.find(crc32);
        if (it == mUniforms.end()) {
            throw std::invalid_argument("Uniform couldn't be found");
        }
        return it->second;
    }

    void GLProgram::setUniformTexture(CRC32 uniformNameCRC32, const GLTexture &texture, const GLSampler *sampler) {
        const GLUniform& uniform = uniformByNameCRC32(uniformNameCRC32);

        if (!uniform.isSampler()) {
            throw std::runtime_error(string_format("Uniform %s is not a texture", uniform.name().c_str()));
        }

        if (!isModifyingUniforms) {
            throw std::logic_error("You must set texture/sampler uniforms inside a designated closure provided by 'ensureSamplerValidity' member function");
        }

        GLTextureUnitManager::Shared().bindTextureToUnit(texture, uniform.textureUnit());

        if (sampler) {
            GLTextureUnitManager::Shared().bindSamplerToUnit(*sampler, uniform.textureUnit());
        }
    }

    void GLProgram::ensureSamplerValidity(UniformModifierClosure closure) {
        isModifyingUniforms = true;
        GLTextureUnitManager::Shared().unbindAllSamplers();
        closure();

        // For some reason this is sometimes necessary to make GLSL sampler not return Black
        GLTextureUnitManager::Shared().activateUnit(GLTextureUnitManager::Shared().maximumTextureUnits() - 1);
        isModifyingUniforms = false;
    }

	void GLProgram::setUniformMatrix(CRC32 uniformNameCRC32, const glm::mat4 &matrix) {
		glUniformMatrix4fv(uniformByNameCRC32(uniformNameCRC32).location(), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void GLProgram::setUniformVector(CRC32 uniformNameCRC32, const glm::vec2 &vector) {
		glUniform2fv(uniformByNameCRC32(uniformNameCRC32).location(), 1, glm::value_ptr(vector));
	}

	void GLProgram::setUniformVector(CRC32 uniformNameCRC32, const glm::vec3 &vector) {
		glUniform3fv(uniformByNameCRC32(uniformNameCRC32).location(), 1, glm::value_ptr(vector));
	}

	void GLProgram::setUniformVector(CRC32 uniformNameCRC32, const glm::vec4 &vector) {
		glUniform4fv(uniformByNameCRC32(uniformNameCRC32).location(), 1, glm::value_ptr(vector));
	}

	void GLProgram::setUniformFloat(CRC32 uniformNameCRC32, float value) {
		glUniform1f(uniformByNameCRC32(uniformNameCRC32).location(), value);
	}

	void GLProgram::setUniformFloatArray(CRC32 uniformNameCRC32, float *array, size_t count) {
		glUniform1fv(uniformByNameCRC32(uniformNameCRC32).location(), count, array);
	}

	void GLProgram::setUniformInteger(CRC32 uniformNameCRC32, int32_t value) {
		glUniform1i(uniformByNameCRC32(uniformNameCRC32).location(), value);
	}

	void GLProgram::setUniformIntegerArray(CRC32 uniformNameCRC32, int32_t *array, size_t count) {
		glUniform1iv(uniformByNameCRC32(uniformNameCRC32).location(), count, array);
	}

    bool GLProgram::validateState() const {
        GLsizei loglen = 0;
        GLchar logbuffer[1000];
        glValidateProgram(mName);
        glGetProgramInfoLog(mName, sizeof(logbuffer), &loglen, logbuffer);
        if (loglen > 0) {
            printf("OpenGL Program %d is invalid: \n %s\n", mName, logbuffer);
            return false;
        } else {
            printf("OpenGL Program %d is valid \n", mName);
            return true;
        }
    }

}
