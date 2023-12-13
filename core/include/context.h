
#include <iostream>
#include <vector>
#include <map>
#include "base.h"
#include "render/system.h"

class Context final {
public:
	
	//render
	EngineObject renderEO;


	void Update() {

		//render
		Render::System::CreateBaseInfo(this);
	}

};
