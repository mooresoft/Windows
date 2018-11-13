// glversion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include "GL/freeglut.h"

int _tmain(int argc, _TCHAR* argv[])
{
	char *argvs[]={"OpenGL Thread"};
	glutInit(&argc,argvs);
	//显示模式初始化
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	//定义窗口大小
	glutInitWindowSize(30,30);
	//定义窗口位置
	glutInitWindowPosition(10000,10000);
	//创建窗口
	glutCreateWindow("OpenGL Version");
	const GLubyte* name = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
	const GLubyte* biaoshifu = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
	const GLubyte* OpenGLVersion =glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
	//const GLubyte* Extensions  =glGetString(GL_EXTENSIONS);
	const GLubyte* gluVersion= gluGetString(GLU_VERSION); //返回当前GLU工具库版本
	printf("OpenGL实现厂商的名字：%s\n", name);
	printf("渲染器标识符：%s\n", biaoshifu);
	printf("OpenGL实现的版本号：%s\n",OpenGLVersion );
	//printf("OpenGL支持的扩展：%s\n",Extensions );
	printf("OGLU工具库版本：%s\n", gluVersion);
	system("pause");

	return 0;
}

