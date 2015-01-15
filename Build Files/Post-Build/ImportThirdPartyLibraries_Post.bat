goto comment
/////////////////////////////////////////////////////////////////////////////
// Name:        	ImportThirdPartyLibraries_Post.bat
// Author:      	Nikolaos Patsiouras
// Email:			npatsiouras@gmail.com
// Created:     	2015, 1, 15, 6:09 PM
// Modified by:		Nikos
// Last Modified:   2015, 1, 15, 10:17 PM
// Copyright:   	(c) 2015 Nikolaos Patsiouras
// Licence:     	MIT License
/////////////////////////////////////////////////////////////////////////////
:comment
if %3 == "Win32" (
	copy %1Dependencies\freeglut\bin\freeglut.dll %1%2\ /Y
	copy %1Dependencies\glew\bin\glew32.dll %1%2\ /Y
) else (
	if %3 == "x64" (
		copy %1Dependencies\freeglut\bin\x64\freeglut.dll %1%3\%2\ /Y
		copy %1Dependencies\glew\bin\x64\glew32.dll %1%3\%2\ /Y
	)
)
