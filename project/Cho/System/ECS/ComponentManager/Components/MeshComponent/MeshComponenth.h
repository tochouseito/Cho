#pragma once
struct MeshComponent final{
    uint32_t meshID = 0;

    void SetMeshID(uint32_t id) {
        meshID = id;
    }
};