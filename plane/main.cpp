#include "Reader.cpp"
#include "Renderer.cpp"
#include "Component.cpp"
#include "Canvas.cpp"


int main() {
    Component airplane;
    Component propellerLeft, propellerRight;
    Component wheelLeft, wheelRight;
    Component parachute;
    Component cannonBall;
    Reader reader;
    Renderer renderer;
    Canvas canvas;

    // Read components from txt
    reader.readComponent(&airplane, "assets/airplane.txt");
    reader.readComponent(&propellerLeft, "assets/propeller.txt");
    reader.readComponent(&wheelLeft, "assets/wheel.txt");
    reader.readComponent(&parachute, "assets/parachute.txt");
    reader.readComponent(&cannonBall, "assets/wheel.txt");

    // Set color for each components
    airplane.setColor(Color(255, 255, 255));
    propellerLeft.setColor(Color(255, 0, 0));
    wheelLeft.setColor(Color(120, 120, 120));
    parachute.setColor(Color(0, 255, 0));
    cannonBall.setColor(Color(30, 30, 30));

    // Copy propeller and wheel
    propellerRight = propellerLeft;
    wheelRight = wheelLeft;

    // Scale the cannonBall;
    cannonBall.scale(Point(0, 0), 10, 10);

    // Set initial position for each component
    airplane.translate(500, 400);
    propellerLeft.translate(518, 405);
    propellerRight.translate(618, 405);
    wheelLeft.translate(571, 450);
    wheelRight.translate(591, 450);
    cannonBall.translate(627, 800);

    // Render every component
    Point origin(584, 500);
    float scale = 1;
    float rotation = 0;
    while (1) {
        scale += 0.1;
        rotation += 10;
        Component scaledAirplane = airplane;
        Component scaledPropellerLeft = propellerLeft, scaledPropellerRight = propellerRight;
        Component scaledWheelLeft = wheelLeft, scaledWheelRight = wheelRight;

        // Scale components
        scaledAirplane.scale(origin, scale, scale);
        scaledPropellerLeft.scale(origin, scale, scale);
        scaledPropellerRight.scale(origin, scale, scale);
        scaledWheelLeft.scale(origin, scale, scale);
        scaledWheelRight.scale(origin, scale, scale);

        // Rotate propellers
        scaledPropellerLeft.rotateAgainstCenter(rotation);
        scaledPropellerRight.rotateAgainstCenter(rotation);

        // Move cannon ball
        cannonBall.translate(0, -10);

        canvas.clear();
        renderer.renderToCanvas(scaledAirplane, &canvas);
        renderer.renderToCanvas(scaledPropellerLeft, &canvas);
        renderer.renderToCanvas(scaledPropellerRight, &canvas);
        renderer.renderToCanvas(scaledWheelLeft, &canvas);
        renderer.renderToCanvas(scaledWheelRight, &canvas);
        renderer.renderToCanvas(cannonBall, &canvas);
        renderer.copyToFrameBuffer(canvas);
    }

    while(1);
    return 0;
}
