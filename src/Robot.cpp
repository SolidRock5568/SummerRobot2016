#include "WPILib.h"
#include "AHRS.h"
#include "Math.h"
#include "FPS.cpp"


#define A_BUTTON(TEST) 1
#define B_BUTTON 2
#define X_BUTTON 3
#define Y_BUTTON 4
#define L_BUMPER 5
#define R_BUMPER 6
#define SELECT_BUTTON 7
#define START_BUTTON 8

class Robot: public IterativeRobot
{

public:

	int TestY;
	int TestX;
	// Start Define Auto Var

		int robotBaseHeading;
		int robotCurrentX;
		int robotCurrentY;
		int time;
		double rightSpeed;
		double leftSpeed;
		double targetDistance;
		double currentDistance;
		double startDistance;
		int currentHeading;
		int targetHeading;
		double distancePerSecond;
		double howFarOffCourse;
		double timeOffCourse;
		bool slowStop;
		bool slowStart;
		bool reachedTarget;
		bool areOffCourse;
		bool areObjects;

		FPS Field[11][11];






		Joystick mainJoystick;





		Victor leftWheel;
		Victor rightWheel;
		Victor leftShoot;
		Victor rightShoot;
		Victor armLift;
		Victor kicker;




				/*IMAQdxSession session;
				Image *frame;
				IMAQdxError imaqError;
				std::unique_ptr<AxisCamera> camera;*/


		Encoder leftEncoder;

		AHRS *driveGyro;

		//FPS field[320][649];
		// End Define Sensor Types

		float squareDrive(float val) {
				if(val > 0.05) {
					val = val*val;
					return val;
				}
				else if(val < -0.05) {
					val = -(val*val);
					return val;
				}
				else {
					return 0;
				}
			}
		void advancedMove(int targetROW , int targetCOL)
		{
			startDistance = leftEncoder.GetDistance();
			currentDistance = leftEncoder.GetDistance();
			targetDistance = Field[robotCurrentY][robotCurrentX].GetDistance(Field[targetROW][targetCOL]) * 190;

			while( currentDistance + 20 < targetDistance)
			{
				rightWheel.Set( Field[robotCurrentY][robotCurrentX].GetSpeed());
				leftWheel.Set( - (Field[robotCurrentY][robotCurrentX].GetSpeed()));
				currentDistance = leftEncoder.GetDistance();

				SmartDashboard::PutNumber("EncoderDistance" , leftEncoder.GetDistance());
				SmartDashboard::PutNumber("EncoderTarget" , targetDistance);
			}


			rightWheel.Set(0);
			leftWheel.Set(0);

			robotCurrentX = targetCOL;
			robotCurrentY = targetROW;




		}

		void advancedTurn (int targetROW , int targetCOL)
		{
			currentHeading = driveGyro->GetAngle() + robotBaseHeading;
			targetHeading = Field[robotCurrentY][robotCurrentX].GetAngleDifference(Field[targetROW][targetCOL]);
			SmartDashboard::PutNumber("TargetHeading" , targetHeading);
			SmartDashboard::PutNumber("currentHeading", currentHeading);
			leftSpeed = Field[robotCurrentY][robotCurrentX].GetSpeed();
			rightSpeed = Field[robotCurrentY][robotCurrentX].GetSpeed();

			if ( currentHeading - 3 < targetHeading && currentHeading + 3 > targetHeading)
			{
				return;
			}
			else if (targetHeading == 0)
			{
				return;
			}

			int z = currentHeading - 225;
			int m = 450 + z;
			bool max = 1;
			if (targetHeading<currentHeading and targetHeading>z)
			{
				max=0;
			}
			else if (targetHeading>currentHeading and targetHeading>m)
			{
				max=0;
			}
			else
			{
				max=1;
			}
			if (max == 1)
													{
				if(targetHeading <= robotBaseHeading)
												{
													targetHeading = targetHeading + robotBaseHeading;
												}
				else if ( (targetHeading - 360 < currentHeading + 5 && targetHeading - 360 > currentHeading - 5) || (targetHeading + 360 < currentHeading + 5 && targetHeading + 360 > currentHeading - 5))
												{
													return;
												}

				else
				{
														while((2 + currentHeading > targetHeading && targetHeading > currentHeading - 2) == false)
														{
															leftWheel.Set(leftSpeed);
															rightWheel.Set(rightSpeed);
															currentHeading = driveGyro->GetAngle();
															currentHeading = currentHeading + robotBaseHeading;
															SmartDashboard::PutNumber("TargetHeading" , targetHeading);
															SmartDashboard::PutNumber("currentHeading", currentHeading);
														}
										}
													}
			else if (max ==  0)
							{
				//LEFT
								targetHeading = targetHeading + 360;

								if (targetHeading >= 360 + robotBaseHeading)
								{
									targetHeading = Field[robotCurrentY][robotCurrentX].GetAngleDifference(Field[targetROW][targetCOL]);



								}


								 if(targetHeading <= robotBaseHeading)
								{
									targetHeading = targetHeading + robotBaseHeading;
								}

								 else if ( (targetHeading - 360 < currentHeading + 5 && targetHeading - 360 > currentHeading - 5) || (targetHeading + 360 < currentHeading + 5 && targetHeading + 360 > currentHeading - 5))
																{
																	return;
																}

								while( (2 + currentHeading > targetHeading && targetHeading > currentHeading - 2) == false)
													{

														leftWheel.Set(-leftSpeed);
														rightWheel.Set(-rightSpeed);
														currentHeading = driveGyro->GetAngle();
														currentHeading = currentHeading + robotBaseHeading;

														SmartDashboard::PutNumber("TargetHeading" , targetHeading);
														SmartDashboard::PutNumber("currentHeading", currentHeading);
													}

							}
			leftWheel.Set(0);
			rightWheel.Set(0);
		}
		void basicTurn(int targetAngle, double speed)
			{
			SmartDashboard::PutNumber("TargetHeading" , targetHeading);
			SmartDashboard::PutNumber("currentHeading", currentHeading);
			leftSpeed = speed;
			rightSpeed = speed;
			currentHeading = driveGyro->GetAngle();
				targetHeading = currentHeading + targetAngle;

				if (targetHeading < currentHeading)
				{
					while(targetHeading < currentHeading)
					{

						leftWheel.Set(-leftSpeed);
						rightWheel.Set(-rightSpeed);
						currentHeading = driveGyro->GetAngle();

						SmartDashboard::PutNumber("TargetHeading" , targetHeading);
						SmartDashboard::PutNumber("currentHeading", currentHeading);
					}
				}
				else if (targetHeading > currentHeading)
							{
								while(targetHeading > currentHeading)
								{
									leftWheel.Set(leftSpeed);
									rightWheel.Set(rightSpeed);
									currentHeading = driveGyro->GetAngle();

									SmartDashboard::PutNumber("TargetHeading" , targetHeading);
									SmartDashboard::PutNumber("currentHeading", currentHeading);
								}
				}
				leftWheel.Set(0);
				rightWheel.Set(0);

			}



	Robot() :


		 	 	 mainJoystick(0),


				 leftWheel(0),
				 rightWheel(1),
				 leftShoot(2),
				 rightShoot(3),
				 armLift(4),
				 kicker(5),



			 leftEncoder(0,1, false, Encoder::k4X)


	{








		driveGyro = new AHRS(SPI::Port::kMXP);

				//frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
				//camera.reset(new AxisCamera("axis-camera.local"));





				leftEncoder.SetSamplesToAverage(5);
				leftEncoder.SetDistancePerPulse(1.0 / 360.0 * 2.0 * 3.1415 * 3.0);
				leftEncoder.SetMinRate(1.0);





				 time = 0;
				 rightSpeed = 0.0;
				 leftSpeed = 0.0;
				 targetDistance = 0.0;
				 currentDistance = 0.0;
				 startDistance = 0.0;
				 currentHeading = 90;
				 targetHeading = 0;
				 distancePerSecond = 0.0;
				 howFarOffCourse = 0.0;
				 timeOffCourse = 0.0;
				 slowStop = false;
				 slowStart = false;
				 reachedTarget = false;
				 areOffCourse = false;
				 areObjects = false;

				 robotCurrentX = 5;
				 robotCurrentY = 5;

				 TestY = 1;
				 TestX = 1;
				 robotBaseHeading = 90;




//Field [25][25] = FPS(25 , 25 , .3);


	}

	void RobotInit()
		{


		}

	void AutonomousInit()
		{

		}

	void AutonomousPeriodic()
		{

		}

	void TeleopInit()
		{
						for (int tempY = 0; tempY <= 10 ; tempY++)
						 {
							 for (int tempX = 0; tempX <= 10; tempX++)
							 {
								 Field[tempY][tempX] = FPS(tempY , tempX , .55);
						}
						 }


		}

	void TeleopPeriodic()
		{

		SmartDashboard::PutNumber("Encoder Pulses", leftEncoder.GetDistance());




		if (mainJoystick.GetRawButton(1))
			{



			SmartDashboard::PutNumber("Angle Returned" ,Field[robotCurrentY][robotCurrentX].GetDistance(Field[TestY][TestX]) );
			advancedTurn(TestY , TestX);
			Wait(1);
			//advancedMove(TestY , TestX);





				//basicTurn(Field[robotCurrentX][robotCurrentY].GetAngleDifference(Field[50][25]), .4);
			}

		if(mainJoystick.GetRawButton(3))
		{
			TestY++;
			Wait(.5);
		}
		if(mainJoystick.GetRawButton(4))
		{
			TestX++;
			Wait(.5);
		}
		if (mainJoystick.GetRawButton(2))
				{
					TestY--;
					TestX--;
					Wait(.5);
				}
		SmartDashboard::PutNumber("TestY" , TestY);
		SmartDashboard::PutNumber("TestX" , TestX);


		if(mainJoystick.GetRawAxis(2) > 0 )
									{
										leftShoot.Set(0.4);
										rightShoot.Set(-0.4);
									}
									else if(mainJoystick.GetRawAxis(3) > 0 )
									{
										leftShoot.Set(-1);
										rightShoot.Set(1);

									}
									else
									{
										leftShoot.Set(0);
										rightShoot.Set(0);
									}

						if (mainJoystick.GetRawAxis(1) < -0.2)
									{
										float speed = mainJoystick.GetRawAxis(1) * mainJoystick.GetRawAxis(1);
										//Raise(speed);
										armLift.Set(-speed);
									}
									else if (mainJoystick.GetRawAxis(1) > 0.2)
									{
										float speed = mainJoystick.GetRawAxis(1) * mainJoystick.GetRawAxis(1);
										//Lower(speed);
										armLift.Set(speed);
									}
									else
									{
										armLift.Set(0);
										//Stop();
									}

						if(mainJoystick.GetRawButton(B_BUTTON) == true)
									{
										kicker.Set(1);
										Wait(0.1);
										kicker.Set(-0.5);
										Wait(0.17);
										kicker.Set(0);
									}


						float yIn = mainJoystick.GetRawAxis(5);
						float xIn = mainJoystick.GetRawAxis(4);

						if(mainJoystick.GetRawButton(R_BUMPER)){

							rightWheel.Set((squareDrive(yIn) + squareDrive(xIn)));
							leftWheel.Set((squareDrive(xIn) - squareDrive(yIn)));

						}
						else{

							rightWheel.Set(-(squareDrive(yIn) - squareDrive(xIn)) / 2);
							leftWheel.Set((squareDrive(yIn) + squareDrive(xIn)) / 2);

						}
						// wait for a motor update time
						Wait(0.005);


		}



	void TestPeriodic()
		{

		}


};
START_ROBOT_CLASS(Robot)




