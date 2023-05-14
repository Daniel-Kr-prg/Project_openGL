#pragma once

#include "object.h"

class Boat : public ObjectInstance
{
public:

	Boat();
	~Boat();

	void draw() override;
	void deserialize(nlohmann::json data) override;
private:

	bool initialized;  ///< object has the shader with defined locations
};

