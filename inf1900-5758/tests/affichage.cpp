#include "EEPROMMatrix.h"
#include "memoire_24.h"
#include "USART.h"
#include "DrawableRectangle.h"
#include "DrawableLine.h"
#include "DrawableTerminatedLine.h"
#include "DrawableMatrix.h"
#include "Line.h"

int main() {
    Memoire24CXXX eeprom;

    EEPROMMatrix matrix(25, 101, ' ', 0, eeprom);

    DrawableRectangle outline(25, 101, DrawableLine::LineType::THIN);
	DrawableRectangle box(17, 26, DrawableLine::LineType::BOLD);
	DrawableLine boxEntry(DrawableLine::Direction::VERTICAL, 3, DrawableLine::LineType::EMPTY);

	matrix.draw(outline, 0, 0);
	matrix.draw(box, 4, 7);
	matrix.draw(boxEntry, 11, 32);

	DrawableRectangle route(13, 50, DrawableLine::LineType::DOUBLE);
	matrix.draw(route, 6, 42);

	DrawableLine routeEnd(DrawableLine::Direction::HORIZONTAL, 6, DrawableLine::LineType::DOUBLE);
	DrawableTerminatedLine routeEndTerminated(DrawableTerminatedLine::TERMINATION::END, routeEnd);

	matrix.draw(routeEndTerminated, 12, 36);

    EEPROMMatrix robotMatrix(4, 3, ' ', 10000, eeprom);
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
	matrix.draw(robot, 15, 18);

	Line line1(5, 2, true);
	Line line2(9, 6, true);
	Line line3(12, 4, true);
	Line line4(12, 8, false);
	Line line5(7, 1, false);
	Line lines[6] = {line1, line2, line3, line4, line5};

	uint8_t distFromPrevAcc {};
	uint8_t lineIndex {};
	
	for(; lineIndex < 6 && lines[lineIndex].getFirstSide(); lineIndex++) {
		Line& line = lines[lineIndex];
		distFromPrevAcc += line.getDistFromPrev() * 1;
		DrawableLine dl(line);
		DrawableTerminatedLine dtl(DrawableTerminatedLine::TERMINATION::START, dl);
		matrix.draw(dtl, 6, 42 + distFromPrevAcc);
	}

	distFromPrevAcc = 0;

	for(; lineIndex < 6 && lines[lineIndex].getRawLineType() != LineLength::UNDEFINED; lineIndex++) {
		Line& line = lines[lineIndex];
		distFromPrevAcc += line.getDistFromPrev() * 1;
		DrawableLine dl(line);
		DrawableTerminatedLine dtl(DrawableTerminatedLine::TERMINATION::END, dl);
		matrix.draw(dtl, 18 - dl.getSize(), 92 - distFromPrevAcc);
	}

    matrix.display();

    return 0;
}