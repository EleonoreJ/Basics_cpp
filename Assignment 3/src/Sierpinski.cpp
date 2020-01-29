#include "Sierpinski.h"
#include "error.h"
using namespace std;

/**
 * Draws a triangle with the specified corners in the specified window. Feel free
 * to edit this function as you see fit to change things like color, fill style,
 * etc. in whatever way you'd like!
 *
 * @param window The window in which to draw the triangle.
 * @param x0 y0 The first corner of the triangle.
 * @param x1 y1 The second corner of the triangle.
 * @param x2 y2 The third corner of the triangle.
 */
void drawTriangle(GWindow& window,
                  double x0, double y0,
                  double x1, double y1,
                  double x2, double y2) {
    window.setColor("black");
    window.fillPolygon({ x0, y0, x1, y1, x2, y2 });
}

Vector<double> computeMidpoint(double x0, double y0,
                               double x1, double y1) {
    double newX = (x0+x1)/2;
    double newY = (y0+y1)/2;
    return {newX, newY};
}

/**
 * Draws a Sierpinski triangle with the specified corners.
 *
 * An order-0 Sierpinski triangle is just a regular filled triangle. An order-n Sierpinski
 * triangle, for n > 0, consists of three smaller Sierpinski triangles of order n-1, each half
 * as wide and as tall as the original triangle, positioned so that they meet corner-to-corner.
 *
 * The order of the provided points is irrelevant.
 *
 * If the order provided to this function is negative, this function should call error() to report
 * an error.
 *
 * @param window The window in which to draw the triangle.
 * @param x0, y0 The x and y coordinates of the first corner of the triangle.
 * @param x1, y1 The x and y coordinates of the second corner of the triangle.
 * @param x2, y2 The x and y coordinates of the third corner of the triangle.
 * @param order  The order of the triangle, which will always be nonnegative.
 */
void drawSierpinskiTriangle(GWindow& window,
                            double x0, double y0,
                            double x1, double y1,
                            double x2, double y2,
                            int order) {


    if (order < 0) {
        error("Order should be nonnegative.");
    }

    else if (order == 0) {
        drawTriangle(window, x0, y0, x1, y1, x2, y2);
    }

    else {

        drawSierpinskiTriangle(window, x0, y0,
                               computeMidpoint(x0,y0,x1,y1)[0], computeMidpoint(x0,y0,x1,y1)[1],
                               computeMidpoint(x0,y0,x2,y2)[0], computeMidpoint(x0,y0,x2,y2)[1],
                               order-1);

        drawSierpinskiTriangle(window, x1, y1,
                               computeMidpoint(x0,y0,x1,y1)[0], computeMidpoint(x0,y0,x1,y1)[1],
                               computeMidpoint(x1,y1,x2,y2)[0], computeMidpoint(x1,y1,x2,y2)[1],
                               order-1);

        drawSierpinskiTriangle(window, x2, y2,
                               computeMidpoint(x1,y1,x2,y2)[0], computeMidpoint(x1,y1,x2,y2)[1],
                               computeMidpoint(x0,y0,x2,y2)[0], computeMidpoint(x0,y0,x2,y2)[1],
                               order-1);

    }
}
