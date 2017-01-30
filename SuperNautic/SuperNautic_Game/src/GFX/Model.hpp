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

		protected:
		private:
			VertexArrayObject	_vao;

			// This is explicitly for the ModelLoader to handle.
			VertexArrayObject& getVertexArrayObject();
			friend class ModelLoader;
		};

		Model();
		~Model();


	protected:
	private:
		std::vector<Mesh> _meshes;

		// This is explicitly for the ModelLoader to handle.
		Mesh& addMesh();
		friend class ModelLoader;
	};

}

#endif //MODEL_HPP
