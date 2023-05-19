#pragma once
#include <vector>
#include "../model.h"
#include "camera.h"

class Scene {
public:

	Camera cameras;
	std::vector <Model> entities;
//	std::vector<Material> materials;


};