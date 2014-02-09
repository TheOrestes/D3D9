D3D9 Framework
====

This is DirectX 9.0c based C++ framework. To use this framework, you will need the following :

1. 	Data folder : Due to shear size of the folder, I have not uploaded the same on Git. But it is required to run the framework.
	It contains, custom scene file namely, Level1.scn which engine uses right at the beginning to load all the assets. This scene 
	file also keeps information about Mesh world space transformations, Mesh materials & shader information. Scene file also
	has information about lights in the scene. 
	
	Folder also contains MD3, MD5, MD2 & COLLADA meshes which were used  to test out ASSIMP's capabilities to load specified 
	mesh formats. Folder has various textures used for Particle data & skyboxes too. 
	
	Please contact me incase you need the scene file. 
	
2. 	ASSIMP :  Can be downloaded from "http://assimp.sourceforge.net/". You will need to build it. Current source code is tested
	against 32-bit debug & release build of ASSIMP. Please feel free to report any sort of issues with this. Current prohject uses
	statically linked DLL for loading ASSIMP related features. 
	
Following are the key features of the framework : 

1. 	Basic Scene Management
2. 	ASSIMP integration with Rendering & Shading Engine for Mesh loading.
3. 	Shader Engine ( Fx )
4. 	Postprocessing Framework ( Screen space effects such as Edge detection, Gaussian Blur, Bath glass etc
5. 	Particle Effects Framework ( Factory System )
6. 	Basic Heightmap based Terrain Rendering
7. 	Observer Design Pattern for Input Manager
8. 	Logging & Error Reporting Mechanism
9. 	Screenshot feature


