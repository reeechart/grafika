#include "Reader.cpp"
#include "Renderer.cpp"
#include "Component.cpp"
#include "Canvas.cpp"

#define SCALE_TIME_HIT 7.0
#define ERROR_TIME_HIT 0.1

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
    parachute.setColor(Color(255, 255, 0));
    cannonBall.setColor(Color(240, 120, 60));

    // Copy propeller and wheel
    propellerRight = propellerLeft;
    wheelRight = wheelLeft;

    // Scale the cannonBall and parachute;
    cannonBall.scale(Point(25, 0), 25, 25);
    parachute.scale(Point(), 5, 5);

    // Set the right wheel velocity and acceleration
    wheelRight.setVelocity(4, 0);
    wheelRight.setAcceleration(0, 0.5);
    wheelRight.setSpringConstant(0.8);

    // Set initial position for each component
    airplane.translate(500, 400);
    propellerLeft.translate(518, 405);
    propellerRight.translate(618, 405);
    wheelLeft.translate(571, 449);
    wheelRight.translate(591, 449);
    cannonBall.translate(618, 800);
    parachute.translate(200, -200);

    // Render every component
    Point origin(584, 500);
    Point ballOrigin(653, 0);
    float scale = 1;
    float rotation = 0;
    while (1) {
        scale += 0.1;
        rotation += 10;

        float ballScale = (SCALE_TIME_HIT - scale) / 5 + 0.1;

        Component scaledAirplane = airplane;
        Component scaledPropellerLeft = propellerLeft, scaledPropellerRight = propellerRight;
        Component scaledWheelLeft = wheelLeft, scaledWheelRight = wheelRight;
        Component scaledCannonBall = cannonBall;

        // Scale components
        scaledAirplane.scale(origin, scale, scale);
        scaledPropellerLeft.scale(origin, scale, scale);
        scaledPropellerRight.scale(origin, scale, scale);
        scaledWheelLeft.scale(origin, scale, scale);
        if (scale < SCALE_TIME_HIT)
            scaledWheelRight.scale(origin, scale, scale);
        else if (scale < SCALE_TIME_HIT + ERROR_TIME_HIT)
            wheelRight.scale(origin, scale, scale);

        scaledCannonBall.scale(ballOrigin, ballScale, ballScale);
        scaledCannonBall.translate(27, 80);

        // When the cannonball hits the wheel
        if (scale >= SCALE_TIME_HIT) {
            wheelRight.update();
            if (wheelRight.getBottomRightPosition().getY() > V_SIZE) {
                wheelRight.bounce();
                wheelRight.update();
            }

            parachute.translate(0, 2);
        }

        // Rotate propellers
        scaledPropellerLeft.rotateAgainstCenter(rotation);
        scaledPropellerRight.rotateAgainstCenter(rotation);        

        if ((int) scale % 2 == 0) {
            parachute.rotateAgainstCenter(1);
        } else {
            parachute.rotateAgainstCenter(-1);
        }

        canvas.clear();
        renderer.renderToCanvas(scaledAirplane, &canvas);
        renderer.renderToCanvas(scaledPropellerLeft, &canvas);
        renderer.renderToCanvas(scaledPropellerRight, &canvas);
        renderer.renderToCanvas(scaledWheelLeft, &canvas);
        if (scale < SCALE_TIME_HIT) {
            renderer.renderToCanvas(scaledCannonBall, &canvas);
            renderer.renderToCanvas(scaledWheelRight, &canvas);
        } else {
            renderer.renderToCanvas(wheelRight, &canvas);
        }
        renderer.renderToCanvas(parachute, &canvas);

        renderer.copyToFrameBuffer(canvas);
    }

    while(1);
    return 0;
}
