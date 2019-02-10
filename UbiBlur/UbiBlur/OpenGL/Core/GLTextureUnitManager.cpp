//
//  GLTextureUnitManager.cpp
//  Engine
//
//  Created by Pavlo Muratov on 21.11.2018.
//  Copyright © 2018 MPO. All rights reserved.
//

#include "GLTextureUnitManager.hpp"
#include "StringUtils.hpp"

namespace Engine {

    GLTextureUnitManager &GLTextureUnitManager::Shared() {
        static GLTextureUnitManager manager;
        return manager;
    }

    GLTextureUnitManager::GLTextureUnitManager() {
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &mMaximumTextureUnits);
    }

    GLTextureUnitManager::TextureUnit GLTextureUnitManager::activeTextureUnit() const {
        return mActiveTextureUnit;
    }

    uint16_t GLTextureUnitManager::maximumTextureUnits() const {
        return mMaximumTextureUnits;
    }

    void GLTextureUnitManager::bindTextureToUnit(const GLTexture &texture, GLTextureUnitManager::TextureUnit unit) {
        if (unit >= mMaximumTextureUnits) {
            throw std::invalid_argument(string_format("Texture unit %d exceeds maximum texture unit index %d", unit, mMaximumTextureUnits - 1));
        }

        if (mActiveTextureUnit != unit) {
            activateUnit(unit);
        }

        bindTextureToActiveUnit(texture);
    }

    void GLTextureUnitManager::bindSamplerToUnit(const GLSampler &sampler, GLTextureUnitManager::TextureUnit unit) {
        if (unit >= mMaximumTextureUnits) {
            throw std::invalid_argument(string_format("Texture unit %d exceeds maximum texture unit index %d", unit, mMaximumTextureUnits - 1));
        }

        auto it = mBoundSamplers.find(unit);
        if (it != mBoundSamplers.end() && it->second == sampler.name()) {
            return;
        }

        glBindSampler(unit, sampler.name());
        mBoundSamplers[unit] = sampler.name();
    }

    void GLTextureUnitManager::activateUnit(TextureUnit unit) {
        glActiveTexture(GL_TEXTURE0 + unit);
        mActiveTextureUnit = unit;
    }

    void GLTextureUnitManager::bindTextureToActiveUnit(const GLTexture &texture) {
        auto boundTextureInfo = mBoundTextures[mActiveTextureUnit];
        auto name = boundTextureInfo.first;
        auto bindingPoint = boundTextureInfo.second;

        if (name == texture.name() && bindingPoint == texture.bindingPoint()) {
            return;
        }

        if (name > 0 && bindingPoint != texture.bindingPoint()) {
            // Unbind previous texture
            glBindTexture(bindingPoint, 0);
        }

        glBindTexture(texture.bindingPoint(), texture.name());
        mBoundTextures[mActiveTextureUnit] = std::make_pair(texture.name(), texture.bindingPoint());
    }

    void GLTextureUnitManager::unbindAllSamplers() {
        for (auto &unitSamplerPair : mBoundSamplers) {
            TextureUnit unit = unitSamplerPair.first;
            glBindSampler(unit, 0);
        }
        mBoundSamplers.clear();
    }

}
