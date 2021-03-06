#include "pch.h"
#include <iostream>
#include <cmath>
#include <vector>

#define PI 3.14

struct RGB
{
	int red;
	int green;
	int blue;
};

struct Point
{
	double x;
	double y;
};




class Shape
{
private:
	static int s_ID;
protected:	
	RGB m_color;
	int m_ID;
	Point m_center;
	std::vector <Point> coordinates;
public:
	Shape(RGB color, Point center) :m_color(color), m_center(center)
	{
		m_ID=s_ID++;
	}

	//перемещение
	void move(Point a) 
	{
		double moving_x = a.x - m_center.x;
		double moving_y = a.y - m_center.y;
		m_center = a;
		for (size_t i = 0; i < coordinates.size(); i++)
		{
			coordinates[i].x += moving_x;
			coordinates[i].y += moving_y;
		}
		
	}
//поворот
	void rotate(double angle)
	{
		//перевод в радианы
		double angle_rad = angle * PI / 180.0;
		for (size_t i = 0; i < coordinates.size(); i++)
		{
			coordinates[i].x = m_center.x + (coordinates[i].x - m_center.x)*cos(angle_rad) - (coordinates[i].y - m_center.y)*sin(angle_rad);
			coordinates[i].y = m_center.y + (coordinates[i].x - m_center.x)*sin(angle_rad) + (coordinates[i].y - m_center.y)*cos(angle_rad);
		}
	}

	//масштабирование
	virtual void scaling(double scale) = 0;

	//установка цвета
	void set_color(const RGB& color)
	{
		m_color = color;
	}
	//получение цвета
	RGB& get_color()
	{
		return m_color;
	}
	Point get_center()
	{
		return m_center;
	}
	unsigned int get_ID()
	{
		return m_ID;
	}

	virtual std::ostream& print_shape(std::ostream& stream, Shape& shape) = 0;

	friend std::ostream& operator << (std::ostream& stream, Shape& shape) 
	{
		return shape.print_shape(stream, shape);
	}
};

int Shape::s_ID = 0;

class Parallelogram :public Shape {
	Point m_p1;
	Point m_p2;
	Point m_p3;
public:
	Parallelogram(RGB color, Point p1, Point p2, Point p3) :Shape(color, m_center) {
		m_p1 = p1;
		m_p2 = p2;
		m_p3 = p3;
		coordinates.push_back(p1);
		coordinates.push_back(p2);
		coordinates.push_back(p3);
		coordinates.push_back(Point{ coordinates[0].x + (coordinates[2].x - coordinates[1].x), coordinates[2].y - (coordinates[1].y - coordinates[0].y) });
		m_center.x = (coordinates[0].x + coordinates[2].x) / 2;
		m_center.y = (coordinates[0].y + coordinates[2].y) / 2;
	}

	void scaling(double scale) override {
	
		for (size_t i = 0; i < coordinates.size(); i++) 
		{
			coordinates[i].x= m_center.x + (coordinates[i].x - m_center.x)*scale;
			coordinates[i].y = m_center.y + (coordinates[i].y - m_center.y)*scale;
		}
	}

	std::ostream& print_shape(std::ostream& stream, Shape& shape) override {
		stream << "Figure: Parallelogram" << std::endl;
		stream << "ID: " << shape.get_ID() << std::endl;
		stream << "Coordinates: " << std::endl;
		stream << "Center: (" << shape.get_center().x << ";" << shape.get_center().y << ")" << std::endl;
		stream << "Vertexes:" << std::endl;
		for (size_t i = 0; i < coordinates.size(); i++) 
		{
			stream << "(" << coordinates[i].x << ";" << coordinates[i].y << ")" << std::endl;
		}
		stream << "Color: " << shape.get_color().red << " " << shape.get_color().green << " " << shape.get_color().blue << std::endl;
		stream << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
		return stream;
	}

};
class Square : public Shape
{
	double m_lenght;
public:
	Square(double lenght, RGB color, Point center, double angle) :Shape(color, center)
	{
		m_lenght = lenght;
		coordinates.push_back(Point{center.x-lenght/2,center.y-lenght/2});
		coordinates.push_back(Point{ center.x - lenght / 2,center.y + lenght / 2 });
		coordinates.push_back(Point{ center.x + lenght / 2,center.y + lenght / 2 });
		coordinates.push_back(Point{ center.x + lenght / 2,center.y - lenght / 2 });
		rotate(angle);
	}
	void scaling(double scale) override
	{
		for (size_t i = 0; i < coordinates.size(); i++)
		{
			coordinates[i].x = m_center.x + (coordinates[i].x - m_center.x)*scale;
			coordinates[i].y = m_center.y + (coordinates[i].y - m_center.y)*scale;

		}
	}
	std::ostream& print_shape(std::ostream& stream, Shape& shape) override {
		stream << "Figure: Square" << std::endl;
		stream << "ID: " << shape.get_ID() << std::endl;
		stream << "Coordinates: " << std::endl;
		stream << "Center: (" << shape.get_center().x << ";"<< shape.get_center().y<< ")"<< std::endl;
		stream << "Vertexes:" << std::endl;
		for (size_t i = 0; i < coordinates.size(); i++) {
			stream << "(" << coordinates[i].x << ";" << coordinates[i].y << ")" << std::endl;
		}
		stream << "Color: " << shape.get_color().red << " " << shape.get_color().green << " " << shape.get_color().blue << std::endl;
		stream << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
		return stream;
	}
};
class Rhombus : public Shape {
	double m_diagonal1;
	double m_diagonal2;
public:
	Rhombus(double diagonal1, double diagonal2, Point p1, RGB color) :Shape(color, m_center) 
	{
		m_diagonal1 = diagonal1;
		m_diagonal2 = diagonal2;
		coordinates.push_back(p1);
		coordinates.push_back(Point{ p1.x - diagonal2 / 2, p1.y + diagonal1 / 2 });
		coordinates.push_back(Point{ p1.x, p1.y + diagonal1 });
		coordinates.push_back(Point{ p1.x + diagonal2 / 2, p1.y + diagonal1 / 2 });
		m_center.x = (coordinates[1].x + coordinates[3].x) / 2;
		m_center.y = (coordinates[0].y + coordinates[2].y) / 2;
	}
	void scaling(double scale) override {
		for (size_t i = 0; i < coordinates.size(); i++) 
		{
			coordinates[i].x = m_center.x + (coordinates[i].x - m_center.x)*scale;
			coordinates[i].y = m_center.y + (coordinates[i].y - m_center.y)*scale;
		}
	}
	std::ostream& print_shape(std::ostream& stream, Shape& shape) override 
	{
		stream << "Figure: Rhombus" << std::endl;
		stream << "ID: " << shape.get_ID() << std::endl;
		stream << "Coordinates: " << std::endl;
		stream << "Center: (" << shape.get_center().x << ";" << shape.get_center().y << ")" << std::endl;
		stream << "Vertexes:" << std::endl;
		for (size_t i = 0; i < coordinates.size(); i++)
		{
			stream << "(" << coordinates[i].x << ";" << coordinates[i].y << ")" << std::endl;
		}
		stream << "Color: " << shape.get_color().red << " " << shape.get_color().green << " " << shape.get_color().blue << std::endl;
		stream << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
		return stream;
	}
};

void fun(std::vector<Shape*>& arr) {
	for (int i = 0; i < arr.size(); i++)
	{
		arr[i]->scaling(2);
	}
}

int main() {

	std::vector <Shape*> figures;
	
	Square test1(2, { 255, 255, 255 }, { 1, 2 },30);
	//std::cout << test1;
	test1.scaling(2);
	//std::cout << test1;
	Parallelogram test2({ 128,35,127 }, { 1, 5 }, { -1,9 }, { 7,9 });
	//std::cout << test2;
	test2.scaling(2);
	//std::cout << test2;
	Rhombus test3(7, 3, { 0,0 }, { 123, 23, 77 });
	//std::cout << test3;
	test3.scaling(2);
	//std::cout << test3;

	figures.push_back(new Square (2, { 255, 255, 255 }, { 1, 2 }, 30));
	figures.push_back(new Parallelogram ({ 128,35,127 }, { 1, 5 }, { -1,9 }, { 7,9 }));
	figures.push_back(new Rhombus (7, 3, { 0,0 }, { 123, 23, 77 }));

	for (int i = 0; i < figures.size(); i++)
	{
		std::cout << *figures[i]<< std::endl;
	} 

	fun(figures);

	for (int i = 0; i < figures.size(); i++)
	{
		std::cout << *figures[i]<<std::endl;
	}

	for (int i = 0; i < figures.size(); i++)
	{
		delete figures[i];
	}

	return 0;
}