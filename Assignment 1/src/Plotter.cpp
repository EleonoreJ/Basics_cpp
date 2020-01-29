#include "Plotter.h"
#include "strlib.h"
#include <unordered_map>
using namespace std;


/* Unordered map from strings representing the different action commands,
 * to integers representing different cases.
 */
unordered_map<string,int> stringToCase{
    {"pendown", 1},
    {"penup", 2},
    {"moveabs", 3},
    {"moverel", 4},
    {"pencolor", 5},
    {"penwidth", 6},
};



/* This function takes as input a reference to a stream, reads the associated
 * plotter script and draws the image it contains.
 */
void runPlotterScript(istream& input) {
    PenStyle style = {1, "black"};
    string status = "UP";
    double x = 0;
    double y = 0;

    for (string line; getline(input, line); ) {
        Vector<string> command = stringSplit(line, " ");
        string action = toLowerCase(command[0]);

        switch(stringToCase.at(action)) {
        case 1:
            status = "DOWN";
            break;

        case 2:
            status = "UP";
            break;

        case 3:
            if (status == "DOWN") {
                drawLine(x, y, stringToReal(command[1]), stringToReal(command[2]), style);
            }
            x = stringToReal(command[1]);
            y = stringToReal(command[2]);
            break;

        case 4:
            if (status == "DOWN") {
                drawLine(x, y, x + stringToReal(command[1]), y + stringToReal(command[2]), style);
            }
            x += stringToReal(command[1]);
            y += stringToReal(command[2]);
            break;

        case 5:
            style.color = command[1];
            break;

        case 6:
            style.width = stringToReal(command[1]);
            break;

        default:
            break;
        }
    }
}
