// DeferredShading

/*
	Links
	Rastertek DeferredShading: http://www.rastertek.com/dx11tut50.html
	http://backspacegames.net/2013/12/14/deferred-rendering-creating-the-gbuffer/
	http://www.catalinzima.com/xna/tutorials/deferred-rendering-in-xna/creating-the-g-buffer/
	
	Create a GBuffer with 4 render targets.
	pass all materials, position, normals and texture coordinates from the scene into the GBuffer.
	NOTE: Use one of three types of forward lighting for this effect rendered onto a fullscreen quad. (Directional, Point, Spot)
	NOTE: Perform Tessellation on Point Lights and Spot Lights.
	Unpack the GBuffer to get the information passed from each render targets texture and pass the information to the light algorithm.
*/

