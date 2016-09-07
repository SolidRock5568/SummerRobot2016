#include "math.h"
#include "Barrier.cpp"


//X = ROW
//Y = COL
class FPS
{

public:

	int ROW;
	int COL;
	double SPEED;
	int XLimit;
	int YLimit;

	Barrier barrier;

FPS()
	{
	ROW = 1;
	COL = 1;
	SPEED = .5;
	barrier = Barrier(0,0,0,0,0);
	XLimit = 0;
	YLimit = 0;
	}
FPS(int Row, int Col, double Speed, int X, int Y)
		{
	ROW = Row;
	COL = Col;
	SPEED = Speed;
	XLimit = X;
	YLimit = Y;
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

	if (x1 == x2 && y1 > y2)
	{
		return 270;
	}
	else if (x1 == x2 && y1 < y2)
		{
			return 90;
		}
	else if (y1 == y2 && x1 < x2)
			{
				return 180;
			}
	else if (y1 == y2 && x1 < x2)
				{
					return 360;
				}
	else if (y1 == y2 && x1 == x2)
	{
		return 0;
	}




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

void CreateBarrier(int Y_S,int Y_E,int X_S,int X_E)
{
	barrier = Barrier(Y_S,Y_E,X_S,X_E,SPEED);
}

bool CheckSpot()
{
	if (barrier.GetXStart() > 0 && barrier.GetYStart() > 0)
	{
		return true;
	}
	return false;

}


int BarrierAvoid(FPS EndPostion)
{
	int x1 = ROW;
	int y1 = COL;
	int x2 = EndPostion.GetRow();
	int y2 = EndPostion.GetCol();

if ( x1 > x2 && y1 > y2)
{
	return 0;
}

return 0;

}


};
