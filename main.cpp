#define GLEW_STATIC           ////////////////////5
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//包含OpenGL数学运算库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
//VBO数据
float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
//EBO数据
unsigned int indices[] = {
	0,1,2,
	2,3,0
};
//10个立方体的平移向量
glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};
//鼠标位置
float lastX;
float lastY;
bool firstMouse = true;
//全局camera
Camera camera(glm::vec3(0, 0, 3.0f), glm::radians(0.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
//输入控制函数声明
void processInput(GLFWwindow* window);
//鼠标回调函数声明
void mouse_callback(GLFWwindow* window,double xPos,double yPos);
//滑轮回调函数声明
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
int main() {
	//初始化
	glfwInit();
	//设置版本3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//设置配置
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//创建窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL 多个立方体", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//绑定窗口到当前线程
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}
	//禁用光标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//注册鼠标事件回调函数
	glfwSetCursorPosCallback(window, mouse_callback);
	//注册滑轮事件回调函数
	glfwSetScrollCallback(window, scroll_callback);
	//设置绘图区域
	glViewport(0, 0, 800, 600);
	//设置背面剔除 逆时针绘制为正面
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	//设置只绘制线条
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	Shader* pshader = new Shader("vertextSource.txt", "fragmentSource.txt");
	pshader->print();
	//与layout(location=0)中 lacation 缓冲区位置一样（0-15）
	//位置属性
	glVertexAttribPointer(15, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(15);
	//颜色属性
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//uv坐标
	glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(7);

	//上下颠倒texture贴图
	stbi_set_flip_vertically_on_load(true);

	unsigned int TexBufferA;
	glGenTextures(1, &TexBufferA);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexBufferA);
	int width, height, numChannel;
	unsigned char* data = stbi_load("box.jpg", &width, &height, &numChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout << "load image failed0." << std::endl;
	}
	stbi_image_free(data);

	unsigned int TexBufferB;
	glGenTextures(1, &TexBufferB);
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, TexBufferB);
	unsigned char* data2 = stbi_load("awesomeface.png", &width, &height, &numChannel, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "load image failed13." << std::endl;
	}
	stbi_image_free(data2);

	//glm::mat4 trans;
	//位移矩阵
	//trans = glm::translate(trans, glm::vec3(-1.0, 0, 0));
	//旋转矩阵
	//trans = glm::rotate(trans, glm::radians(45.0f),glm::vec3(0,0,1.0f));
	//缩放矩阵
	//trans = glm::scale(trans, glm::vec3(2.0f, 2.0f, 2.0f));
	//局部坐标转世界坐标  模型矩阵
	//glm::mat4 modelMat;
	//modelMat = glm::rotate(modelMat, glm::radians(-55.0f),glm::vec3(1.0f,0,0));
	////世界坐标转相机坐标  观察矩阵
	glm::mat4 viewMat;
	//viewMat = glm::translate(viewMat, glm::vec3(0, 0, -3.0f));
	//获取viewMatrix方式一
	//Camera camera(glm::vec3(0, 0, 3.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1.0f, 0));
	//viewMat = camera.GetViewMatrix();
	//获取viewMatrix方式二
	/*Camera camera(glm::vec3(0, 0, 3.0f), glm::radians(0.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
	viewMat = camera.GetViewMatrix();*/
	////相机坐标转屏幕坐标（2D） 投影矩阵
	glm::mat4 projMat;
	projMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	
	//启用深度测试  遮挡的部位不会绘制出来
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		viewMat = camera.GetViewMatrix();
		processInput(window);
		glClearColor(0.5, 0.2, 0.3, 1.0);
		//添加清除深度信息
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glBindVertexArray(VAO);
		for (size_t i = 0; i < 10; i++)
		{
			glm::mat4 modelMat;
			//平移矩阵
			modelMat = glm::translate(modelMat, cubePositions[i]);
			//旋转
			if (i % 3<2) {
				modelMat = glm::rotate(modelMat, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
			}
			pshader->use();
			//uniform传递texture贴图数据
			glUniform1i(glGetUniformLocation(pshader->ID, "ourTextrue"), 0);
			glUniform1i(glGetUniformLocation(pshader->ID, "ourFace"), 13);
			//uniform传递旋转位移等变换矩阵，完成投影
			glUniformMatrix4fv(glGetUniformLocation(pshader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			glUniformMatrix4fv(glGetUniformLocation(pshader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(glGetUniformLocation(pshader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));

			//VBO方式绘制
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
		camera.UpdateCameraPos();

	}
	//结束绘制
	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.speedZ = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.speedZ = -1.0f;
	}
	else {
		camera.speedZ = .0f;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.speedZ = yoffset*100;
}
