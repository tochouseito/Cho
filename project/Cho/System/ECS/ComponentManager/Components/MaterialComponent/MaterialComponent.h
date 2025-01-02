#pragma once

#include"ChoMath.h"
#include"ConstantData/MaterialData.h"

struct MaterialComponent final {
	std::string textureID = "";
	std::string preTexID = "";

	Color color = { 1.0f,1.0f,1.0f,1.0f };
	int enableLighting = false;
	Matrix4 matUV = ChoMath::MakeIdentity4x4();
	float shininess = 50.0f;

	ConstBufferDataMaterial* constData = nullptr;

	uint32_t cbvIndex = 0;
};