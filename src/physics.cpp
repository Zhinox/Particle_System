#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>

bool show_test_window = false;

namespace LilSpheres {
	extern const int maxParticles;
	extern void setupParticles(int numTotalParticles, float radius = 0.15f);
	extern void cleanupParticles();
	extern void updateParticles(int startIdx, int count, float* array_data);
	extern void drawParticles(int startIdx, int count);
}

float* particleCOOR; 
float* particleLast;

void GUI() { 
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//TODO
	}

	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}



void PhysicsInit() {
	LilSpheres::setupParticles(LilSpheres::maxParticles);

	particleCOOR = new float[LilSpheres::maxParticles * 3];
	particleLast = new float[LilSpheres::maxParticles * 3]; 
	
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		particleCOOR[i * 3 + 0] = ((float)rand() / RAND_MAX) * 10.f - 5.f;
		particleCOOR[i * 3 + 1] = ((float)rand() / RAND_MAX) * 10.f;
		particleCOOR[i * 3 + 2] = ((float)rand() / RAND_MAX) * 10.f - 5.f;
	}

	LilSpheres::updateParticles(0, LilSpheres::maxParticles, particleCOOR);

	//delete[] particleVectors;
	
}


void PhysicsUpdate(float dt) {

	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
	//particleVectors[i * 3 + 0] = ((float)rand() / RAND_MAX) * 10.f - 5.f;
	particleLast[i * 3 + 1] = particleCOOR[i * 3 + 1];
	particleCOOR[i * 3 + 1] = particleCOOR[i * 3 + 1] + (particleCOOR[i * 3 + 1] - particleLast[i * 3 + 1]) + (-9.81f * (dt*dt)); // ((float)rand() / RAND_MAX) * 10.f;
	//particleVectors[i * 3 + 2] = ((float)rand() / RAND_MAX) * 10.f - 5.f;

	}
	
	LilSpheres::updateParticles(0, LilSpheres::maxParticles, particleCOOR);

}



void PhysicsCleanup() {
	LilSpheres::cleanupParticles();
}