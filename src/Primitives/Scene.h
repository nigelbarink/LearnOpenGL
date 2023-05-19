#pragma once
#include <vector>
#include "../model.h"
#include "camera.h"
#include "Skybox.h"

class Scene {
public:

	Camera MainCamera;
	Skybox skybox;
	std::vector <Model> entities;
//	std::vector<Material> materials;


};