#include <GL/glut.h>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <iterator>

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

class Color {
public:
	float r;
	float g;
	float b;

	Color() {
		r = 0.0;
		g = 0.0;
		b = 0.0;
	}

	void generateColor() {
		r = randomFloat();
		g = randomFloat();
		b = randomFloat();
	}

	void debugColor() {
		std::cout << "r " << r << " g " << g << " b " << b << std::endl;
	}
};

class ColorContainer {
private:
	std::vector<Color> colors;

	template<typename Iter, typename RandomGenerator>
	Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
		std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
		std::advance(start, dis(g));
		return start;
	}

	template<typename Iter>
	Iter select_randomly(Iter start, Iter end) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		return select_randomly(start, end, gen);
	}


public:
	void generateColors(int n) {
		for (unsigned int i = 0; i < n; i++) {
			Color color;
			color.generateColor();
			colors.emplace_back(color);
		}
	}

	Color *getRandomColor() {
		Color *color = &(*select_randomly(colors.begin(), colors.end()));
		return color;
	}
};


ColorContainer cnt;

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

	Color *color;
	Position position;
	float speed;

public:
	Fish() {
		speed = randomFloat() / 100;
	}

	void setColor(Color *col) {
		std::cout << "set color ";
		col->debugColor();
		color = col;
		std::cout << "after set color ";
		color->debugColor();
	}

	void draw() {
		glColor3f(color->r , color->g, color->b);
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

	void debug() {
		color->debugColor();
	}

};


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
  usleep(50000);
}

void initFishes(int n) {
	cnt.generateColors(10);
	Color *col;
	for (int i = 0; i < n; i++) {
		Fish fishie;
		col = cnt.getRandomColor();
		col->debugColor();
		fishie.setColor(col);
		fishes.push_back(fishie);
	}
	std::cout << std::endl;
	for (auto &fishi : fishes) {
		fishi.debug();
	}
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  int n = 500;
  if (argc == 2) 
  	n = atoi(argv[1]);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Fishies");
  glutReshapeFunc(reshape);
  glutIdleFunc(display);
  initFishes(n);
  glutMainLoop();
}
