if %3 == "Win32" (
	copy %1Dependencies\freeglut\bin\freeglut.dll %1%2\ /Y
	copy %1Dependencies\glew\bin\glew32.dll %1%2\ /Y
) else (
	if %3 == "x64" (
		copy %1Dependencies\freeglut\bin\x64\freeglut.dll %1%3\%2\ /Y
		copy %1Dependencies\glew\bin\x64\glew32.dll %1%3\%2\ /Y
	)
)