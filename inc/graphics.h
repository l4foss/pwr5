#ifndef GRAPHICS_H
#define GRAPHICS_H 

/*
 * this likely support only intel HD graphics (i915)
 */

enum type {
	GRAPHICS_UNKNOWN = 0
	GRAPHICS_INTEL = 1,
	GRAPHICS_RADEON = 1 << 1,
	GRAPHICS_NVIDIA = 1 << 2
};

typedef struct {
	
} IGraphicsInfo;

IGraphicsInfo *graphics_get_info() {

}

#endif
