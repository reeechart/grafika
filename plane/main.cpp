#include "Reader.cpp"
#include "Renderer.cpp"
#include "Component.cpp"
#include "Canvas.cpp"
#include <pthread.h>

#define SCALE_TIME_HIT 7.0
#define ERROR_TIME_HIT 0.1

bool isZoomed = false;
pthread_t tid[2];

void* readInput(void *arg) {
    while (1) {
        if (cin.get()) {
            isZoomed = !isZoomed;        }
    }

    return NULL;
}


int main() {
    Component airplane;
    Component propellerLeft, propellerRight;
    Component wheelLeft, wheelRight;
    Component parachute;
    Component cannonBall;
    Component windows;
    Component pilot;
    Reader reader;
    Renderer renderer;
    Canvas canvas;
    ClippingPlane clippingPlane(200, 500, 180, 1100);

    // Read components from txt
    reader.readComponent(&airplane, "assets/airplane.txt");
    reader.readComponent(&propellerLeft, "assets/propeller.txt");
    reader.readComponent(&wheelLeft, "assets/wheel.txt");
    reader.readComponent(&parachute, "assets/parachute.txt");
    reader.readComponent(&cannonBall, "assets/wheel.txt");
    reader.readComponent(&windows, "assets/windows.txt");
    reader.readComponent(&pilot, "assets/wheel.txt");

    // Set color for each components
    airplane.setColor(Color(255, 255, 255));
    propellerLeft.setColor(Color(255, 0, 0));
    wheelLeft.setColor(Color(120, 120, 120));
    parachute.setColor(Color(255, 255, 0));
    cannonBall.setColor(Color(240, 120, 60));
    windows.setColor(Color(201,214,255));
    pilot.setColor(Color(0,0,0));

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
    windows.translate(500,400);
    pilot.translate(572,412);
    propellerLeft.translate(518, 405);
    propellerRight.translate(618, 405);
    wheelLeft.translate(571, 449);
    wheelRight.translate(591, 449);
    cannonBall.translate(620, 800);
    parachute.translate(200, -200);

    // another thread to read input
    pthread_create(&(tid[0]), NULL, &readInput, NULL);

    // Render every component
    Point origin(584, 500);
    Point ballOrigin(653, 0);
    float scale = 1;
    float rotation = 0;
    bool hit = false;
    bool once = false;
    float zoom = 1.5;
    while (1) {
        scale += 0.1;
        rotation += 10;

        float ballScale = (SCALE_TIME_HIT - scale) / 5 + 0.1;

        Component scaledAirplane = airplane;
        Component scaledWindows = windows;
        Component scaledPilot = pilot;
        Component scaledPropellerLeft = propellerLeft, scaledPropellerRight = propellerRight;
        Component scaledWheelLeft = wheelLeft, scaledWheelRight = wheelRight;
        Component scaledCannonBall = cannonBall;

        // Scale components
        scaledAirplane.scale(origin, scale, scale);
        scaledWindows.scale(origin, scale,scale);
        scaledPilot.scale(origin, scale,scale);
        scaledPropellerLeft.scale(origin, scale, scale);
        scaledPropellerRight.scale(origin, scale, scale);
        scaledWheelLeft.scale(origin, scale, scale);


        scaledCannonBall.scale(ballOrigin, ballScale, ballScale);
        scaledCannonBall.translate(27, 80);

        if (!hit) {
            scaledWheelRight.scale(origin, scale, scale);
            Point topLeftWheel = scaledWheelRight.getTopLeftPosition();
            Point bottomRightWheel = scaledWheelRight.getBottomRightPosition();
            // Point topRightWheel(bottomRightWheel.getX(), topLeftWheel.getY());
            // Point bottomLeftWheel(topLeftWheel.getX(), bottomRightWheel.getY());
            Point topLeftCannonball = scaledCannonBall.getTopLeftPosition();
            Point bottomRightCannonball = scaledCannonBall.getBottomRightPosition();
            Point topRightCannonball(bottomRightCannonball.getX(), topLeftCannonball.getY());
            Point bottomLeftCannonball(topLeftCannonball.getX(), bottomRightCannonball.getY());
            // cout << "cannon" << topRightCannonball.getX() << "," << topRightCannonball.getY() << endl;
            if (topRightCannonball.getX() > topLeftWheel.getX() && topRightCannonball.getX() < bottomRightWheel.getX() &&
                topRightCannonball.getY() > topLeftWheel.getY() && topRightCannonball.getY() < bottomRightWheel.getY()) {
                hit = true;
                once = true;
            }
        } else if (once) {
            wheelRight.scale(origin, scale, scale);
            once = false;
        }
        // if (scale < SCALE_TIME_HIT)
        //     scaledWheelRight.scale(origin, scale, scale);
        // else if (scale < SCALE_TIME_HIT + ERROR_TIME_HIT)
        //     wheelRight.scale(origin, scale, scale);

        // When the cannonball hits the wheel
        if (/*scale >= SCALE_TIME_HIT*/ hit) {
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

        Component scaledParachute = parachute;

        if (isZoomed) {
        	scaledAirplane.scale(origin, zoom, zoom);
        	scaledWindows.scale(origin, zoom, zoom);
        	scaledPilot.scale(origin, zoom, zoom);
        	scaledPropellerLeft.scale(origin, zoom, zoom);
	        scaledPropellerRight.scale(origin, zoom, zoom);
	        scaledWheelLeft.scale(origin, zoom, zoom);
            scaledWheelRight.scale(origin, zoom, zoom);
	        scaledCannonBall.scale(origin, zoom, zoom);
	        scaledParachute.scale(origin, zoom, zoom);
        }

        canvas.clear();
        renderer.renderToCanvas(scaledAirplane.clip(clippingPlane), &canvas);
        renderer.renderToCanvas(scaledWindows.clip(clippingPlane), &canvas);
        renderer.renderToCanvas(scaledPilot.clip(clippingPlane), &canvas);
        renderer.renderToCanvas(scaledPropellerLeft.clip(clippingPlane), &canvas);
        renderer.renderToCanvas(scaledPropellerRight.clip(clippingPlane), &canvas);
        renderer.renderToCanvas(scaledWheelLeft.clip(clippingPlane), &canvas);
        if (/*scale < SCALE_TIME_HIT*/ !hit) {
            renderer.renderToCanvas(scaledCannonBall.clip(clippingPlane), &canvas);
            renderer.renderToCanvas(scaledWheelRight.clip(clippingPlane), &canvas);
        } else {
            renderer.renderToCanvas(scaledWheelRight.clip(clippingPlane), &canvas);
        }
        renderer.renderToCanvas(scaledParachute.clip(clippingPlane), &canvas);

        renderer.copyToFrameBuffer(canvas);

        usleep(10000);
    }

    while(1) {
    	char x;
    	cin >> x;
    }
    return 0;
}
