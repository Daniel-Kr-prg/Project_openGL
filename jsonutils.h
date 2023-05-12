#pragma once

#include "pgr.h"
#include "json.hpp"

static glm::vec3 readVectorFromJSON(nlohmann::json data, glm::vec3 defaultValue)
{
	if (data.contains("x") && data.contains("y") && data.contains("z"))
		return glm::vec3(data["x"].get<float>(), data["y"].get<float>(), data["z"].get<float>());
	else
		if (data.contains("r") && data.contains("g") && data.contains("b"))
			return glm::vec3(data["r"].get<float>(), data["g"].get<float>(), data["b"].get<float>());
		else
			return defaultValue;
}