#include "WPILib.h"
#include "AHRS.h"
#include "Math.h"
#include "FPS.cpp"
#include "CameraTech.cpp"


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
		int yServoPos;
		int xServoPos;

		FPS Field[21][21];
		CameraTech CT;






		Joystick mainJoystick;
		Joystick servoJoystick;





		Victor leftWheel;
		Victor rightWheel;
		Victor leftShoot;
		Victor rightShoot;
		Victor armLift;
		Victor kicker;

		Servo xServo;
		Servo yServo;



				IMAQdxSession session;
				IMAQdxSession USB;
				Image *frame;

				IMAQdxError imaqError;
				std::unique_ptr<AxisCamera> camera;


		Encoder leftEncoder;

		AHRS *driveGyro;

		//FPS field[320][649];
		// End Define Sensor Types

		void servoSet()
		{

			if (servoJoystick.GetY() > .1 || servoJoystick.GetY() < -.15)
						{
							if (servoJoystick.GetY() < 0)
							{

								yServoPos = yServoPos - ((-servoJoystick.GetY() * servoJoystick.GetY()) * 3);
							}

							else if (servoJoystick.GetY() > 0)
							{
								yServoPos = yServoPos - ((servoJoystick.GetY() * servoJoystick.GetY()) * 3);
							}
						}

			yServo.SetAngle(yServoPos);
			if (servoJoystick.GetX() > .1 || servoJoystick.GetX() < -.15)
									{
										if (servoJoystick.GetX() < 0)
										{

											xServoPos = xServoPos - ((-servoJoystick.GetX() * servoJoystick.GetX()) * 3);
										}

										else if (servoJoystick.GetX() > 0)
										{
											xServoPos = xServoPos - ((servoJoystick.GetX() * servoJoystick.GetX()) * 3);
										}
									}

						xServo.SetAngle(xServoPos);

						if (yServo.GetAngle() >= 90)
									{
										yServoPos = 90;
									}
						if ( xServo.GetAngle() >= 179)
							{
							xServoPos = 179;

									}




									if (servoJoystick.GetRawButton(2))
									{
										yServoPos = yServo.GetAngle();
										xServoPos = xServo.GetAngle();
									}

									else if (servoJoystick.GetRawButton(3))
									{
										xServoPos = 90;
										yServoPos = 45;
									}







		}

		void simpleScan()
		{
			int xPos = xServo.Get();
			int yPos = yServo.Get();
			bool xPostive = true;
			bool yPostive = true;

			while (servoJoystick.GetRawButton(1))
			{


				if (yPos >= 90)
				{
					yPostive = false;
				}
				else if (yPos <= 0)
				{
					yPostive = true;
				}
				if (xPos >= 180)
				{
					xPostive = false;
				}
				else if (xPos <= 0)
				{
					xPostive = true;
				}

				if (xPostive)
				{
					xPos++;
					xServo.Set(xPos);
				}
				else
				{
					xPos--;
					xServo.Set(xPos);
				}
				if (yPostive)
								{
									yPos++;
									yServo.Set(yPos);
								}
								else
								{
									yPos--;
									yServo.Set(yPos);
								}
				Wait(.05);

			}
		}

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
			if ( targetROW == robotCurrentY && targetCOL == robotCurrentX)
			{
				return;
			}
			rightSpeed = 0;
			leftSpeed = 0;

			leftEncoder.Reset();
			startDistance = leftEncoder.GetDistance();
			currentDistance = leftEncoder.GetDistance();
			targetDistance = Field[robotCurrentY][robotCurrentX].GetDistance(Field[targetROW][targetCOL]) * 190;

			currentHeading = driveGyro->GetAngle() + robotBaseHeading;
			targetHeading = Field[robotCurrentY][robotCurrentX].GetAngleDifference(Field[targetROW][targetCOL]);

			if(targetHeading < robotBaseHeading && targetHeading > 0)
						{
							targetHeading = targetHeading + 360;
						}





			while( currentDistance + 40 < targetDistance)
			{

				currentHeading = driveGyro->GetAngle() + robotBaseHeading;

				int z = currentHeading - 225;
							int m = 450 + z;
							bool max = 0;
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
								rightSpeed = .3;
								leftSpeed = 0;
							}
							else if (max == 0)
							{


								if (targetHeading <  currentHeading - 2)
												{
													leftSpeed = .3;
													rightSpeed = 0;
												}

												else if (targetHeading < currentHeading - 1)
												{
													leftSpeed = .15;
													rightSpeed = 0;
												}

												else
												{
													leftSpeed = 0;
												rightSpeed = 0;}
							}

							else
							{
								rightSpeed = 0;
								leftSpeed = 0;
							}


				rightWheel.Set( Field[robotCurrentY][robotCurrentX].GetSpeed() - .3 + rightSpeed);
				leftWheel.Set( - (Field[robotCurrentY][robotCurrentX].GetSpeed() - .3 + leftSpeed) );
				currentDistance = leftEncoder.GetDistance();
				SmartDashboard::PutNumber( "TargetHeading",Field[robotCurrentY][robotCurrentX].GetAngleDifference(Field[targetROW][targetCOL]));
				SmartDashboard::PutNumber("EncoderDistance" , leftEncoder.GetDistance());
				SmartDashboard::PutNumber("EncoderTarget" , targetDistance);

			}
			rightWheel.Set(0);
			leftWheel.Set(0);
			Wait(.5);
			currentDistance = leftEncoder.GetDistance();

			while( currentDistance > targetDistance + 30)
						{

				currentHeading = driveGyro->GetAngle() + robotBaseHeading;


											int z = currentHeading - 225;
											int m = 450 + z;
											bool max = 0;
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
																		rightSpeed = 0;
																		leftSpeed = .3;
																	}
																	else if (max == 0)
																	{


																		if (targetHeading <  currentHeading - 2)
																						{
																							leftSpeed = 0;
																							rightSpeed = .3;
																						}

																						else if (targetHeading < currentHeading - 1)
																						{
																							leftSpeed = 0;
																							rightSpeed = .15;
																						}

																						else
																						{
																							leftSpeed = 0;
																						rightSpeed = 0;}
																	}

																	else
																	{
																		rightSpeed = 0;
																		leftSpeed = 0;
																	}

							rightWheel.Set( -  (Field[robotCurrentY][robotCurrentX].GetSpeed() - .35 + rightSpeed));
							leftWheel.Set(  (Field[robotCurrentY][robotCurrentX].GetSpeed()) - .35 + leftSpeed);
							currentDistance = leftEncoder.GetDistance();
							SmartDashboard::PutNumber( "TargetHeading",Field[robotCurrentY][robotCurrentX].GetAngleDifference(Field[targetROW][targetCOL]));
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
			leftSpeed = Field[robotCurrentY][robotCurrentX].GetSpeed() - .15;
			rightSpeed = Field[robotCurrentY][robotCurrentX].GetSpeed() - .15;

			if(targetHeading < robotBaseHeading && targetHeading > 0)
			{
				targetHeading = targetHeading + 360;
			}



			if ( currentHeading - 2 < targetHeading && currentHeading + 2 > targetHeading)
			{
				return;
			}
				else if (targetHeading == 0 || targetHeading == robotBaseHeading + 360)
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
						while((3 + currentHeading > targetHeading && targetHeading > currentHeading - 3) == false)
						{
								leftWheel.Set(leftSpeed);
								rightWheel.Set(rightSpeed);
								currentHeading = driveGyro->GetAngle() + robotBaseHeading;
						    	SmartDashboard::PutNumber("TargetHeading" , targetHeading);
								SmartDashboard::PutNumber("currentHeading", currentHeading);
						}

			}
					else if (max ==  0)
							{
				//LEFT

									while( (3 + currentHeading > targetHeading && targetHeading > currentHeading - 3) == false)
													{
												leftWheel.Set(-leftSpeed);
												rightWheel.Set(-rightSpeed);
												currentHeading = driveGyro->GetAngle() + robotBaseHeading;
												SmartDashboard::PutNumber("TargetHeading" , targetHeading);
												SmartDashboard::PutNumber("currentHeading", currentHeading);
													}

							}
			leftWheel.Set(0);
			rightWheel.Set(0);

			if(targetHeading > currentHeading - 2 || targetHeading < currentHeading + 2)
			{
				 z = currentHeading - 225;
				 m = 450 + z;
				 max = 1;
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




																					while((1 + currentHeading > targetHeading && targetHeading > currentHeading - 1) == false)
																					{
																						leftWheel.Set(leftSpeed);
																						rightWheel.Set(rightSpeed);
																						//currentHeading = driveGyro->GetAngle();
																						currentHeading = driveGyro->GetAngle() + robotBaseHeading;
																						SmartDashboard::PutNumber("TargetHeading" , targetHeading);
																						SmartDashboard::PutNumber("currentHeading", currentHeading);
																					}

																				}
										else if (max ==  0)
														{
											//LEFT

															while( (1 + currentHeading > targetHeading && targetHeading > currentHeading - 1) == false)
																				{

																					leftWheel.Set(-leftSpeed);
																					rightWheel.Set(-rightSpeed);
																					currentHeading = driveGyro->GetAngle() + robotBaseHeading;

																					SmartDashboard::PutNumber("TargetHeading" , targetHeading);
																					SmartDashboard::PutNumber("currentHeading", currentHeading);
																				}
														}
							leftWheel.Set(0);
							rightWheel.Set(0);

			}
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
				 servoJoystick(1),


				 leftWheel(0),
				 rightWheel(1),
				 leftShoot(2),
				 rightShoot(3),
				 armLift(4),
				 kicker(5),

				 xServo(6),
				 yServo(7),



			 leftEncoder(0,1, false, Encoder::k4X)


	{








		driveGyro = new AHRS(SPI::Port::kMXP);

		//CameraServer::GetInstance()->SetQuality(50);
		//CameraServer::GetInstance()->StartAutomaticCapture("cam0");

				frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
				camera.reset(new AxisCamera("axis-camera"));




				imaqError = IMAQdxOpenCamera("cam0", IMAQdxCameraControlModeController, &USB);
						if(imaqError != IMAQdxErrorSuccess) {
							DriverStation::ReportError("IMAQdxOpenCamera error: " + std::to_string((long)imaqError) + "\n");
						}
						imaqError = IMAQdxConfigureGrab(USB);
						if(imaqError != IMAQdxErrorSuccess) {
							DriverStation::ReportError("IMAQdxConfigureGrab error: " + std::to_string((long)imaqError) + "\n");
						}





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
				 robotCurrentY = 1;

				 TestY = 1;
				 TestX = 1;
				 robotBaseHeading = 90;

				 yServoPos = 45;
				 xServoPos = 90;




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
						for (int tempY = 0; tempY <= 20 ; tempY++)
						 {
							 for (int tempX = 0; tempX <= 20; tempX++)
							 {
								 Field[tempY][tempX] = FPS(tempY , tempX , .55, 20 , 20);
						}
						 }

						/*xServo.Set(0);
						yServo.Set(0);
						Wait(5);
						xServo.Set(180);
						yServo.Set(180);
						Wait(5);
						xServo.Set(90);
						yServo.Set(90);*/


		}

	void TeleopPeriodic()
		{
		IMAQdxStartAcquisition(USB);



		if ( !servoJoystick.GetRawButton(4))
		{
			IMAQdxGrab(USB, frame, true, NULL);
						if(imaqError != IMAQdxErrorSuccess) {
							DriverStation::ReportError("IMAQdxGrab error: " + std::to_string((long)imaqError) + "\n");
						} else {

							CameraServer::GetInstance()->SetImage(frame);
						}
		}
		else
		{
			camera->GetImage(frame);
			CameraServer::GetInstance()->SetImage(frame);
		}
		if(servoJoystick.GetRawButton(2))
				{
					CT.ColorPickUp(USB, frame, 20, {350,10},{100,255},{100,255});//Red
					//CT.ColorPickUp(USB, frame, 20, {110,130},{50,255},{50,255});//Green
				}
		else if(servoJoystick.GetRawButton(3))
		{
			CT.ColorPickUp(USB, frame, 20, {220,260},{0,255},{0,255});//Blue

		}
		servoSet();
		if (servoJoystick.GetRawButton(1))
		{
			simpleScan();
		}

		SmartDashboard::PutNumber("Encoder Pulses", leftEncoder.GetDistance());




		if (mainJoystick.GetRawButton(1))
			{



			SmartDashboard::PutNumber("Angle Returned" ,Field[robotCurrentY][robotCurrentX].GetDistance(Field[TestY][TestX]) );
			//advancedTurn(TestY , TestX);
			Wait(2);
			advancedMove(TestY , TestX);





				//basicTurn(Field[robotCurrentX][robotCurrentY].GetAngleDifference(Field[50][25]), .4);
			}
		SmartDashboard::PutNumber("CurrentDistance" , leftEncoder.GetDistance());
		SmartDashboard::PutNumber("CurrentAngle" , driveGyro->GetAngle() + robotBaseHeading);
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

						if(mainJoystick.GetRawButton(6) == true)
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
						SmartDashboard::PutNumber("testYAccell" , driveGyro->GetWorldLinearAccelY());

						SmartDashboard::PutNumber("testXAccell" , driveGyro->GetWorldLinearAccelX());


		}



	void TestPeriodic()
		{



		}


};
START_ROBOT_CLASS(Robot)




