#include "Castle.h"
#include "PointLight.h"

Castle::Castle() : castleObj(nullptr)
{}

bool Castle::Initialize(Material& matOpaco, PointLight* pointLights, unsigned int& pointLightCount)
{
	if (!mainRoomModel.load("Models/HyruleCastle_MainRoom.obj")) return false;
	if (!bigTowerModel.load("Models/HyruleCastle_BigTower.obj")) return false;
	if (!wallModel.load("Models/HyruleCastle_Wall.obj")) return false;
	if (!midTowerModel.load("Models/HyruleCastle_MidTower.obj")) return false;
	if (!floorModel.load("Models/HyruleCastle_Floor.obj")) return false;
	if (!torchModel.load("Models/Torch.obj")) return false;

	// Container
	castleObj = std::make_shared<GameObject>("Castle Container", GameObjectType::MODEL);

	// Main Room
	std::shared_ptr<GameObject> mainRoomObj = std::make_shared<GameObject>("Main Room", GameObjectType::MODEL);
	mainRoomObj->setModel(&mainRoomModel); // Usamos el modelo interno
	mainRoomObj->setMaterial(&matOpaco);
	mainRoomObj->transform.setPosition(20.0f, 0.0f, 1.64f);
	mainRoomObj->transform.setRotation(0.0f, 180.0f, 0.0f);
	mainRoomObj->transform.setScale(120.0f);
	castleObj->addChild(mainRoomObj);

	// Wall 1
	std::shared_ptr<GameObject> wall1Obj = std::make_shared<GameObject>("Wall 1", GameObjectType::MODEL);
	wall1Obj->setModel(&wallModel);
	wall1Obj->setMaterial(&matOpaco);
	wall1Obj->transform.setPosition(6.9f, 0.0f, 0.0f);
	wall1Obj->transform.setRotation(0.0f, -90.0f, 0.0f);
	wall1Obj->transform.setScale(60.0f);
	castleObj->addChild(wall1Obj);

	// Wall 2
	std::shared_ptr<GameObject> wall2Obj = std::make_shared<GameObject>("Wall 2", GameObjectType::MODEL);
	wall2Obj->setModel(&wallModel);
	wall2Obj->setMaterial(&matOpaco);
	wall2Obj->transform.setPosition(33.21f, 0.0f, 0.0f);
	wall2Obj->transform.setRotation(0.0f, -90.0f, 0.0f);
	wall2Obj->transform.setScale(60.0f);
	castleObj->addChild(wall2Obj);

	// Wall 3
	std::shared_ptr<GameObject> wall3Obj = std::make_shared<GameObject>("Wall 3", GameObjectType::MODEL);
	wall3Obj->setModel(&wallModel);
	wall3Obj->setMaterial(&matOpaco);
	wall3Obj->transform.setPosition(40.9f, 0.0f, -7.55f);
	wall3Obj->transform.setRotation(0.0f, 0.0f, 0.0f);
	wall3Obj->transform.setScale(60.0f);
	castleObj->addChild(wall3Obj);

	// Wall 4
	std::shared_ptr<GameObject> wall4Obj = std::make_shared<GameObject>("Wall 4", GameObjectType::MODEL);
	wall4Obj->setModel(&wallModel);
	wall4Obj->setMaterial(&matOpaco);
	wall4Obj->transform.setPosition(20.27f, 0.0f, -15.21f);
	wall4Obj->transform.setRotation(0.0f, 90.0f, 0.0f);
	wall4Obj->transform.setScale(glm::vec3(60.0f, 60.0f, 165.0f));
	castleObj->addChild(wall4Obj);

	// Wall 5
	std::shared_ptr<GameObject> wall5Obj = std::make_shared<GameObject>("Wall 5", GameObjectType::MODEL);
	wall5Obj->setModel(&wallModel);
	wall5Obj->setMaterial(&matOpaco);
	wall5Obj->transform.setPosition(-0.73f, 0.0f, -7.66f);
	wall5Obj->transform.setRotation(0.0f, 180.0f, 0.0f);
	wall5Obj->transform.setScale(60.0f);
	castleObj->addChild(wall5Obj);

	// Wall 6
	std::shared_ptr<GameObject> wall6Obj = std::make_shared<GameObject>("Wall 6", GameObjectType::MODEL);
	wall6Obj->setModel(&wallModel);
	wall6Obj->setMaterial(&matOpaco);
	wall6Obj->transform.setPosition(20.0f, 3.96f, -6.58f);
	wall6Obj->transform.setRotation(0.0f, -90.0f, 0.0f);
	wall6Obj->transform.setScale(60.0f);
	castleObj->addChild(wall6Obj);

	// Wall 7
	std::shared_ptr<GameObject> wall7Obj = std::make_shared<GameObject>("Wall 7", GameObjectType::MODEL);
	wall7Obj->setModel(&wallModel);
	wall7Obj->setMaterial(&matOpaco);
	wall7Obj->transform.setPosition(20.1f, 3.96f, -12.91f);
	wall7Obj->transform.setRotation(0.0f, 90.0f, 0.0f);
	wall7Obj->transform.setScale(60.0f);
	castleObj->addChild(wall7Obj);

	// Wall 8
	std::shared_ptr<GameObject> wall8Obj = std::make_shared<GameObject>("Wall 8", GameObjectType::MODEL);
	wall8Obj->setModel(&wallModel);
	wall8Obj->setMaterial(&matOpaco);
	wall8Obj->transform.setPosition(27.7f, 3.96f, -9.8f);
	wall8Obj->transform.setRotation(0.0f, 0.0f, 0.0f);
	wall8Obj->transform.setScale(glm::vec3(60.0f, 60.0f, 24.89f));
	castleObj->addChild(wall8Obj);

	// Wall 9
	std::shared_ptr<GameObject> wall9Obj = std::make_shared<GameObject>("Wall 9", GameObjectType::MODEL);
	wall9Obj->setModel(&wallModel);
	wall9Obj->setMaterial(&matOpaco);
	wall9Obj->transform.setPosition(12.4f, 3.96f, -9.8f);
	wall9Obj->transform.setRotation(0.0f, 180.0f, 0.0f);
	wall9Obj->transform.setScale(glm::vec3(60.0f, 60.0f, 24.89f));
	castleObj->addChild(wall9Obj);

	// Big Tower 1
	std::shared_ptr<GameObject> bigTower1Obj = std::make_shared<GameObject>("Big Tower 1", GameObjectType::MODEL);
	bigTower1Obj->setModel(&bigTowerModel);
	bigTower1Obj->setMaterial(&matOpaco);
	bigTower1Obj->transform.setPosition(16.84f, 8.92f, -9.0f);
	bigTower1Obj->transform.setScale(60.0f);
	castleObj->addChild(bigTower1Obj);

	// Big Tower 2
	std::shared_ptr<GameObject> bigTower2Obj = std::make_shared<GameObject>("Big Tower 2", GameObjectType::MODEL);
	bigTower2Obj->setModel(&bigTowerModel);
	bigTower2Obj->setMaterial(&matOpaco);
	bigTower2Obj->transform.setPosition(24.65f, 0.53f, -9.47f);
	bigTower2Obj->transform.setScale(60.0f);
	castleObj->addChild(bigTower2Obj);

	// Mid Tower 1
	std::shared_ptr<GameObject> midTower1Obj = std::make_shared<GameObject>("Mid Tower 1", GameObjectType::MODEL);
	midTower1Obj->setModel(&midTowerModel);
	midTower1Obj->setMaterial(&matOpaco);
	midTower1Obj->transform.setPosition(36.09f, 5.44f, -3.64f);
	midTower1Obj->transform.setRotation(0.0f, -90.0f, 0.0f);
	midTower1Obj->transform.setScale(50.0f);
	castleObj->addChild(midTower1Obj);

	// Mid Tower 2
	std::shared_ptr<GameObject> midTower2Obj = std::make_shared<GameObject>("Mid Tower 2", GameObjectType::MODEL);
	midTower2Obj->setModel(&midTowerModel);
	midTower2Obj->setMaterial(&matOpaco);
	midTower2Obj->transform.setPosition(36.09f, 5.44f, -11.0f);
	midTower2Obj->transform.setRotation(0.0f, -90.0f, 0.0f);
	midTower2Obj->transform.setScale(50.0f);
	castleObj->addChild(midTower2Obj);

	// Mid Tower 3
	std::shared_ptr<GameObject> midTower3Obj = std::make_shared<GameObject>("Mid Tower 3", GameObjectType::MODEL);
	midTower3Obj->setModel(&midTowerModel);
	midTower3Obj->setMaterial(&matOpaco);
	midTower3Obj->transform.setPosition(3.69f, 5.44f, -3.64f);
	midTower3Obj->transform.setRotation(0.0f, -90.0f, 0.0f);
	midTower3Obj->transform.setScale(50.0f);
	castleObj->addChild(midTower3Obj);

	// Mid Tower 4
	std::shared_ptr<GameObject> midTower4Obj = std::make_shared<GameObject>("Mid Tower 4", GameObjectType::MODEL);
	midTower4Obj->setModel(&midTowerModel);
	midTower4Obj->setMaterial(&matOpaco);
	midTower4Obj->transform.setPosition(3.69f, 5.44f, -11.0f);
	midTower4Obj->transform.setRotation(0.0f, -90.0f, 0.0f);
	midTower4Obj->transform.setScale(50.0f);
	castleObj->addChild(midTower4Obj);

	// Floor 1
	std::shared_ptr<GameObject> floor1Obj = std::make_shared<GameObject>("Floor 1", GameObjectType::MODEL);
	floor1Obj->setModel(&floorModel);
	floor1Obj->setMaterial(&matOpaco);
	floor1Obj->transform.setPosition(33.59f, 5.28f, -8.34f);
	floor1Obj->transform.setScale(58.3f, 1.0f, 76.2f);
	castleObj->addChild(floor1Obj);

	// Floor 2
	std::shared_ptr<GameObject> floor2Obj = std::make_shared<GameObject>("Floor 2", GameObjectType::MODEL);
	floor2Obj->setModel(&floorModel);
	floor2Obj->setMaterial(&matOpaco);
	floor2Obj->transform.setPosition(6.43f, 5.28f, -8.34f);
	floor2Obj->transform.setScale(58.3f, 1.0f, 76.2f);
	castleObj->addChild(floor2Obj);

	// Floor 3
	std::shared_ptr<GameObject> floor3Obj = std::make_shared<GameObject>("Floor 3", GameObjectType::MODEL);
	floor3Obj->setModel(&floorModel);
	floor3Obj->setMaterial(&matOpaco);
	floor3Obj->transform.setPosition(20.0f, 5.28f, -10.2f);
	floor3Obj->transform.setScale(50.1f, 1.0f, 55.15f);
	castleObj->addChild(floor3Obj);

	// Floor 4
	std::shared_ptr<GameObject> floor4Obj = std::make_shared<GameObject>("Floor 4", GameObjectType::MODEL);
	floor4Obj->setModel(&floorModel);
	floor4Obj->setMaterial(&matOpaco);
	floor4Obj->transform.setPosition(19.75f, 9.28f, -10.0f);
	floor4Obj->transform.setScale(60.0f, 1.0f, 31.1f);
	castleObj->addChild(floor4Obj);

	CreateTorches(matOpaco, pointLights, pointLightCount);

	return true;
}

void Castle::CreateTorches(Material& matOpaco, PointLight* pointLights, unsigned int& pointLightCount)
{
	std::shared_ptr<GameObject> container = std::make_shared<GameObject>("Torches", GameObjectType::MODEL);

	std::shared_ptr<GameObject> torchObj1 = std::make_shared<GameObject>("Torch", GameObjectType::MODEL);
	torchObj1->setModel(&torchModel);
	torchObj1->setMaterial(&matOpaco);
	torchObj1->transform.setPosition(6.38f, 2.85f, 0.03f);
	torchObj1->transform.setRotation(0.0f, 90.0f, 0.0f);
	torchObj1->transform.setScale(0.5f);

	std::shared_ptr<GameObject> lightObj1 = std::make_shared<GameObject>("TorchLight1", GameObjectType::POINT_LIGHT);
	pointLights[pointLightCount] = PointLight(1.0f, 0.5f, 0.0f, 0.2f, 1.0f, 0.0f, 1.15f, 0.0f, 1.0f, 0.7f, 1.8f);
	lightObj1->setPointLight(&pointLights[pointLightCount++]);
	lightObj1->transform.setPosition(0.0f, 1.15f, 0.0f);
	torchObj1->addChild(lightObj1);

	container->addChild(torchObj1);

	std::shared_ptr<GameObject> torchObj2 = std::make_shared<GameObject>("Torch", GameObjectType::MODEL);
	torchObj2->setModel(&torchModel);
	torchObj2->setMaterial(&matOpaco);
	torchObj2->transform.setPosition(32.9f, 2.85f, 0.03f);
	torchObj2->transform.setRotation(0.0f, 90.0f, 0.0f);
	torchObj2->transform.setScale(0.5f);

	std::shared_ptr<GameObject> lightObj2 = std::make_shared<GameObject>("TorchLight2", GameObjectType::POINT_LIGHT);
	pointLights[pointLightCount] = PointLight(1.0f, 0.5f, 0.0f, 0.2f, 1.0f, 0.0f, 1.15f, 0.0f, 1.0f, 0.7f, 1.8f);
	lightObj2->setPointLight(&pointLights[pointLightCount++]);
	lightObj2->transform.setPosition(0.0f, 1.15f, 0.0f);
	torchObj2->addChild(lightObj2);

	container->addChild(torchObj2);

	std::shared_ptr<GameObject> torchObj3 = std::make_shared<GameObject>("Torch", GameObjectType::MODEL);
	torchObj3->setModel(&torchModel);
	torchObj3->setMaterial(&matOpaco);
	torchObj3->transform.setPosition(24.0f, 1.85f, 5.57f);
	torchObj3->transform.setRotation(0.0f, 135.0f, 0.0f);
	torchObj3->transform.setScale(0.5f);

	std::shared_ptr<GameObject> lightObj3 = std::make_shared<GameObject>("TorchLight3", GameObjectType::POINT_LIGHT);
	pointLights[pointLightCount] = PointLight(1.0f, 0.5f, 0.0f, 0.2f, 1.0f, 0.0f, 1.15f, 0.0f, 1.0f, 0.7f, 1.8f);
	lightObj3->setPointLight(&pointLights[pointLightCount++]);
	lightObj3->transform.setPosition(0.0f, 1.15f, 0.0f);
	torchObj3->addChild(lightObj3);

	container->addChild(torchObj3);

	std::shared_ptr<GameObject> torchObj4 = std::make_shared<GameObject>("Torch", GameObjectType::MODEL);
	torchObj4->setModel(&torchModel);
	torchObj4->setMaterial(&matOpaco);
	torchObj4->transform.setPosition(16.06f, 1.85f, 5.57f);
	torchObj4->transform.setRotation(0.0f, 47.21f, 0.0f);
	torchObj4->transform.setScale(0.5f);

	std::shared_ptr<GameObject> lightObj4 = std::make_shared<GameObject>("TorchLight4", GameObjectType::POINT_LIGHT);
	pointLights[pointLightCount] = PointLight(1.0f, 0.5f, 0.0f, 0.2f, 1.0f, 0.0f, 1.15f, 0.0f, 1.0f, 0.7f, 1.8f);
	lightObj4->setPointLight(&pointLights[pointLightCount++]);
	lightObj4->transform.setPosition(0.0f, 1.15f, 0.0f);
	torchObj4->addChild(lightObj4);

	container->addChild(torchObj4);

	castleObj->addChild(container);
}