#include "Train.h"
#include <glm.hpp>
#include <cmath>
#include <string>

const glm::vec3 Train::TRACK_CENTER = glm::vec3(0.0f, 0.0f, 20.0f);

Train::Train() : distanceTraveled(0.0f) {}

bool Train::Initialize(Material& matOpaco)
{
	if (!bigCarModel.load("Models/Train_BigCar.obj")) return false;
	if (!smallCarModel.load("Models/Train_SmallCar.obj")) return false;
	if (!bigWheelModel.load("Models/Train_BigWheel.obj")) return false;
	if (!smallWheelModel.load("Models/Train_SmallWheel.obj")) return false;

	trainObj = std::make_shared<GameObject>("Train Container", GameObjectType::MODEL);

	engineCar = std::make_shared<GameObject>("Big Car", GameObjectType::MODEL);
	engineCar->setModel(&bigCarModel);
	engineCar->setMaterial(&matOpaco);
	engineCar->transform.setPosition(0.0f, 1.25f, 0.0f);
	engineCar->transform.setScale(1.0f);
	trainObj->addChild(engineCar);

	// Ruedas grandes 
	std::shared_ptr<GameObject> bigWheel1 = std::make_shared<GameObject>("Big Wheel 1", GameObjectType::MODEL);
	bigWheel1->setModel(&bigWheelModel); bigWheel1->setMaterial(&matOpaco);
	bigWheel1->transform.setPosition(-0.6f, -0.82f, 0.75f); bigWheel1->transform.setScale(1.0f);
	engineCar->addChild(bigWheel1); wheels.push_back(bigWheel1);

	std::shared_ptr<GameObject> bigWheel2 = std::make_shared<GameObject>("Big Wheel 2", GameObjectType::MODEL);
	bigWheel2->setModel(&bigWheelModel); bigWheel2->setMaterial(&matOpaco);
	bigWheel2->transform.setPosition(0.6f, -0.82f, 0.75f); bigWheel2->transform.setRotation(0.0f, 180.0f, 0.0f); bigWheel2->transform.setScale(1.0f);
	engineCar->addChild(bigWheel2); wheels.push_back(bigWheel2);

	// Ruedas pequeñas 
	std::shared_ptr<GameObject> smallWheel1 = std::make_shared<GameObject>("Small Wheel 1", GameObjectType::MODEL);
	smallWheel1->setModel(&smallWheelModel); smallWheel1->setMaterial(&matOpaco);
	smallWheel1->transform.setPosition(-0.6f, -0.95f, -0.25f); smallWheel1->transform.setScale(1.0f);
	engineCar->addChild(smallWheel1); wheels.push_back(smallWheel1);

	std::shared_ptr<GameObject> smallWheel2 = std::make_shared<GameObject>("Small Wheel 2", GameObjectType::MODEL);
	smallWheel2->setModel(&smallWheelModel); smallWheel2->setMaterial(&matOpaco);
	smallWheel2->transform.setPosition(0.6f, -0.95f, -0.25f); smallWheel2->transform.setRotation(0.0f, 180.0f, 0.0f); smallWheel2->transform.setScale(1.0f);
	engineCar->addChild(smallWheel2); wheels.push_back(smallWheel2);

	std::shared_ptr<GameObject> smallWheel3 = std::make_shared<GameObject>("Small Wheel 3", GameObjectType::MODEL);
	smallWheel3->setModel(&smallWheelModel); smallWheel3->setMaterial(&matOpaco);
	smallWheel3->transform.setPosition(-0.6f, -0.95f, -1.20f); smallWheel3->transform.setScale(1.0f);
	engineCar->addChild(smallWheel3); wheels.push_back(smallWheel3);

	std::shared_ptr<GameObject> smallWheel4 = std::make_shared<GameObject>("Small Wheel 4", GameObjectType::MODEL);
	smallWheel4->setModel(&smallWheelModel); smallWheel4->setMaterial(&matOpaco);
	smallWheel4->transform.setPosition(0.6f, -0.95f, -1.20f); smallWheel4->transform.setRotation(0.0f, 180.0f, 0.0f); smallWheel4->transform.setScale(1.0f);
	engineCar->addChild(smallWheel4); wheels.push_back(smallWheel4);

	int numberOfWagons = 4;

	for (int i = 0; i < numberOfWagons; ++i)
	{
		CreateWagon(i, matOpaco);
	}

	return true;
}

void Train::CreateWagon(int i, Material& matOpaco)
{
	std::string wagonName = "Wagon " + std::to_string(i + 1);
	std::shared_ptr<GameObject> wagonObj = std::make_shared<GameObject>(wagonName, GameObjectType::MODEL);
	wagonObj->setModel(&smallCarModel);
	wagonObj->setMaterial(&matOpaco);
	wagonObj->transform.setPosition(0.0f, 1.0f, 0.0f);
	wagonObj->transform.setScale(1.0f);
	trainObj->addChild(wagonObj);
	wagons.push_back(wagonObj);

	std::shared_ptr<GameObject> w1 = std::make_shared<GameObject>(wagonName + " Wheel 1", GameObjectType::MODEL);
	w1->setModel(&smallWheelModel); w1->setMaterial(&matOpaco);
	w1->transform.setPosition(-0.6f, -0.71f, -0.7f); w1->transform.setScale(1.0f);
	wagonObj->addChild(w1); wheels.push_back(w1);

	std::shared_ptr<GameObject> w2 = std::make_shared<GameObject>(wagonName + " Wheel 2", GameObjectType::MODEL);
	w2->setModel(&smallWheelModel); w2->setMaterial(&matOpaco);
	w2->transform.setPosition(0.6f, -0.71f, -0.7f); w2->transform.setRotation(0.0f, 180.0f, 0.0f); w2->transform.setScale(1.0f);
	wagonObj->addChild(w2); wheels.push_back(w2);

	std::shared_ptr<GameObject> w3 = std::make_shared<GameObject>(wagonName + " Wheel 3", GameObjectType::MODEL);
	w3->setModel(&smallWheelModel); w3->setMaterial(&matOpaco);
	w3->transform.setPosition(-0.6f, -0.71f, 0.35f); w3->transform.setScale(1.0f);
	wagonObj->addChild(w3); wheels.push_back(w3);

	std::shared_ptr<GameObject> w4 = std::make_shared<GameObject>(wagonName + " Wheel 4", GameObjectType::MODEL);
	w4->setModel(&smallWheelModel); w4->setMaterial(&matOpaco);
	w4->transform.setPosition(0.6f, -0.71f, 0.35f); w4->transform.setRotation(0.0f, 180.0f, 0.0f); w4->transform.setScale(1.0f);
	wagonObj->addChild(w4); wheels.push_back(w4);
}

glm::vec3 Train::GetTrackPosition(float d) const
{
	float L = TRACK_STRAIGHT_LEN;
	float R = TRACK_CURVE_RADIUS;
	float curveLen = glm::pi<float>() * R / 2.0f;
	float perimeter = 4.0f * L + 4.0f * curveLen;

	d = fmod(d, perimeter);

	if (d < 0)
	{
		d += perimeter;
	}

	float newX = 0.0f;
	float newZ = 0.0f;

	if (d < L)
	{
		newX = -L / 2.0f + d;
		newZ = L / 2.0f + R;
	}
	else if (d < L + curveLen)
	{
		float localD = d - L;
		float theta = glm::pi<float>() / 2.0f - (localD / R);
		newX = L / 2.0f + R * cos(theta);
		newZ = L / 2.0f + R * sin(theta);
	}
	else if (d < 2 * L + curveLen)
	{
		float localD = d - (L + curveLen);
		newX = L / 2.0f + R;
		newZ = L / 2.0f - localD;
	}
	else if (d < 2 * L + 2 * curveLen)
	{
		float localD = d - (2 * L + curveLen);
		float theta = 0.0f - (localD / R);
		newX = L / 2.0f + R * cos(theta);
		newZ = -L / 2.0f + R * sin(theta);
	}
	else if (d < 3 * L + 2 * curveLen)
	{
		float localD = d - (2 * L + 2 * curveLen);
		newX = L / 2.0f - localD;
		newZ = -L / 2.0f - R;
	}
	else if (d < 3 * L + 3 * curveLen)
	{
		float localD = d - (3 * L + 2 * curveLen);
		float theta = -glm::pi<float>() / 2.0f - (localD / R);
		newX = -L / 2.0f + R * cos(theta);
		newZ = -L / 2.0f + R * sin(theta);
	}
	else if (d < 4 * L + 3 * curveLen)
	{
		float localD = d - (3 * L + 3 * curveLen);
		newX = -L / 2.0f - R;
		newZ = -L / 2.0f + localD;
	}
	else
	{
		float localD = d - (4 * L + 3 * curveLen);
		float theta = -glm::pi<float>() - (localD / R);
		newX = -L / 2.0f + R * cos(theta);
		newZ = L / 2.0f + R * sin(theta);
	}

	return glm::vec3(newX, 0.0f, newZ);
}

bool Train::IsInCurve(float d) const
{
	float L = TRACK_STRAIGHT_LEN;
	float R = TRACK_CURVE_RADIUS;
	float curveLen = glm::pi<float>() * R / 2.0f;
	float perimeter = 4.0f * L + 4.0f * curveLen;

	d = fmod(d, perimeter);
	if (d < 0) d += perimeter;

	return (d >= L && d < L + curveLen) ||
		(d >= 2 * L + curveLen && d < 2 * L + 2 * curveLen) ||
		(d >= 3 * L + 2 * curveLen && d < 3 * L + 3 * curveLen) ||
		(d >= 4 * L + 3 * curveLen);
}

void Train::Update(float trainSpeed, float deltaTime, float wheelRotationSpeed)
{
	bool inCurve = IsInCurve(distanceTraveled);
	float currentSpeed = inCurve ? (trainSpeed * 0.5f) : trainSpeed;
	float currentWheelSpeed = inCurve ? (wheelRotationSpeed * 0.5f) : wheelRotationSpeed;

	distanceTraveled += currentSpeed * deltaTime;

	float perimeter = 4.0f * TRACK_STRAIGHT_LEN + 4.0f * (glm::pi<float>() * TRACK_CURVE_RADIUS / 2.0f);

	glm::vec3 engineLocalPos = GetTrackPosition(distanceTraveled);
	glm::vec3 engineNextPos = GetTrackPosition(distanceTraveled + 0.1f);
	glm::vec3 engineDir = engineNextPos - engineLocalPos;

	engineCar->transform.setPosition(TRACK_CENTER.x + engineLocalPos.x, 1.25f, TRACK_CENTER.z + engineLocalPos.z);
	float engineYaw = glm::degrees(atan2(engineDir.x, engineDir.z));
	engineCar->transform.setRotation(0.0f, engineYaw + 180.0f, 0.0f);

	for (size_t i = 0; i < wagons.size(); ++i)
	{
		float wagonDistance = distanceTraveled - FIRST_WAGON_OFFSET - (WAGON_SPACING * i);

		wagonDistance = fmod(wagonDistance, perimeter);
		if (wagonDistance < 0) wagonDistance += perimeter;

		glm::vec3 wagonLocalPos = GetTrackPosition(wagonDistance);
		glm::vec3 wagonNextPos = GetTrackPosition(wagonDistance + 0.1f);
		glm::vec3 wagonDir = wagonNextPos - wagonLocalPos;

		wagons[i]->transform.setPosition(TRACK_CENTER.x + wagonLocalPos.x, 1.0f, TRACK_CENTER.z + wagonLocalPos.z);

		float wagonYaw = glm::degrees(atan2(wagonDir.x, wagonDir.z));
		wagons[i]->transform.setRotation(0.0f, wagonYaw + 180.0f, 0.0f);
	}

	for (auto& wheel : wheels)
	{
		wheel->transform.rotate(-currentWheelSpeed * deltaTime, 0.0f, 0.0f);
	}
}