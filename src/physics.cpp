#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>
#include <iostream>
#include <time.h>

bool show_test_window = false;

namespace LilSpheres {
	extern const int maxParticles;
	extern void setupParticles(int numTotalParticles, float radius = 0.03f);
	extern void cleanupParticles();
	extern void updateParticles(int startIdx, int count, float* array_data);
	extern void drawParticles(int startIdx, int count);
}

static float lifeP = 3.f;
float* particleCOOR;
float* particleLast;
static int PxS = 100;

struct Particle {
	float life = 1.f;

};

float *InitialPos;
float *lastPos;
float *particleVel;
static int mode = 1;
static int type = 1;
int mode1 = 1;

int lastUsed = 0;

Particle ParticlesContainer[SHRT_MAX];
float particlesAlive = 100000.0f;
int particleCounter = 1;

void GUI() {

	//FrameRate
	{ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); } //Framerate. TODO
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	ImGui::Text("Euler                          Verlet");
	ImGui::SliderInt(" ", &mode, 1, 2);
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	ImGui::Text("Fountain                      Cascade");
	ImGui::SliderInt("", &type, 1, 2);
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	ImGui::SliderFloat("Life expectancy", &lifeP, 1.f, 5.f);
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	ImGui::SliderInt("Particles per second", &PxS, 100, 300);
	



	if (show_test_window) { // ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

void PhysicsInit() {
	
	LilSpheres::setupParticles(LilSpheres::maxParticles);

	particleCOOR = new float[LilSpheres::maxParticles * 3];
	particleLast = new float[LilSpheres::maxParticles * 3];
	InitialPos = new float[LilSpheres::maxParticles * 3];
	particleVel = new float[LilSpheres::maxParticles * 3];
	lastPos = new float[LilSpheres::maxParticles * 3];
	



	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		particleLast[i * 3 + 0] = particleCOOR[i * 3 + 0] = -3.f;
		particleLast[i * 3 + 1] = particleCOOR[i * 3 + 1] = 7.f;
		particleLast[i * 3 + 2] = particleCOOR[i * 3 + 2] = ((float)rand() / RAND_MAX) * 2.f - 1.f;

	}
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		
		InitialPos[i * 3 + 0] = 0.f;
		InitialPos[i * 3 + 1] = 2.f;
		InitialPos[i * 3 + 2] = 0.f;
		particleVel[i * 3 + 0] = ((float)rand() / RAND_MAX) * 2.f - 1.f;
		particleVel[i * 3 + 1] = ((float)rand() / RAND_MAX) * 5.f + 4.f;
		particleVel[i * 3 + 2] = ((float)rand() / RAND_MAX) * 2.f - 1.f;
	}


}


void PhysicsUpdate(float dt) {

	
	switch (mode) {

	case 1:

		if (type == 1) {

			for (int i = 0; i < particleCounter; i++) {

				Particle& p = ParticlesContainer[i];
				if (p.life > 0.0f) {
					p.life -= dt;

				}
				if (p.life > 0.0f) {
					
					float temp[3]{ InitialPos[i * 3 + 0], InitialPos[i * 3 + 1],  InitialPos[i * 3 + 2] }; //Stores on temp variable the last position for each axis

					if (lastPos[i * 3 + 1] <= 0) { //Terra
						particleVel[i * 3 + 1] *= -0.9;
					}
					else if (lastPos[i * 3 + 1] >= 10) { //Sostre
						particleVel[i * 3 + 1] *= -0.9;
					}
					else if (lastPos[i * 3 + 0] <= -5) { //Paret esquerra
						particleVel[i * 3 + 0] *= -0.9;
					}
					else if (lastPos[i * 3 + 0] >= 5) { //Paret dreta
						particleVel[i * 3 + 0] *= -0.9;
					}
					else if (lastPos[i * 3 + 2] >= 5) { //Paret davant
						particleVel[i * 3 + 2] *= -0.9;
					}
					else if (lastPos[i * 3 + 2] <= -5) { //Paret darrera
						particleVel[i * 3 + 2] *= -0.9;
					}


					lastPos[i * 3 + 0] = InitialPos[i * 3 + 0] + dt * particleVel[i * 3 + 0]; //Applies Euler on X
					lastPos[i * 3 + 1] = InitialPos[i * 3 + 1] + dt * particleVel[i * 3 + 1]; //Applies Euler on Y
					lastPos[i * 3 + 2] = InitialPos[i * 3 + 2] + dt * particleVel[i * 3 + 2]; //Applies Euler on Z

					particleVel[i * 3 + 1] = particleVel[i * 3 + 1] + dt * -9.81;

					InitialPos[i * 3 + 0] = lastPos[i * 3 + 0];
					InitialPos[i * 3 + 1] = lastPos[i * 3 + 1];
					InitialPos[i * 3 + 2] = lastPos[i * 3 + 2];



				}
				else {
					if (particleCounter >= LilSpheres::maxParticles) { particleCounter -= PxS; }

					if (ParticlesContainer[i].life <= 0.0f) {
						InitialPos[i * 3 + 0] = 0.f;
						InitialPos[i * 3 + 1] = 2.f;
						InitialPos[i * 3 + 2] = 0.f;
						particleVel[i * 3 + 0] = ((float)rand() / RAND_MAX) * 2.f - 1.f;
						particleVel[i * 3 + 1] = ((float)rand() / RAND_MAX) * 5.f + 4.f;
						particleVel[i * 3 + 2] = ((float)rand() / RAND_MAX) * 2.f - 1.f;
						ParticlesContainer[i].life = lifeP;


					}


				}
			}
			LilSpheres::updateParticles(0, LilSpheres::maxParticles, InitialPos);
			if (particleCounter + PxS <= LilSpheres::maxParticles) { particleCounter += PxS; }
		}
		else if (type == 2) {
			
			for (int i = 0; i < particleCounter; i++) {

				Particle& p = ParticlesContainer[i];
				if (p.life > 0.0f) {
					p.life -= dt;

				}
				if (p.life > 0.0f) {

					float temp[3]{ InitialPos[i * 3 + 0], InitialPos[i * 3 + 1],  InitialPos[i * 3 + 2] }; //Stores on temp variable the last position for each axis

					if (lastPos[i * 3 + 1] <= 0) { //Terra
						particleVel[i * 3 + 1] *= -0.5;
					}
					else if (lastPos[i * 3 + 1] >= 10) { //Sostre
						particleVel[i * 3 + 1] *= -0.9;
					}
					else if (lastPos[i * 3 + 0] <= -5) { //Paret esquerra
						particleVel[i * 3 + 0] *= -0.9;
					}
					else if (lastPos[i * 3 + 0] >= 5) { //Paret dreta
						particleVel[i * 3 + 0] *= -0.9;
					}
					else if (lastPos[i * 3 + 2] >= 5) { //Paret davant
						particleVel[i * 3 + 2] *= -0.9;
					}
					else if (lastPos[i * 3 + 2] <= -5) { //Paret darrera
						particleVel[i * 3 + 2] *= -0.9;
					}


					lastPos[i * 3 + 0] = InitialPos[i * 3 + 0] + dt * particleVel[i * 3 + 0]; //Applies Euler on X
					lastPos[i * 3 + 1] = InitialPos[i * 3 + 1] + dt * particleVel[i * 3 + 1]; //Applies Euler on Y
					lastPos[i * 3 + 2] = InitialPos[i * 3 + 2] + dt * particleVel[i * 3 + 2]; //Applies Euler on Z

					particleVel[i * 3 + 1] = particleVel[i * 3 + 1] + dt * -9.81;

					InitialPos[i * 3 + 0] = lastPos[i * 3 + 0];
					InitialPos[i * 3 + 1] = lastPos[i * 3 + 1];
					InitialPos[i * 3 + 2] = lastPos[i * 3 + 2];



				}
				else {
					if (particleCounter >= LilSpheres::maxParticles) { particleCounter -= PxS; }

					if (ParticlesContainer[i].life <= 0.0f) {
						InitialPos[i * 3 + 0] = -3.f;
						InitialPos[i * 3 + 1] = 7.f;
						InitialPos[i * 3 + 2] = ((float)rand() / RAND_MAX) * 2.f - 1.f;
						particleVel[i * 3 + 0] = 1.5f;
						particleVel[i * 3 + 1] = ((float)rand() / RAND_MAX) * 0.5f;
						particleVel[i * 3 + 2] = ((float)rand() / RAND_MAX) * 2.f - 1.f;
						ParticlesContainer[i].life = lifeP;
					}
				}
			}
			LilSpheres::updateParticles(0, LilSpheres::maxParticles, InitialPos);
			if (particleCounter + PxS <= LilSpheres::maxParticles) { particleCounter += PxS; }
		}

		break;



	case 2:

		for (int i = 0; i < LilSpheres::maxParticles; ++i) { //Verlet

			float temp[3]{ particleCOOR[i * 3 + 0], particleCOOR[i * 3 + 1],  particleCOOR[i * 3 + 2] }; //Stores on temp variable the last position for each axis

			particleCOOR[i * 3 + 1] = particleCOOR[i * 3 + 1] + (particleCOOR[i * 3 + 1] - particleLast[i * 3 + 1]) + (-9.81f * (dt*dt)); //Applies Verlet on Y

			particleLast[i * 3 + 0] = temp[0]; particleLast[i * 3 + 1] = temp[1]; 	particleLast[i * 3 + 2] = temp[2]; //Stores each position axis on particleLast

			LilSpheres::updateParticles(0, LilSpheres::maxParticles, particleCOOR);

		}


		break;
	}



}





void PhysicsCleanup() {
	LilSpheres::cleanupParticles();
}