#include <GL/glut.h>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <iterator>
#include <cmath>

// Return a random float in the range 0.0 to 1.0.
GLfloat randomFloat() {
	return (GLfloat)rand() / RAND_MAX;
}

class Position {
private:
	float x;
	float y;
	float z;

	const double pi = std::acos(-1);
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
		//y = std::sin(pi * x * 2);
	}

	float getX() {
		return x;
	}

	void resetX() {
		x = 1.0 + std::abs(x);
	}
};

class Color {
private:
	int *spaceTaker = nullptr;
public:
	float r;
	float g;
	float b;

	Color() {
		r = 0.0;
		g = 0.0;
		b = 0.0;
		spaceTaker = static_cast<int*>(malloc(10000000));
	}

	void generateColor() {
		r = randomFloat();
		g = randomFloat();
		b = randomFloat();
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
			colors.push_back(color);
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
		color = col;
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

void initFishes(int n, int colors) {
	cnt.generateColors(colors);
	Color *col;
	for (int i = 0; i < n; i++) {
		Fish fishie;
		col = cnt.getRandomColor();
		fishie.setColor(col);
		fishes.push_back(fishie);
	}
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  int n = 500;
  int colors = 10;
  if (argc > 1) 
  	n = atoi(argv[1]);
  if (argc > 2)
	colors = atoi(argv[2]);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Fishies");
  glutReshapeFunc(reshape);
  glutIdleFunc(display);
  initFishes(n, colors);
  glutMainLoop();
}
