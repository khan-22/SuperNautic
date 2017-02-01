#pragma once

#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include <string>

#include "Model.hpp"


namespace GFX {
	
	struct RawMeshCollection; // Forward-decl

	class ModelLoader
	{
	public:
		ModelLoader(/*const std::string& rootPath*/);
		~ModelLoader();

		Model* loadModel(std::string filePath);


	protected:
	


	private:
		//std::string	_rootPath;

		struct Grouping;
		std::vector<Grouping> generateGroupings(RawMeshCollection* rawModel);
	};



}

#endif //MODEL_LOADER_HPP