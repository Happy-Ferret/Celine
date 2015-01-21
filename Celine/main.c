/////////////////////////////////////////////////////////////////////////////
// Name:        	Test.c
// Purpose:     
// Author:      	Nikolaos Patsiouras
// Email:			npatsiouras@gmail.com
// Created:     	2015, 1, 19, 2:32 PM
// Modified by:		Nikos
// Last Modified:   2015, 1, 19, 2:32 PM
// Copyright:   	(c) 2015 Nikolaos Patsiouras
// Licence:     	MIT License
/////////////////////////////////////////////////////////////////////////////

#include "Utils.h"

#define WINDOW_TITLE_PREFIX "TestWindow"

int WindowHandle = 0, CurrentWidth = 600, CurrentHeight = 600;

Matrix ProjectionMatrix, ViewMatrix, ModelMatrix;

GLuint VAO_Ids[1] = { 0 },
		VBO_Ids[2] = { 0 },
		ShaderProgram_Ids[1] = { 0 },
		Shader_Ids[2] = { 0 },
		TBO_Ids[1] = { 0 };

void Initialize( int, char*[] );
void InitWindow( int, char*[] );
void RenderFunction( void );
void ResizeFunction( int, int );
void KeyboardFunction( unsigned char, int, int );
void LoadShaders();
void LoadMesh();

int main( int argc, char** argv )
{
	Initialize( argc, argv );
	glutMainLoop();
	exit( EXIT_SUCCESS );
}

void Initialize( int argc, char* argv[] )
{
	//Initialize FREEGLUT and CREATE A WINDOW
	InitWindow( argc, argv );

	//Initialize GLEW
	GLenum GlewInitResult = glewInit();
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
	
	//Clear the error collection
	glGetError();

	//Set the clear color to be black which also seems to be the default
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	ModelMatrix = IDENTITY_MATRIX;
	ProjectionMatrix = IDENTITY_MATRIX;
	ViewMatrix = IDENTITY_MATRIX;

	//Load shaders
	LoadShaders();
	//Load Mesh
	LoadMesh();
	//Load textures
}

void InitWindow( int argc, char* argv[] )
{
	glutInit( &argc, argv );
	glutInitContextVersion( 4, 0 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );

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
	glutKeyboardFunc( KeyboardFunction );
}

void RenderFunction()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glutSwapBuffers();
	glutPostRedisplay();
}

void ResizeFunction( int Width, int Height )
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport( 0, 0, CurrentWidth, CurrentHeight );
}

void KeyboardFunction( unsigned char Key, int MouseX, int MouseY )
{
	switch ( Key )
	{
		case 'F':
		case 'f':
			{
				glutFullScreenToggle();
			}
			break;
		default:
			MouseX; MouseY;
	}
}

char* LoadShaderSourceFromFile( const char* filepath )
{
	char *data = NULL;
	FILE *file = NULL;
	int file_size = 0;
	
	if ( NULL != (file = fopen( filepath, "rb" )) &&
		0 == fseek( file, 0, SEEK_END ) &&
		-1 != (file_size = ftell( file )) )
	{
		rewind( file );
		if ( NULL != (data = (char*)malloc( file_size + 1 )) )
		{
			if ( file_size == (long)fread( data, sizeof( char ), file_size, file ) )
			{
				data[file_size] = '\0';
				return data;
			}
			else
				fprintf( stderr, "ERROR: Could not read file %s\n", filepath );

			free( data );
		}
		else
			fprintf( stderr, "ERROR: Could not allocate %i bytes.\n", file_size );

		fclose( file );
	}
	else
	{
		if ( NULL != file )
			fclose( file );
		fprintf( stderr, "ERROR: Could not open file %s\n", filepath );
	}

	return NULL;
}

void LoadShaders()
{
	char* Shader_data[2] = { 0 };

	//Load from Shader Files into the "buffers"
	Shader_data[0] = LoadShaderSourceFromFile( "SimpleShader.vertex.glsl" );
	Shader_data[1] = LoadShaderSourceFromFile( "SimpleShader.fragment.glsl" );
	//Create Program
	ShaderProgram_Ids[0] = glCreateProgram();
	//Create Shader
	Shader_Ids[0] = glCreateShader( GL_VERTEX_SHADER );
	Shader_Ids[1] = glCreateShader( GL_FRAGMENT_SHADER );
	//Load from Source
	GLsizei Shader_dataSizes[2];
	Shader_dataSizes[0] = sizeof( Shader_data[0] );
	Shader_dataSizes[1] = sizeof( Shader_data[1] );
	glShaderSource( Shader_Ids[0], 1, &Shader_data[0], &Shader_dataSizes[0]);
	glShaderSource( Shader_Ids[1], 1, &Shader_data[1], &Shader_dataSizes[1]);
	//Compile Shader2
	glCompileShader( Shader_Ids[0] );
	glCompileShader( Shader_Ids[1] );
	//Attach Shader
	glAttachShader( ShaderProgram_Ids[0], Shader_Ids[0] );
	glAttachShader( ShaderProgram_Ids[0], Shader_Ids[1] );
	//Link Program with Shaders
	glLinkProgram( ShaderProgram_Ids[0] );
	//Use Program
	glUseProgram( ShaderProgram_Ids[0] );

	free( Shader_data[1] );
	free( Shader_data[0] );
}

void LoadMesh()
{
	//Generate VAO
	//Bind VAO
	 

	//Generate 3 VBOs
	//Bind Vertices VBO
	//Load Vertices to VBO
	//Bind TextureCoords VBO
	//Load Texture Coords to VBO
	//Bind PerVertexColor VBO
	//Load Per Vertex Colors

	//Set attributes

	//Get pointers to shader inputs

	//Link attributes to Shader inputs

	//Bind Vertices VBO
}

void LoadTextures()
{
	//Create Texture Buffer
	//Set it Active
	//Load from File

}