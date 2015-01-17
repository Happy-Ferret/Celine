/////////////////////////////////////////////////////////////////////////////
// Name:        	main.c
// Purpose:     
// Author:      	Nikolaos Patsiouras
// Email:			npatsiouras@gmail.com
// Created:     	2015, 1, 14, 1:35 PM
// Modified by:		Nikos
// Last Modified:   2015, 1, 16, 5:55 PM
// Copyright:   	(c) 2015 Nikolaos Patsiouras
// Licence:     	MIT License
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL\glew.h> //Must be before freeglut inclusion
#include <GL\freeglut.h>


#define WINDOW_TITLE_PREFIX "OpenGLPrimer"

typedef struct
{
	float XYZW[4];
	float RGBA[4];
} Vertex;

typedef enum GeometryRenderState
{
	NORMAL,
	INDIVIDUAL_SPOKE
} GeometryRenderState;

int CurrentWidth = 600,
CurrentHeight = 600,
WindowHandle = 0;
unsigned FrameCount = 0;
GLuint VertexShaderId = 0, FragmentShaderId = 0, ProgramId = 0, VaoId = 0, VboId = 0, IndexBufferId[5];
GeometryRenderState state = NORMAL;

const GLchar* VertexShader =
{
	"#version 400\n"\

	"layout(location=0) in vec4 in_Position;\n"\
	"layout(location=1) in vec4 in_Color;\n"\
	"out vec4 ex_Color;\n"\

	"void main(void)\n"\
	"{\n"\
	" gl_Position = in_Position;\n"\
	" ex_Color = in_Color;\n"\
	"}\n"
};

const GLchar* FragmentShader =
{
	"#version 400\n"\

	"in vec4 ex_Color;\n"\
	"out vec4 out_Color;\n"\

	"void main(void)\n"\
	"{\n"\
	" out_Color = ex_Color;\n"\
	"}\n"
};

void Initialize( int, char*[] );
void InitWindow( int, char*[] );
void ResizeFunction( int, int );
void RenderFunction( void );
void TimerFunction( int );
void IdleFunction( void );
void KeyboardFunction( unsigned char, int, int );
void Cleanup( void );
void CreateVBO( void );
void DestroyVBO( void );
void CreateShaders( void );
void DestroyShaders( void );

int main( int argc, char* argv[] )
{
	Initialize( argc, argv );
	glutMainLoop();
	exit( EXIT_SUCCESS );
}
void Initialize( int argc, char* argv[] )
{
	GLenum GlewInitResult;

	InitWindow( argc, argv );

	glewExperimental = GL_TRUE; //GLEW probes the graphics driver for some info and in case it's an experimental/pre-release driver we need this set to avoid exceptions.
	GlewInitResult = glewInit();

	if ( GLEW_OK != GlewInitResult )
	{
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString( GlewInitResult )
			);
		exit( EXIT_FAILURE );
	}

	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString( GL_VERSION )
		);

	CreateShaders();
	CreateVBO();
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
}
void InitWindow( int argc, char* argv[] )
{
	glutInit( &argc, argv );
	glutInitContextVersion( 4, 0 );
	glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
		);
	glutInitWindowSize( CurrentWidth, CurrentHeight );
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
	WindowHandle = glutCreateWindow( WINDOW_TITLE_PREFIX );
	if ( WindowHandle < 1 )
	{
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
			);
		exit( EXIT_FAILURE );
	}
	glutReshapeFunc( ResizeFunction );
	glutDisplayFunc( RenderFunction );
	glutIdleFunc( IdleFunction );
	glutTimerFunc( 0, TimerFunction, 0 );
	glutCloseFunc( Cleanup );
	glutKeyboardFunc( KeyboardFunction );
}
void ResizeFunction( int Width, int Height )
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport( 0, 0, CurrentWidth, CurrentHeight );
}
void RenderFunction( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	if ( state == NORMAL )
	{
		glDrawElements( GL_TRIANGLES, 48, GL_UNSIGNED_BYTE, NULL );
	}
	else if ( state == INDIVIDUAL_SPOKE )
	{
		glDrawElements( GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, NULL );
	}
	
	glutSwapBuffers();
	++FrameCount;
}
void IdleFunction( void )
{
	glutPostRedisplay();
}

void TimerFunction( int Value )
{
	if ( 0 != Value )
	{
		char* TempString = (char*)
			malloc( 512 + strlen( WINDOW_TITLE_PREFIX ) );

		sprintf(
			TempString,
			"%s: %d Frames Per Second @ %d x %d",
			WINDOW_TITLE_PREFIX,
			FrameCount * 4,
			CurrentWidth,
			CurrentHeight
			);

		glutSetWindowTitle( TempString );
		free( TempString );
	}
	FrameCount = 0;
	glutTimerFunc( 250, TimerFunction, 1 );
}

void Cleanup( void )
{
	DestroyShaders();
	DestroyVBO();
}

void CreateVBO( void )
{
	Vertex Vertices[] =
	{
		//Origin
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		// Top
		{ { -0.2f, 0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.2f, 0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { 0.0f, 0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		// Bottom
		{ { -0.2f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { 0.2f, -0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.0f, -0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { 0.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		// Left
		{ { -0.8f, -0.2f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.8f, 0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { -0.8f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { -1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		// Right
		{ { 0.8f, -0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { 0.8f, 0.2f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.8f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }
	};

	GLubyte Indices[] = {
		// Top
		0, 1, 3,
		0, 3, 2,
		3, 1, 4,
		3, 4, 2,
		// Bottom
		0, 5, 7,
		0, 7, 6,
		7, 5, 8,
		7, 8, 6,
		// Left
		0, 9, 11,
		0, 11, 10,
		11, 9, 12,
		11, 12, 10,
		// Right
		0, 13, 15,
		0, 15, 14,
		15, 13, 16,
		15, 16, 14
	};

	GLubyte TopIndices[] = {
		// Top
		0, 1, 3,
		0, 3, 2,
		3, 1, 4,
		3, 4, 2
	};
	GLubyte LeftIndices[] = {
		// Left
		0, 9, 11,
		0, 11, 10,
		11, 9, 12,
		11, 12, 10
	};
	GLubyte RightIndices[] = {
		// Right
		0, 13, 15,
		0, 15, 14,
		15, 13, 16,
		15, 16, 14
	};
	GLubyte BottomIndices[] = {
		// Bottom
		0, 5, 7,
		0, 7, 6,
		7, 5, 8,
		7, 8, 6
	};

	GLenum ErrorCheckValue = glGetError();
	const GLsizei BufferSize = sizeof( Vertices );
	const GLsizei VertexSize = sizeof( Vertices[0] );
	const GLsizei RgbOffset = sizeof( Vertices[0].XYZW );

	glGenVertexArrays( 1, &VaoId );
	glBindVertexArray( VaoId );

	glGenBuffers( 1, &VboId );
	glBindBuffer( GL_ARRAY_BUFFER, VboId );
	glBufferData( GL_ARRAY_BUFFER, BufferSize, Vertices, GL_STATIC_DRAW );
	
	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0 );
	glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)RgbOffset );

	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

	glGenBuffers( 5, IndexBufferId );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[0] );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( Indices ), Indices, GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[1] );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( TopIndices ), TopIndices, GL_STATIC_DRAW );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[2] );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( LeftIndices ), LeftIndices, GL_STATIC_DRAW );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[3] );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( RightIndices ), RightIndices, GL_STATIC_DRAW );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[4] );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( BottomIndices ), BottomIndices, GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[0] );

	ErrorCheckValue = glGetError();
	if ( ErrorCheckValue != GL_NO_ERROR )
	{
		fprintf(
			stderr,
			"ERROR: Could not create a VBO: %s \n",
			gluErrorString( ErrorCheckValue )
			);

		exit( -1 );
	}
}

void DestroyVBO( void )
{
	GLenum ErrorCheckValue = glGetError();

	glDisableVertexAttribArray( 1 );
	glDisableVertexAttribArray( 0 );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glDeleteBuffers( 1, &VboId );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glDeleteBuffers( 5, IndexBufferId );

	glBindVertexArray( 0 );
	glDeleteVertexArrays( 1, &VaoId );

	ErrorCheckValue = glGetError();
	if ( ErrorCheckValue != GL_NO_ERROR )
	{
		fprintf(
			stderr,
			"ERROR: Could not destroy the VBO: %s \n",
			gluErrorString( ErrorCheckValue )
			);

		exit( -1 );
	}
}

void CreateShaders( void )
{
	GLenum ErrorCheckValue = glGetError();

	VertexShaderId = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( VertexShaderId, 1, &VertexShader, NULL );
	glCompileShader( VertexShaderId );

	FragmentShaderId = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( FragmentShaderId, 1, &FragmentShader, NULL );
	glCompileShader( FragmentShaderId );

	ProgramId = glCreateProgram();
	glAttachShader( ProgramId, VertexShaderId );
	glAttachShader( ProgramId, FragmentShaderId );
	glLinkProgram( ProgramId );
	glUseProgram( ProgramId );

	ErrorCheckValue = glGetError();
	if ( ErrorCheckValue != GL_NO_ERROR )
	{
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n",
			gluErrorString( ErrorCheckValue )
			);

		exit( -1 );
	}
}

void DestroyShaders( void )
{
	GLenum ErrorCheckValue = glGetError();

	glUseProgram( 0 );

	glDetachShader( ProgramId, VertexShaderId );
	glDetachShader( ProgramId, FragmentShaderId );

	glDeleteShader( FragmentShaderId );
	glDeleteShader( VertexShaderId );

	glDeleteProgram( ProgramId );

	ErrorCheckValue = glGetError();
	if ( ErrorCheckValue != GL_NO_ERROR )
	{
		fprintf(
			stderr,
			"ERROR: Could not destroy the shaders: %s \n",
			gluErrorString( ErrorCheckValue )
			);

		exit( -1 );
	}
}

void KeyboardFunction( unsigned char Key, int X, int Y )
{
	switch ( Key )
	{
		//Displays as wireframe
		case 'W':
		case 'w':
			{
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				break;
			}
		//Displays as filled
		case 'F':
		case 'f':
			{
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				break;
			}
		//Displays only the top spoke 
		case 'T':
		case 't':
			{
				state = INDIVIDUAL_SPOKE;
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[1] );
				break;
			}
		//Displays only the left spoke
		case 'L':
		case 'l':
			{
				state = INDIVIDUAL_SPOKE;
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[2] );
				break;
			}
		//Displays only the right spoke
		case 'R':
		case 'r':
			{
				state = INDIVIDUAL_SPOKE;
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[3] );
				break;
			}
		//Displays only the bottom spoke
		case 'B':
		case 'b':
			{
				state = INDIVIDUAL_SPOKE;
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[4] );
				break;
			}
		//Displays only the bottom spoke
		case 'N':
		case 'n':
			{
				state = NORMAL;
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[0] );
				break;
			}
		default:
			X;Y;
			break;
	}
}