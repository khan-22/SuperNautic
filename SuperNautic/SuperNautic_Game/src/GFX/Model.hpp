#pragma once

#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>

#include "VertexArrayObject.hpp"
#include "Renderable3D.hpp"

#include <glm/gtx/transform.hpp>

namespace GFX
{


	class Model : public Renderable3D
	{
	public:
		class Mesh
		{
		public:
			Mesh();
			~Mesh();

			void render();

		protected:
		private:
			VertexArrayObject	_vao;

			// This is explicitly for the ModelLoader to handle.
			VertexArrayObject& getVertexArrayObject();
			friend class ModelLoader;
		};

		Model();
		~Model();

		size_t getNumMeshes() const;

		//Temporary way of setting model matrix
		glm::mat4 _tempModelMat = glm::mat4();
		void setModelMatrix(glm::mat4 ModelMatrix);
		///////////////////////////////////////////

	protected:
		void render(RenderStates& states) override;

	private:
		std::vector<std::unique_ptr<Mesh>> _meshes;

		// This is explicitly for the ModelLoader to handle.
		Mesh& addMesh();
		void setAttributes(RenderStates& states) const;
		friend class ModelLoader;
		friend class TexturedModel;
	};

}

#endif //MODEL_HPP
