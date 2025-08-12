#pragma once
#include "map/map.h"
#include "../utilities/AssetIO/API.h"
#include "../rendering/2DRenderer/2DRenderer.h"
#include <vector>
#include "../Engine.h"



void InitWorld(){
		// testRenderable.CreateTriangle(
		// 	{0.0f, 1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f},
		// 	{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}
		// );

		// V3D v[] = {{0.0f, 1.0f, 0.0f}, {1.0f, 0.7f, 0.0f},{0.8f, -0.5f, 0.0f},{-0.8f, -0.5f, 0.0f} ,{-1.0f, 0.7f, 0.0f}};
		// V3D rgb[] = {{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f},{0.0f, 0.0f, 1.0f},{1.0f, 0.0f, 0.0f}};
		// V3Du8 indices[] = {{0,1,2}, {0,2,3},{0,3,4}};
		// testRenderable.CreatePolygon(
		// 	5,
		// 	v,
		// 	rgb,
		// 	indices
		// );
		

		//for(int i = 0; i < 100; ++i){
		//	BasicHex hexagon;
		//	hexagon.SetLocation(hex_base_locations((i % 10) -5, (i / 10) - 5));
		//	basicHexagons.push_back(hexagon);
		//}

		//for(auto&e : basicHexagons){
		//	e.Init();
		//}

		// BasicHex hex1,hex2,hex3,hex4;
		// hex1.Init();hex2.Init();hex3.Init();hex4.Init();
		// hex1.SetLocation({0.0f, 0.0f});hex2.SetLocation({0.1f, -0.6f});hex3.SetLocation({0.2f, 0.4f});hex4.SetLocation({0.0f, 0.7f});
		// basicHexagons.push_back(hex1);basicHexagons.push_back(hex2);basicHexagons.push_back(hex3);basicHexagons.push_back(hex4);
	map.Init(5);
	visualDebugger.Init();
}



void UpdateWorld(uint64_t uElapsedMicros, uint64_t uTotalMicros) {
	
	map.Draw(M4::identity()); 
	//for(auto&e : basicHexagons)
	//	e.Update(uElapsedMicros, uTotalMicros);

	visualDebugger.Draw();
}


void StopWorld() {



}
