#ifndef LAYER_CPP
#define LAYER_CPP

#include <iostream>
#include <fstream>
#include <vector>
#include "Component.cpp"
#include "Line.cpp"

using namespace std;

class Layer {
    private:
        vector<Component> components;
        bool visibility;

    public:
        //ctor
        Layer() {
            visibility = true;
        }

        //cctor
        Layer(Layer& layer) {
            this->components = layer.components;
            this->visibility = layer.visibility;
        }

        Layer& operator=(const Layer& layer) {
            this->components = layer.components;
            this->visibility = layer.visibility;
        }

        void addComponent(Component& component) {
            components.push_back(component);
        }

        bool isVisible() {
            return visibility;
        }

        void clip(ClippingPlane& clippingPlane) {
            for (auto& component : components) {
                component = component.clip(clippingPlane);
            }
        }

        vector<Component> getComponents() {
            return components;
        }

        void setVisibility(bool visibility) {
            this->visibility = visibility;
        }

        void translate(int dx, int dy) {
            for (auto& component : components) {
                component.translate(dx, dy);
            }
        }

        void scale(Point center, float scaleX, float scaleY) {
            for (auto& component : components) {
                component.scale(center, scaleX, scaleY);
            }
        }
};

#endif
