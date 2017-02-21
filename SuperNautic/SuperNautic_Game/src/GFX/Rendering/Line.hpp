#pragma once

#ifndef LINE_HPP
#define LINE_HPP

#include "GFX/Resources/Model.hpp"
#include "GFX/Rendering/Renderable3D.hpp"
#include "GFX/Rendering/Box.hpp"
#include "Core/Geometry/BoundingBox.hpp"


namespace GFX
{


	class Line : public Renderable3D
	{
    public:
        Line();
        Line(const std::vector<glm::vec3>& points);

        void render(RenderStates& states) override;
        void setPoints(const std::vector<glm::vec3>& points);
        void setModelMatrix(const glm::mat4& matrix);

    private:
        size_t _numPoints;
        std::shared_ptr<GFX::Model> _model;
        std::shared_ptr<std::vector<GFX::Box>> _boxes;


        std::shared_ptr<std::vector<GFX::Box>> generateBoxes(const std::vector<glm::vec3>& points) const;
        std::shared_ptr<GFX::Model> generateModel(const std::vector<glm::vec3>& points) const;
	};

}

#endif //LINE_HPP
