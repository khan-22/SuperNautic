#pragma once

#ifndef LINE_HPP
#define LINE_HPP

#include "GFX/Resources/Model.hpp"
#include "GFX/Rendering/Renderable3D.hpp"
#include "GFX/Rendering/Box.hpp"
#include "Core/Geometry/BoundingBox.hpp"

#include "Core/Asset/LoadAssetFunctions.hpp"

namespace GFX
{


	class Line : public Renderable3D
	{
    public:
        Line();
        Line(const std::vector<glm::vec3>& points);

        void render(RenderStates& states) override;
        void setPoints(const std::vector<glm::vec3>& points);

        void move(const glm::vec2& xy);
        void rotate(const glm::vec3& xyz);
        void zoom(float factor);

    private:
        size_t _numPoints;
        std::shared_ptr<GFX::VertexArrayObject> _vao;
        ShaderAsset _shader;

        glm::vec3 _centerTranslation;
        glm::vec2 _screenTranslation;
        glm::mat4 _rotation;
        glm::vec3 _scale;

        float _radius;
        float _zoom;


        glm::mat4 _modelMatrix;
        glm::mat4 _screenPerspectiveMatrix;


        std::shared_ptr<GFX::VertexArrayObject> generateVao(const std::vector<glm::vec3>& points) const;
        void updateModelMatrix();
        void updateScreenPerspectiveMatrix();
	};

}

#endif //LINE_HPP
