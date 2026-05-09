#pragma once

#include <glm.hpp>
#include <memory>
#include <vector>
#include "GameObject.h"
#include "Model.h"
#include "Material.h"

class Train
{
public:
	Train();

	bool Initialize(Material& matOpaco);
	std::shared_ptr<GameObject> GetTrainObject() const { return trainObj; }
	void Update(float trainSpeed, float deltaTime, float wheelRotationSpeed);

private:
	static constexpr float TRACK_STRAIGHT_LEN = 18.0f;
	static constexpr float TRACK_CURVE_RADIUS = 6.0f;
	static constexpr float WAGON_SPACING = 2.4f;
	static constexpr float FIRST_WAGON_OFFSET = 2.8f;
	static const glm::vec3 TRACK_CENTER;

	std::shared_ptr<GameObject> trainObj;
	std::shared_ptr<GameObject> engineCar;
	std::vector<std::shared_ptr<GameObject>> wagons;
	std::vector<std::shared_ptr<GameObject>> wheels;
	float distanceTraveled;

	Model bigCarModel;
	Model smallCarModel;
	Model bigWheelModel;
	Model smallWheelModel;

	void CreateWagon(int i, Material& matOpaco);
	glm::vec3 GetTrackPosition(float d) const;
	bool IsInCurve(float d) const;
};