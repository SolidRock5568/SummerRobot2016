#include "math.h"

//X = ROW
//Y = COL
class FPS
{

public:

	int ROW;
	int COL;
	double SPEED;
FPS()
	{
	ROW = 1;
	COL = 1;
	SPEED = .5;
	}
FPS(int Row, int Col, double Speed)
		{
	ROW = Row;
	COL = Col;
	SPEED = Speed;
		}
double GetSpeed()
{
	return SPEED;

}
void SetSpeed(double Speed)
{
	SPEED = Speed;
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

	//Y = Bottom
	//X = Top

	int y1 = this->ROW; //1
	int x1 = this->COL;  //1

	int x2 = temp.GetCol(); //25
	int y2 = temp.GetRow(); // 50




	if ( x2 - x1 > 0)
	{
		if (y2 - y1 > 0)
		{
		double sideA = x2 - x1;
		double sideB = y2 - y1;



		//
		return (atan(sideB/sideA) * 180/3.1415);
				//return  asin(sideA/sideC) * 180;
		}

		else if (y2 - y1 < 0)
		{

			double sideA = x2 - x1;
			double sideB = y2 - y1;
			double sideC = (atan(sideB/sideA) * 180/3.1415);

			return 360 + sideC;
		}
	}

	else
	{
		if (y2 - y1 > 0)
				{

			double sideA = x2 - x1;
			double sideB = y2 - y1;
			double sideC = (atan(sideB/sideA) * 180/3.1415);

			return 90 + sideC + 90;




				}
		else
		{
			double sideA = x2 - x1;
			double sideB = y2 - y1;
			double sideC = (atan(sideB/sideA) * 180/3.1415);

			return 180 + sideC;

				}

	}

	//int x3 = x2;
	//int y3 = y1;



	return 0.0;
}
double GetDistance(FPS temp)
{
	int y1 = this->ROW; //1
	int x1 = this->COL;  //1

	int x2 = temp.GetCol(); //25
	int y2 = temp.GetRow(); // 50

	double sideA = x2 - x1;
	double sideB = y2 - y1;

	return sqrt((sideA * sideA) + (sideB*sideB));

}

int TestA()
{
	return 34;
}

double TestB()
{
	return 35.78;
}

};
