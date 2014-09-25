#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>

class Feature {
public:
  enum FeatureType {eUnknown, eCircle, eTriangle, eSquare};
  Feature() : type(eUnknown), points(0) {

  }

  ~Feature() {
    if (points)
      delete [] points;         // delete [] для new []
  }

  bool isValid() {
    return type != eUnknown;
  }

  void read(const std::string & filename) {
    std::ifstream fin;
    fin.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);

    if (!fin.is_open()) {
      std::cerr << "Can't open " << filename << " file for input!" << std::endl;
      exit(EXIT_FAILURE);
    }

    fin.exceptions(std::ios_base::eofbit | std::ios_base::badbit | std::ios_base::failbit);
    try {
      fin.read((char*)&type, sizeof(FeatureType));
    }
    catch (std::ios_base::failure & bf) {
      std::cerr << "Read to type: "
                << bf.what() << std::endl;
      if (fin.eof())
        std::cerr << "End file!" << std::endl;
      exit(EXIT_FAILURE);
    }

    short size = 0;
    switch (type) {
      case eCircle:
        size = 3;
        break;
      case eTriangle:
        size = 6;
        break;
      case eSquare:
        size = 8;
        break;
      default:
        type = eUnknown;
        std::cerr << "Bad type: eUnknown" << std::endl;
        exit(EXIT_FAILURE);
    }

    try {
      points = new double[size];
    }
    catch (std::bad_alloc & ba) {
      std::cerr << ba.what() << std::endl;
    }

    try {
      fin.read((char*)points, sizeof(double) * size);
    }
    catch (std::ios_base::failure & bf) {
      std::cerr << "Read to points: "
                << bf.what() << std::endl;
      if (fin.eof())
        std::cerr << "End file!" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  void draw() {
    switch (type) {
      case eCircle:
        drawCircle(points[0], points[1], points[2]);
        break;
      case eTriangle:
        drawPoligon(points, 6);
        break;
      case eSquare:
        drawPoligon(points, 8);
        break;
    }
  }

protected:
  // заданы только прототипы методов - нет реализации
  void drawCircle(double centerX, double centerY, double radius);
  void drawPoligon(double * points, int size); // метод должен принимать указатель на массив
  double * points;
  FeatureType type;
};

int main(int argc, char** argv) {
  Feature feature;
  feature.read("features.dat");

  return 0;
}
