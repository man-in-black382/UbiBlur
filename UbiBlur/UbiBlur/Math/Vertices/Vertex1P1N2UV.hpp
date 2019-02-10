//
//  Vertex1P1N1UV.hpp
//  Engine
//
//  Created by Pavlo Muratov on 07.03.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#ifndef Vertex1P1N1UV_hpp
#define Vertex1P1N1UV_hpp

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Engine {

    /**
     1 position
     1 normal
     1 texture coordinate
     1 lightmap uv
     */
    struct Vertex1P1N2UV {
        glm::vec4 position;
        glm::vec3 textureCoords;
        glm::vec2 lightmapCoords;
        glm::vec3 normal;

        Vertex1P1N2UV(const glm::vec4 &position,
                const glm::vec3 &texCoords,
                const glm::vec2 &lightmapCoords,
                const glm::vec3 &normal);
    };

}

#endif /* Vertex1P1N1UV_hpp */
