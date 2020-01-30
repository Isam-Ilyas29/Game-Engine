#include "tool.h"


float delta_time = 0.0f;
float last_frame = 0.0f;


float framesPerSecond() {
	float frame_rate = 1.0 / delta_time;
	return frame_rate;
}

