#pragma once

#include <glm.hpp>
#include <memory>
#include "GameObject.h"
#include "Model.h"
#include "Material.h"

class Castle
{
public:
	Castle();

	bool Initialize(Material& matOpaco);
	std::shared_ptr<GameObject> GetCastleObject() const { return castleObj; }

private:
	std::shared_ptr<GameObject> castleObj;

	Model mainRoomModel;
	Model bigTowerModel;
	Model wallModel;
	Model midTowerModel;
	Model floorModel;
};