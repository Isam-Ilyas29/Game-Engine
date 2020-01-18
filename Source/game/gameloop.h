#pragma once

#include <assert.h>
#include <filesystem>

#include "../context/context.h"
#include "../rendering/graphic.h"
#include "../rendering/texture.h"
#include "../input/callback.h"
#include "../camera/camera.h"


namespace gameloop {
	void run(int argc, char* argv[]);
}

