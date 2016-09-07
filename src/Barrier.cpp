/*
 * Barrier.cpp
 *
 *  Created on: Aug 7, 2016
 *      Author: Alex's
 */




class Barrier
{
public :
	int Y_start_location;
	int X_start_location;
	int Y_end_location;
	int X_end_location;
	int traverse_Speed;



	bool Avoid;
	bool Moving;
	bool Collide;

	Barrier()
	{
		Y_start_location = 0;
		Y_end_location = 0;
		X_start_location = 0;
		X_end_location = 0;
		traverse_Speed = 0;



		Avoid = false;
		Moving = false;
		Collide = false;

	}

	Barrier(int Y_S , int Y_E , int X_S, int X_E , int Speed)
		{
			Y_start_location = Y_S;
			Y_end_location = Y_E;
			X_start_location = X_S;
			X_end_location = X_E;
			traverse_Speed = Speed;


			Avoid = false;
			Moving = false;
			Collide = false;

		}

	int GetYStart()
	{
		return Y_start_location;
	}

	int GetXStart()
	{
		return X_start_location;
	}

	int GetYEnd()
	{
		return Y_end_location;
	}

	int GetXEnd()
	{
		return X_end_location;
	}

	int GetTraverseSpeed()
	{
		return traverse_Speed;
	}

	void SetYStart(int Y_S)
		{
			Y_start_location = Y_S;
		}

	void SetXStart( int X_S)
		{
			X_start_location = X_S;
		}

	void SetYEnd(int Y_E)
		{
			Y_end_location = Y_E;
		}

	void SetXEnd(int X_E)
		{
			X_end_location = X_E;
		}

	void SetTraverseSpeed( int SPEED)
		{
		traverse_Speed = SPEED;
		}




};
