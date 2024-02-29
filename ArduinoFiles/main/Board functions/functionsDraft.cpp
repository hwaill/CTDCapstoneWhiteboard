#include "GCodeHandler.h"

/*measurement notes
letter scale 0.8 - height from descender to ascender: 30mm
cap height scale 0.8 - 25
pt size; ~183 stroke 12

letter scale 1.2 - height from descender to scender: 55mm
cap height scale 1.2 - 45mm
box spacing: 
pt size: ~100 stroke 12

assuming 0,0 is the top left corner
*/

Todos grid lines

void Todos(){ //parameters should inslude the number of to-do items so board can draw same number of lines
    drawline(33.282, 76.675, 33.282, 435.432); // vertical line
    float YcoordinateLines = 76.675//starting Y position of horizontal lines
    //horizontal lines distance: 200mm

    //writing cursor strt coordinate: 44.816, 101.614
    //sendWord("To-do List")

    //iterate in for loop for horizontal lines, spacing 37.072mm distance in for loop
    //draw check boxes starting at (208.191, 144.385), 24.5mm height/width



}

void HourlyPlanner(){
    drawline(348.203, 77.273, 384.302, 538.155); // vertical line
    float YcoordinateLines = 77.273;//starting Y position of horizontal lines
    //horizontal lines distance: 200mm

    //iterate in for loop for horizontal lines, spacing 37.072mm distance

}


