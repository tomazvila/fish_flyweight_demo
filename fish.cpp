#include <GL/glut.h>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <iostream>

// Return a random float in the range 0.0 to 1.0.
GLfloat randomFloat() {
	return (GLfloat)rand() / RAND_MAX;
}

// A fish bitmap, size is 27x11, but all rows must have a multiple of 8 bits,
// so we define it like it is 32x11.

class Position {
private:
	float x;
	float y;
	float z;

public:
	Position() {
		x = randomFloat();
		y = randomFloat();
		z = 0.0;
	}

	void setPosition() {
		glRasterPos3f(x, y, z);
	}

	void moveLeft(float speed) {
		x -= speed;
	}

	float getX() {
		return x;
	}

	void resetX() {
		x = 1.0 + std::abs(x);
	}
};

class Fish {
private: 

	unsigned char bitmap[44] = {
  	0x00, 0x60, 0x01, 0x00,
  	0x00, 0x90, 0x01, 0x00,
  	0x03, 0xf8, 0x02, 0x80,
	0x1c, 0x37, 0xe4, 0x40,
  	0x20, 0x40, 0x90, 0x40,
  	0xc0, 0x40, 0x78, 0x80,
  	0x41, 0x37, 0x84, 0x80,
  	0x1c, 0x1a, 0x04, 0x80,
  	0x03, 0xe2, 0x02, 0x40,
	0x00, 0x11, 0x01, 0x40,
  	0x00, 0x0f, 0x00, 0xe0,
	};

	float color1;
	float color2;
	float color3;
	Position position;
	float speed;

public:
	Fish() {
		generateColor();
		speed = randomFloat() / 10;
	}

	void generateColor() {
		color1 = randomFloat();
		color2 = randomFloat();
		color3 = randomFloat();
	}

	void draw() {
		glColor3f(color1, color2, color3);
		position.setPosition();
		glBitmap(27, 11, 0, 0, 0, 0, bitmap);
	}

	void checkOutOfBounds() {
		if (position.getX() < 0)
			position.resetX();
	}

	void move() {
		position.moveLeft(speed);
	}

};


unsigned int nFishes = 30;
std::vector<Fish> fishes;

// On reshape, uses an orthographic projection with world coordinates ranging
// from 0 to 1 in the x and y directions, and -1 to 1 in z.
void reshape(int width, int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, 1, 0, 1);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  for (auto &fishie : fishes) {
	  fishie.draw();
	  fishie.move();
	  fishie.checkOutOfBounds();
  }
  glFlush();
  usleep(250000);
}

void initFishes() {
	for (int i = 0; i < nFishes; i++) {
		Fish fishie;
		fishes.push_back(fishie);
	}
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Fishies");
  glutReshapeFunc(reshape);
  glutIdleFunc(display);
  initFishes();
  glutMainLoop();
}
