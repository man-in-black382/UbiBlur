//
//  GLTextureUnitManager.hpp
//  Engine
//
//  Created by Pavlo Muratov on 21.11.2018.
//  Copyright © 2018 MPO. All rights reserved.
//

#ifndef GLTextureUnitManager_hpp
#define GLTextureUnitManager_hpp

#include <unordered_map>

#include "GLTexture.hpp"
#include "GLSampler.hpp"

namespace Engine {

    class GLTextureUnitManager {
    public:
        using TextureUnit = uint16_t;
        using ObjectName = GLint;
        using BindingPoint = GLenum;

    private:
        std::unordered_map<TextureUnit, std::pair<ObjectName, BindingPoint>> mBoundTextures;
        std::unordered_map<ObjectName, std::pair<TextureUnit, BindingPoint>> mBoundTexturesReverse;
        std::unordered_map<TextureUnit, ObjectName> mBoundSamplers;

        GLint mMaximumTextureUnits = 0;
        TextureUnit mActiveTextureUnit = 0;

        GLTextureUnitManager();

        GLTextureUnitManager(const GLTextureUnitManager &that) = delete;

        GLTextureUnitManager &operator=(const GLTextureUnitManager &rhs) = delete;

    public:
        static GLTextureUnitManager &Shared();

        TextureUnit activeTextureUnit() const;

        uint16_t maximumTextureUnits() const;

        void bindTextureToUnit(const GLTexture &texture, TextureUnit unit);

        void bindSamplerToUnit(const GLSampler &sampler, TextureUnit unit);

        void activateUnit(TextureUnit unit);

        void bindTextureToActiveUnit(const GLTexture &texture);

        void unbindAllSamplers();
    };

}

#endif /* GLTextureUnitManager_hpp */
