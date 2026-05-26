#include "Malon.h"

Malon::Malon() : currentKeyframeIndex(0), nextKeyframeIndex(1), transitionProgress(0.0f), transitionSpeed(1.5f), nimbusTime(0.0f)
{
}

bool Malon::Initialize(Material& matOpaco, Material& matBrillante)
{
	if (!malonCuerpoModel.load("Models/Malon_Cuerpo.obj")) return false;
	malonCuerpoObj = std::make_shared<GameObject>("Malon Cuerpo", GameObjectType::MODEL);
	malonCuerpoObj->setModel(&malonCuerpoModel);
	malonCuerpoObj->setMaterial(&matOpaco);
	malonCuerpoObj->transform.setPosition(-22.48f, 0.0f, 18.0f);
	malonCuerpoObj->transform.setRotation(0.0f, 47.35f, 0.0f);
	malonCuerpoObj->transform.setScale(0.01f);

	if (!malonBrazoIzquierdoModel.load("Models/Malon_BrazoIzquierdo.obj")) return false;
	malonBrazoIzquierdoObj = std::make_shared<GameObject>("Malon Brazo Izquierdo", GameObjectType::MODEL);
	malonBrazoIzquierdoObj->setModel(&malonBrazoIzquierdoModel);
	malonBrazoIzquierdoObj->setMaterial(&matOpaco);
	malonBrazoIzquierdoObj->transform.setPosition(-13.0f, 133.0f, 0.0f);
	malonBrazoIzquierdoObj->transform.setRotation(0.0f, 180.0f, 0.0f);
	malonCuerpoObj->addChild(malonBrazoIzquierdoObj);

	if (!malonAntebrazoIzquierdoModel.load("Models/Malon_AntebrazoIzquierdo.obj")) return false;
	malonAntebrazoIzquierdoObj = std::make_shared<GameObject>("Malon Antebrazo Izquierdo", GameObjectType::MODEL);
	malonAntebrazoIzquierdoObj->setModel(&malonAntebrazoIzquierdoModel);
	malonAntebrazoIzquierdoObj->setMaterial(&matOpaco);
	malonAntebrazoIzquierdoObj->transform.setPosition(23.0f, -10.0f, 3.0f);
	malonAntebrazoIzquierdoObj->transform.setRotation(0.0f, 180.0f, 0.0f);
	malonBrazoIzquierdoObj->addChild(malonAntebrazoIzquierdoObj);

	if (!malonManoIzquierdaModel.load("Models/Malon_ManoIzquierda.obj")) return false;
	malonManoIzquierdaObj = std::make_shared<GameObject>("Malon Mano Izquierda", GameObjectType::MODEL);
	malonManoIzquierdaObj->setModel(&malonManoIzquierdaModel);
	malonManoIzquierdaObj->setMaterial(&matOpaco);
	malonManoIzquierdaObj->transform.setPosition(-22.0f, -16.0f, 4.0f);
	malonManoIzquierdaObj->transform.setRotation(0.0f, 180.0f, 0.0f);
	malonAntebrazoIzquierdoObj->addChild(malonManoIzquierdaObj);

	if (!nimbus2000Model.load("Models/Nimbus2000.obj")) return false;
	nimbus2000Obj = std::make_shared<GameObject>("Nimbus 2000", GameObjectType::MODEL);
	nimbus2000Obj->setModel(&nimbus2000Model);
	nimbus2000Obj->setMaterial(&matBrillante);

	malonManoIzquierdaObj->addChild(nimbus2000Obj);
	nimbus2000Obj->transform.setPosition(15.0f, -30.0f, 10.0f);
	nimbus2000Obj->transform.setRotation(0.0f, 0.0f, -45.0f);
	nimbus2000Obj->transform.setScale(50.0f);

	SetupKeyframes();

	return true;
}

void Malon::SetupKeyframes()
{
	MalonKeyframe kf1;
	kf1.rotBrazoIzquierdo = glm::vec3(0.0f, -120.0f, 40.0f);
	kf1.rotAntebrazoIzquierdo = glm::vec3(0.0f, 180.0f, 0.0f);
	kf1.rotManoIzquierda = glm::vec3(0.0f, 180.0f, 0.0f);

	MalonKeyframe kf2;
	kf2.rotBrazoIzquierdo = glm::vec3(0.0f, -125.0f, 20.0f);
	kf2.rotAntebrazoIzquierdo = glm::vec3(0.0f, 180.0f, 10.0f);
	kf2.rotManoIzquierda = glm::vec3(0.0f, 180.0f, 20.0f);

	MalonKeyframe kf3;
	kf3.rotBrazoIzquierdo = glm::vec3(0.0f, -130.0f, 0.0f);
	kf3.rotAntebrazoIzquierdo = glm::vec3(0.0f, 180.0f, 20.0f);
	kf3.rotManoIzquierda = glm::vec3(0.0f, 180.0f, 40.0f);

	MalonKeyframe kf4;
	kf4.rotBrazoIzquierdo = glm::vec3(0.0f, -135.0f, -20.0f);
	kf4.rotAntebrazoIzquierdo = glm::vec3(0.0f, 180.0f, 30.0f);
	kf4.rotManoIzquierda = glm::vec3(0.0f, 180.0f, 60.0f);

	keyframes.push_back(kf1);
	keyframes.push_back(kf2);
	keyframes.push_back(kf3);
	keyframes.push_back(kf4);
}

void Malon::Update(float deltaTime)
{
	if (keyframes.empty()) return;

	transitionProgress += deltaTime * transitionSpeed;

	if (transitionProgress >= 1.0f)
	{
		transitionProgress = 0.0f;
		currentKeyframeIndex = nextKeyframeIndex;
		nextKeyframeIndex = (nextKeyframeIndex + 1) % keyframes.size();
	}

	const auto& currentKf = keyframes[currentKeyframeIndex];
	const auto& nextKf = keyframes[nextKeyframeIndex];

	glm::vec3 interpBrazo = glm::mix(currentKf.rotBrazoIzquierdo, nextKf.rotBrazoIzquierdo, transitionProgress);
	glm::vec3 interpAntebrazo = glm::mix(currentKf.rotAntebrazoIzquierdo, nextKf.rotAntebrazoIzquierdo, transitionProgress);
	glm::vec3 interpMano = glm::mix(currentKf.rotManoIzquierda, nextKf.rotManoIzquierda, transitionProgress);

	if (malonBrazoIzquierdoObj)
		malonBrazoIzquierdoObj->transform.setRotation(interpBrazo.x, interpBrazo.y, interpBrazo.z);

	if (malonAntebrazoIzquierdoObj)	
		malonAntebrazoIzquierdoObj->transform.setRotation(interpAntebrazo.x, interpAntebrazo.y, interpAntebrazo.z);

	if (malonManoIzquierdaObj)
		malonManoIzquierdaObj->transform.setRotation(interpMano.x, interpMano.y, interpMano.z);

	// Animacion de levitacion de la Nimbus 2000
	if (nimbus2000Obj)
	{
		nimbusTime += deltaTime * 2.5f;
		float nimbusOffset = sin(nimbusTime) * 30.0f;
		nimbus2000Obj->transform.setPosition(15.0f, -30.0f + nimbusOffset, 10.0f);
	}
}