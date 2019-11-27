LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL

triangle: triangle.c initShader.o
	gcc -o triangle triangle.c initShader.o $(LIBS)

triangle_ctm: triangle_ctm.c initShader.o
	gcc -o triangle_ctm triangle_ctm.c initShader.o $(LIBS)

mazeviewer: mazeviewer.c initShader.o
	gcc -o mazeviewer mazeviewer.c initShader.o $(LIBS)

cubes_ctm: cubes_ctm.c initShader.o
	gcc -o cubes_ctm cubes_ctm.c initShader.o $(LIBS)

lighting: lighting.c initShader.o
	gcc -o lighting lighting.c initShader.o $(LIBS)

rubik: rubik.c initShader.o
	gcc -o rubik rubik.c initShader.o $(LIBS)

initShader.o: initShader.c initShader.h
	gcc -c initShader.c

test:
	gcc test.c test

install-deps:
	sudo apt install gcc g++ libglew-dev freeglut3-dev libxi-dev libxmu-dev mesa-utils -y
