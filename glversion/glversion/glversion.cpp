// glversion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include "GL/freeglut.h"

int _tmain(int argc, _TCHAR* argv[])
{
	char *argvs[]={"OpenGL Thread"};
	glutInit(&argc,argvs);
	//��ʾģʽ��ʼ��
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	//���崰�ڴ�С
	glutInitWindowSize(30,30);
	//���崰��λ��
	glutInitWindowPosition(10000,10000);
	//��������
	glutCreateWindow("OpenGL Version");
	const GLubyte* name = glGetString(GL_VENDOR); //���ظ���ǰOpenGLʵ�ֳ��̵�����
	const GLubyte* biaoshifu = glGetString(GL_RENDERER); //����һ����Ⱦ����ʶ����ͨ���Ǹ�Ӳ��ƽ̨
	const GLubyte* OpenGLVersion =glGetString(GL_VERSION); //���ص�ǰOpenGLʵ�ֵİ汾��
	//const GLubyte* Extensions  =glGetString(GL_EXTENSIONS);
	const GLubyte* gluVersion= gluGetString(GLU_VERSION); //���ص�ǰGLU���߿�汾
	printf("OpenGLʵ�ֳ��̵����֣�%s\n", name);
	printf("��Ⱦ����ʶ����%s\n", biaoshifu);
	printf("OpenGLʵ�ֵİ汾�ţ�%s\n",OpenGLVersion );
	//printf("OpenGL֧�ֵ���չ��%s\n",Extensions );
	printf("OGLU���߿�汾��%s\n", gluVersion);
	system("pause");

	return 0;
}

