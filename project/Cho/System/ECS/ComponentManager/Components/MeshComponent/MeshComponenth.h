#pragma once
#include<cstdint>
#include<string>
struct MeshComponent final{
    uint32_t meshID = 0;
    std::string meshModelName = "";
    

    void SetMeshID(uint32_t id) {
        meshID = id;
    }
};