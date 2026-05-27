#pragma once

#include <glm.hpp>
#include <memory>
#include <vector>
#include "GameObject.h"
#include "Model.h"
#include "Material.h"

class Tree
{
public:
    Tree();

    bool Initialize(Material& matOpaco);
    std::vector<std::shared_ptr<GameObject>> GetTreeObjects() const { return treeObjects; }
    std::shared_ptr<GameObject> GetTreeObject() const { return treeRootObj; }

private:
    static constexpr int NUM_TREES = 30;
    static const glm::vec3 TREE_POSITIONS[NUM_TREES];

    std::shared_ptr<GameObject> treeRootObj;
    std::vector<std::shared_ptr<GameObject>> treeObjects;
    Model treeModel;

    void CreateTrees(Material& matOpaco);
};
