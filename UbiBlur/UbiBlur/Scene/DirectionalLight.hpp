//
//  DirectionalLight.hpp
//  Engine
//
//  Created by Pavlo Muratov on 30.04.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#ifndef DirectionalLight_hpp
#define DirectionalLight_hpp

#include "Light.hpp"
#include "Camera.hpp"

#include <vector>

namespace Engine {

    class DirectionalLight : public Light {
    protected:
        glm::vec3 mDirection;

    public:
        DirectionalLight(const Color &color, const glm::vec3 &direction, float area, float shadowBias);

        glm::mat4 viewMatrix() const;

        const glm::vec3 &direction() const;

        void setDirection(const glm::vec3 &direction);
    };

}

#endif /* DirectionalLight_hpp */
