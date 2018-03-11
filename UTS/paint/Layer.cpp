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
        short top;
        short bottom;
        short left;
        short right;

        //ctor
        Layer() {
            visibility = true;
        }

        //cctor
        Layer(Layer& layer) {
            this->components = layer.components;
            this->visibility = layer.visibility;
            this->top = layer.top;
            this->bottom = layer.bottom;
            this->left = layer.left;
            this->right = layer.right;
        }

        Layer& operator=(const Layer& layer) {
            this->components = layer.components;
            this->visibility = layer.visibility;
            this->top = layer.top;
            this->bottom = layer.bottom;
            this->left = layer.left;
            this->right = layer.right;
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
            cout << top << "-" << left << "-" << bottom << "-" << right << endl;
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

        void setEnvelope(int top, int bottom, int left, int right) {
            this->top = top;
            this->bottom = bottom;
            this->left = left;
            this->right = right;
        }
};

#endif
