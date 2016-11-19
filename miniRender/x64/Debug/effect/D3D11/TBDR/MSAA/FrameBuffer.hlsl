#ifndef __FRAME_BUFFER_HLSL__
#define __FRAME_BUFFER_HLSL__

#include "PerFrameConstant.hlsl"

uint GetFrameBufferAddress(uint2 globalCoords, uint sampleIndex)
{
	return (sampleIndex*gFrameBufferDimensions.y+globalCoords.y)*gFrameBufferDimensions.x + globalCoords.x;
}

#endif