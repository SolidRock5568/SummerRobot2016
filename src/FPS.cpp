//#import"FPS.h"
#import "Math.h"
class FPS
{

public:

	int ROW;
	int COL;





FPS()
	{
	ROW = 0;
	COL = 0;
	}
FPS(int Row, int Col)
		{
	ROW = Row;
	COL = Col;
		}
int GetCol()
{
	return COL;
}
int GetRow()
{
	return ROW;
}

double GetAngleDifference(FPS temp)
{
	int y1 = ROW;
	int x1 = COL;

	int x2 = COL;
	int y2 = temp.GetRow();



	double sideA = y1 - y2;
	double sideB = x2 - x1;

	return tan(sideB/sideA);
}
double GetDistance(FPS temp)
{
		int y1 = ROW;
		int x1 = COL;

		int x2 = COL;
		int y2 = temp.GetRow();



		double sideA = y1 - y2;
		double sideB = x2 - x1;

	return sqrt((sideA * sideA) + (sideB*sideB));

}

};
