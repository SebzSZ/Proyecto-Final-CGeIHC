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

	bool Initialize(Material& matOpaco, Material& matBrillante);
	void Update(float deltaTime);

	std::shared_ptr<GameObject> GetMalonObject() const { return malonCuerpoObj; }
	std::shared_ptr<GameObject> GetManoIzquierda() const { return malonManoIzquierdaObj; }

private:
	std::shared_ptr<GameObject> malonCuerpoObj;
	std::shared_ptr<GameObject> malonBrazoIzquierdoObj;
	std::shared_ptr<GameObject> malonAntebrazoIzquierdoObj;
	std::shared_ptr<GameObject> malonManoIzquierdaObj;
	std::shared_ptr<GameObject> nimbus2000Obj;

	Model malonCuerpoModel;
	Model malonBrazoIzquierdoModel;
	Model malonAntebrazoIzquierdoModel;
	Model malonManoIzquierdaModel;
	Model nimbus2000Model;

	std::vector<MalonKeyframe> keyframes;
	int currentKeyframeIndex;
	int nextKeyframeIndex;
	float transitionProgress;
	float transitionSpeed;

	float nimbusTime;

	void SetupKeyframes();
};