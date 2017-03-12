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
static float elasticity = 4.2f;

struct Particle {
	float life = 0.1f;
	glm::vec3 vector = { 0.f,2.f,0.f };
	glm::vec3 velvector = { ((float)rand() / RAND_MAX) * 2.f - 1.f,((float)rand() / RAND_MAX) * 5.f + 4.f,((float)rand() / RAND_MAX) * 2.f - 1.f, };
	glm::vec3 lastvector;
};

float *InitialPos;
float *lastPos;
float *particleVel;
static int mode = 1;
static int type = 1;
int mode1 = 1;
int lastMode = 1;
int lastUsed = 0;

glm::vec3 terraN = { 0,1,0 };
Particle *totalParticles;
int particleCounter = 1;

void GUI() {

	//FrameRate
	{ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); } //Framerate. TODO
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	ImGui::Text("Max particles set to 30.000");
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
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	ImGui::SliderFloat("Elasticity", &elasticity, 0.5f, 9.f);




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
	totalParticles = new Particle[SHRT_MAX];



	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		particleLast[i * 3 + 0] = particleCOOR[i * 3 + 0] = -3.f;
		particleLast[i * 3 + 1] = particleCOOR[i * 3 + 1] = 7.f;
		particleLast[i * 3 + 2] = particleCOOR[i * 3 + 2] = ((float)rand() / RAND_MAX) * 2.f - 1.f;

	}
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		totalParticles[i].vector = { 0.f,2.f,0.f };
		totalParticles[i].velvector = { ((float)rand() / RAND_MAX) * 2.f - 1.f,((float)rand() / RAND_MAX) * 5.f + 4.f,((float)rand() / RAND_MAX) * 2.f - 1.f };
	}


}


void PhysicsUpdate(float dt) {


	switch (mode) {

		//Euler
	case 1:
		//Fountain
		if (type == 1) {
			if (lastMode == 2) {

				for (int i = 0; i < LilSpheres::maxParticles; i++) {
					totalParticles[i].vector = { 0.f,2.f,0.f };
					totalParticles[i].velvector = { ((float)rand() / RAND_MAX) * 2.f - 1.f,((float)rand() / RAND_MAX) * 5.f + 4.f,((float)rand() / RAND_MAX) * 2.f - 1.f };
					totalParticles[i].life = lifeP;
				}
				particleCounter = 1;
				lastMode = 1;
			}

			for (int i = 0; i < particleCounter; i++) {

				Particle& p = totalParticles[i];
				if (p.life > 0.0f) {
					p.life -= dt;

				}

				if (p.life > 0.0f) {

					//(n*pt+n)*(n*pt'+n) <= 0
					if ((terraN.y * p.lastvector.y + terraN.y)*(terraN.y*p.vector.y + terraN.y) <= 0) {
						p.velvector.y *= -elasticity*0.1;
					}
					
					if (lastPos[i * 3 + 1] <= 0) { //Terra
						particleVel[i * 3 + 1] *= -elasticity*0.1;
					}
					else if (lastPos[i * 3 + 1] >= 10) { //Sostre
						particleVel[i * 3 + 1] *= -elasticity*0.1;
					}
					else if (lastPos[i * 3 + 0] <= -5) { //Paret esquerra
						particleVel[i * 3 + 0] *= -elasticity*0.1;
					}
					else if (lastPos[i * 3 + 0] >= 5) { //Paret dreta
						particleVel[i * 3 + 0] *= -elasticity*0.1;
					}
					else if (lastPos[i * 3 + 2] >= 5) { //Paret davant
						particleVel[i * 3 + 2] *= -elasticity*0.1;
					}
					else if (lastPos[i * 3 + 2] <= -5) { //Paret darrera
						particleVel[i * 3 + 2] *= -elasticity*0.1;
					}

					p.lastvector.x = p.vector.x + dt * p.velvector.x; //Euler on X
					p.lastvector.y = p.vector.y + dt * p.velvector.y; //Euler on Y
					p.lastvector.z = p.vector.z + dt * p.velvector.z; //Euler on Z

					p.velvector.y = p.velvector.y + dt * -9.81; //Velocity on Y

					p.vector.x = p.lastvector.x;
					p.vector.y = p.lastvector.y;
					p.vector.z = p.lastvector.z;

				}

				else {
					if (particleCounter >= LilSpheres::maxParticles) { particleCounter -= PxS; }

					if (totalParticles[i].life <= 0.0f) {
						totalParticles[i].vector = { 0.f,2.f,0.f };
						totalParticles[i].velvector = { ((float)rand() / RAND_MAX) * 2.f - 1.f,((float)rand() / RAND_MAX) * 5.f + 4.f,((float)rand() / RAND_MAX) * 2.f - 1.f};

						totalParticles[i].life = lifeP;
					}
				}
			}
			
			for (int i = 0; i < LilSpheres::maxParticles; i++) {
				InitialPos[i * 3 + 0] = totalParticles[i].vector.x;
				InitialPos[i * 3 + 1] = totalParticles[i].vector.y;
				InitialPos[i * 3 + 2] = totalParticles[i].vector.z;

			}

			LilSpheres::updateParticles(0, LilSpheres::maxParticles, InitialPos);

			if (particleCounter + PxS <= LilSpheres::maxParticles) { particleCounter += PxS; }
		}

		//Cascade
		else if (type == 2) {
			
			if (lastMode == 1) {
				for (int i = 0; i < LilSpheres::maxParticles; i++) {
					totalParticles[i].vector = { -3.f,7.f,((float)rand() / RAND_MAX) * 2.f - 1.f };
					totalParticles[i].velvector = { 1.5f,((float)rand() / RAND_MAX) * 0.5f,((float)rand() / RAND_MAX) * 2.f - 1.f};
					totalParticles[i].life = lifeP;
				}
				particleCounter = 1;
				lastMode = 2;
				}
			for (int i = 0; i < particleCounter; i++) {

				Particle& p = totalParticles[i];
				if (p.life > 0.0f) {
					p.life -= dt;

				}
				if (p.life > 0.0f) {

					if (lastPos[i * 3 + 1] <= 0) { //Terra
						particleVel[i * 3 + 1] *= -elasticity*0.1;
						
					}
					else if (lastPos[i * 3 + 1] >= 10) { //Sostre
						particleVel[i * 3 + 1] *= -elasticity*0.1;
					}
					else if (lastPos[i * 3 + 0] <= -5) { //Paret esquerra
						particleVel[i * 3 + 0] *= -elasticity*0.1;
					}
					else if (lastPos[i * 3 + 0] >= 5) { //Paret dreta
						particleVel[i * 3 + 0] *= -elasticity*0.1;
					}
					else if (lastPos[i * 3 + 2] >= 5) { //Paret davant
						particleVel[i * 3 + 2] *= -elasticity*0.1;
					}
					else if (lastPos[i * 3 + 2] <= -5) { //Paret darrera
						particleVel[i * 3 + 2] *= -elasticity*0.1;
					}

					p.lastvector.x = p.vector.x + dt * p.velvector.x; //Euler on X
					p.lastvector.y = p.vector.y + dt * p.velvector.y; //Euler on Y
					p.lastvector.z = p.vector.z + dt * p.velvector.z; //Euler on Z

					p.velvector.y = p.velvector.y + dt * -9.81; //Velocity on Y

					p.vector.x = p.lastvector.x;
					p.vector.y = p.lastvector.y;
					p.vector.z = p.lastvector.z;

				}

				else {
					if (particleCounter >= LilSpheres::maxParticles) { particleCounter -= PxS; }

					if (totalParticles[i].life <= 0.0f) {
						totalParticles[i].vector = { -3.f,7.f,((float)rand() / RAND_MAX) * 2.f - 1.f };
						totalParticles[i].velvector = { 1.5f,((float)rand() / RAND_MAX) * 0.5f,((float)rand() / RAND_MAX) * 2.f - 1.f };
						totalParticles[i].life = lifeP;
					}
				}
			}

			for (int i = 0; i < LilSpheres::maxParticles; i++) {
				InitialPos[i * 3 + 0] = totalParticles[i].vector.x;
				InitialPos[i * 3 + 1] = totalParticles[i].vector.y;
				InitialPos[i * 3 + 2] = totalParticles[i].vector.z;

			}

			LilSpheres::updateParticles(0, LilSpheres::maxParticles, InitialPos);
			if (particleCounter + PxS <= LilSpheres::maxParticles) { particleCounter += PxS; }
		}

		break;


	//Verlet
	case 2:

		for (int i = 0; i < LilSpheres::maxParticles; ++i) { //Verlet

			float temp[3]{ particleCOOR[i * 3 + 0], particleCOOR[i * 3 + 1],  particleCOOR[i * 3 + 2] }; //Stores on temp variable the last position for each axis

			particleCOOR[i * 3 + 1] = particleCOOR[i * 3 + 1] + (particleCOOR[i * 3 + 1] - particleLast[i * 3 + 1]) + (-9.81f * (dt*dt)); //Applies Verlet on Y

			particleLast[i * 3 + 0] = temp[0]; particleLast[i * 3 + 1] = temp[1]; 	particleLast[i * 3 + 2] = temp[2]; //Stores each position axis on particleLast

			

		}

		LilSpheres::updateParticles(0, LilSpheres::maxParticles, particleCOOR);
		break;
	}



}





void PhysicsCleanup() {
	LilSpheres::cleanupParticles();
}