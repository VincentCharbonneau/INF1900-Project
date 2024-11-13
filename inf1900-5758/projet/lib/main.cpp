#include <iostream>

#include <iostream>
#include "RAMMatrixProxy.h"
#include "DrawableChar.h"
#include "DrawableLine.h"
#include "DrawableRectangle.h"
#include <io.h>
#include <fcntl.h>
#include "DrawableMatrix.h"
#include "DrawableTerminatedLine.h"

int main() {
	RAMMatrixProxy emptyMatrix(25, 101, ' ');

	DrawableRectangle outline(25, 101, DrawableLine::LineType::THIN);
	DrawableRectangle box(17, 26, DrawableLine::LineType::BOLD);
	DrawableLine boxEntry(DrawableLine::Direction::VERTICAL, 3, DrawableLine::LineType::EMPTY);

	emptyMatrix.draw(outline, 0, 0);
	emptyMatrix.draw(box, 4, 7);
	emptyMatrix.draw(boxEntry, 11, 32);

	DrawableRectangle route(13, 50, DrawableLine::LineType::DOUBLE);
	emptyMatrix.draw(route, 6, 42);

	DrawableLine routeEnd(DrawableLine::Direction::HORIZONTAL, 6, DrawableLine::LineType::DOUBLE);
	DrawableTerminatedLine routeEndTerminated(DrawableTerminatedLine::TERMINATION::END, routeEnd);

	emptyMatrix.draw(routeEndTerminated, 12, 36);

	RAMMatrixProxy robotMatrix(4, 3, ' ');
	robotMatrix.set(0, 0, u'/');
	robotMatrix.set(0, 1, u'-');
	robotMatrix.set(0, 2, u'\\');
	robotMatrix.set(1, 0, u'|');
	robotMatrix.set(1, 1, u' ');
	robotMatrix.set(1, 2, u'|');
	robotMatrix.set(2, 0, u'X');
	robotMatrix.set(2, 1, u' ');
	robotMatrix.set(2, 2, u'X');
	robotMatrix.set(3, 0, u'\\');
	robotMatrix.set(3, 1, u'-');
	robotMatrix.set(3, 2, u'/');

	DrawableMatrix robot(robotMatrix);

	emptyMatrix.draw(robot, 15, 18);

	std::cout << emptyMatrix;
	std::cout << "Produit par: <NOM DU ROBOT>";

	return 0;
}
