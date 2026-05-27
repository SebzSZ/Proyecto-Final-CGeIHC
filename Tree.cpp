#include "Tree.h"

// 30 posiciones predefinidas distribuidas aleatoriamente en la escena
const glm::vec3 Tree::TREE_POSITIONS[NUM_TREES] = {
    glm::vec3(-15.0f, 0.0f, 30.0f),
    glm::vec3(5.0f, 0.0f, 40.0f),
    glm::vec3(35.0f, 0.0f, 25.0f),
    glm::vec3(-25.0f, 0.0f, -20.0f),
    glm::vec3(45.0f, 0.0f, -15.0f),
    glm::vec3(-35.0f, 0.0f, 50.0f),
    glm::vec3(15.0f, 0.0f, -30.0f),
    glm::vec3(-10.0f, 0.0f, 10.0f),
    glm::vec3(55.0f, 0.0f, 35.0f),
    glm::vec3(25.0f, 0.0f, 60.0f),
    glm::vec3(-45.0f, 0.0f, -10.0f),
    glm::vec3(40.0f, 0.0f, 5.0f),
    glm::vec3(-20.0f, 0.0f, 70.0f),
    glm::vec3(0.0f, 0.0f, 20.0f),
    glm::vec3(60.0f, 0.0f, 55.0f),
    glm::vec3(-30.0f, 0.0f, 35.0f),
    glm::vec3(20.0f, 0.0f, -45.0f),
    glm::vec3(50.0f, 0.0f, -30.0f),
    glm::vec3(-5.0f, 0.0f, 55.0f),
    glm::vec3(30.0f, 0.0f, 75.0f),
    glm::vec3(-40.0f, 0.0f, 5.0f),
    glm::vec3(10.0f, 0.0f, -10.0f),
    glm::vec3(65.0f, 0.0f, 20.0f),
    glm::vec3(-15.0f, 0.0f, -35.0f),
    glm::vec3(35.0f, 0.0f, 45.0f),
    glm::vec3(-50.0f, 0.0f, 25.0f),
    glm::vec3(45.0f, 0.0f, 70.0f),
    glm::vec3(5.0f, 0.0f, 65.0f),
    glm::vec3(-25.0f, 0.0f, 60.0f),
    glm::vec3(70.0f, 0.0f, 10.0f)
};

Tree::Tree()
    : treeRootObj(nullptr)
{
}

bool Tree::Initialize(Material& matOpaco)
{
    // Crear el objeto raíz que contendrá todos los árboles
    treeRootObj = std::make_shared<GameObject>("Trees");

    // Cargar el modelo de árbol
    if (!treeModel.load("Models/Tree.obj"))
        return false;

    // Crear los árboles en sus posiciones
    CreateTrees(matOpaco);

    return true;
}

void Tree::CreateTrees(Material& matOpaco)
{
    for (int i = 0; i < NUM_TREES; i++)
    {
        // Crear un GameObject para cada árbol
        std::shared_ptr<GameObject> treeObj = std::make_shared<GameObject>("Tree_" + std::to_string(i));
        treeObj->setModel(&treeModel);
        treeObj->setMaterial(&matOpaco);
        treeObj->transform.setPosition(TREE_POSITIONS[i].x, TREE_POSITIONS[i].y, TREE_POSITIONS[i].z);
        treeObj->transform.setScale(1.0f);

        // Agregar el árbol al contenedor de la raíz
        treeRootObj->addChild(treeObj);

        // Agregar a la lista de objetos individuales
        treeObjects.push_back(treeObj);
    }
}
