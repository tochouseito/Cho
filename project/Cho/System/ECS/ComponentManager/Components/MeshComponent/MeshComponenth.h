#pragma once
struct MeshComponent {
    uint32_t meshID = 0;

    void SetMeshID(uint32_t id) {
        meshID = id;
    }
};