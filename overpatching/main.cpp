#include <iostream>
using namespace std;
#include <cstdio>
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

  bool read(FILE * file) {
    // fread возвращает не размер типа прочитанных данных, а количество
    // прочитанных данных заданного типа
    if (fread(&type, sizeof(FeatureType), 1, file) != 1) //sizeof(FeatureType))
      return false;
    size_t size = 0;
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
        return false;
    }

    points = new double[size];
    if (!points)
      return false;
    // points - указатель на первый элемент массива, т.е. он сам является указателем
    return fread(points, sizeof(double), size, file) == size; // size * sizeof(double);
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
  void drawPoligon(double * points, int size);
  double * points;
  FeatureType type;
};

int main(int argc, char** argv) {
  Feature feature;
  // добавлена обработка ошибок
  FILE * file;
  if ((file = fopen("features.dat", "r")) == NULL) {
    perror("open file");
    exit(EXIT_FAILURE);
  }

  if (!feature.read(file)) {
    perror("fread");
    exit(EXIT_FAILURE);
  }

  if (!feature.isValid())
    return 1;
  return 0;
}
