#pragma once

#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>

#include "VertexArrayObject.hpp"


namespace GFX
{


	class Model
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

		void render();

	protected:
	private:
		std::vector<std::unique_ptr<Mesh>> _meshes;

		// This is explicitly for the ModelLoader to handle.
		Mesh& addMesh();
		friend class ModelLoader;
	};

}

#endif //MODEL_HPP
