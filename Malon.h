#pragma once

#include <glm.hpp>
#include <memory>
#include <vector>
#include "GameObject.h"
#include "Model.h"
#include "Material.h"

struct MalonKeyframe
{
	glm::vec3 rotBrazoIzquierdo;
	glm::vec3 rotAntebrazoIzquierdo;
	glm::vec3 rotManoIzquierda;
};

class Malon
{
public:
	Malon();

	bool Initialize(Material& matOpaco);
	void Update(float deltaTime);

	std::shared_ptr<GameObject> GetMalonObject() const { return malonCuerpoObj; }

private:
	std::shared_ptr<GameObject> malonCuerpoObj;
	std::shared_ptr<GameObject> malonBrazoIzquierdoObj;
	std::shared_ptr<GameObject> malonAntebrazoIzquierdoObj;
	std::shared_ptr<GameObject> malonManoIzquierdaObj;

	Model malonCuerpoModel;
	Model malonBrazoIzquierdoModel;
	Model malonAntebrazoIzquierdoModel;
	Model malonManoIzquierdaModel;

	std::vector<MalonKeyframe> keyframes;
	int currentKeyframeIndex;
	int nextKeyframeIndex;
	float transitionProgress;
	float transitionSpeed;

	void SetupKeyframes();
};