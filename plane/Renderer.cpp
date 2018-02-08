#include "Component.cpp"
#include "Canvas.cpp"

#include <iostream>
using namespace std;

class Renderer {
  public:
    void renderToCanvas(Component component, Canvas *canvas) {
      for(auto& line : component.getPlane().getLines() ) {
        for(auto& point : line.getAllPoints()) {
          canvas->setColor(point.getY(), point.getX(), component.getColor());
        }
      }
    }
    void fillComponentColor(Component component, Canvas *canvas) {
      
    }
};

int main() {
  Plane plane;
  Component component;
  Canvas canvas;
  Renderer renderer;

  plane.addLine(Line(Point(0,0), Point(100,500)));
  plane.addLine(Line(Point(100,500), Point(500,20)));
  plane.addLine(Line(Point(500,20), Point(0,0)));
  component.setPlane(plane);
  component.setColor(Color(255,255,255));
  component.setTopLeftPosition(Point(0,0));
  component.setBottomRightPosition(Point(600,600));

  renderer.renderToCanvas(component,&canvas);

  for (int i = 0; i < 760; i++) {
    for (int j = 0; j < 1366; j++) {
      cout << canvas.getColor(i,j).getRedValue() << " ";
    }
    cout << endl;
  }
}
