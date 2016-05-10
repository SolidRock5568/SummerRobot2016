#include "WPILib.h"
#include "AHRS.h"
#include "FPS.cpp"

#define A_BUTTON 1
#define B_BUTTON 2
#define X_BUTTON 3
#define Y_BUTTON 4
#define L_BUMPER 5
#define R_BUMPER 6
#define SELECT_BUTTON 7
#define START_BUTTON 8

class Robot: public IterativeRobot
{



private:
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default Auto Mode";
	const std::string autoNameCustom = "Test Auto Selector";
	std::string autoSelected;

	// Start Define Auto Var

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

	// End Define Auto Var

	// Start Define User Input
	Joystick mainJoystick;
	// End Define User Input

	//	Start Define User Input Ports

	// End Define User Inputs Ports
	const int MainJoystickPort = 0;
	// Start Define Motor Control
	Victor leftWheel;
	Victor rightWheel;
	Victor leftShoot;
	Victor rightShoot;
	Victor armLift;
	Victor kicker;
	// End Define Motor Control

	// Start Define Motor Ports
	const int LeftDriveMotorPort = 0;
	const int RightDriveMotorPort = 1;
	const int LeftShootMotorPort = 2;
	const int RightShootMotorPort = 3;
	const int ArmLiftMotorPort = 4;
	const int KickerMotorPort = 5;
	// End Define Motor Ports

	// Start Define Sensor Types
			// Start Camera
			IMAQdxSession session;
			Image *frame;
			IMAQdxError imaqError;
			std::unique_ptr<AxisCamera> camera;
			// End Camera
	Encoder rightEncoder;
	Encoder leftEncoder;
	DigitalInput UpSwitch;
	DigitalInput DownSwitch;
	AHRS *driveGyro;
	AnalogInput UltraSonicFrontRight;
	FPS field[320][649];
	// End Define Sensor Types

	// Start Define Sensor Ports
	const int RightEncoderPort1 = 0;
	const int RightEncoderPort2 = 1;
	const int LeftEncoderPort1 = 2;
	const int LeftEncoderPort2 = 3;
	const std::string GyroPort = "SPI::Port::kMXP" ;
	const int DownArmLimitSwitchPort = 5;
	const int UpArmLimitSwitchPort = 6;
	const int UltraSonicFrontRightPort = 7;
	// End Define Sensor Ports
public:

	//Methods needed for SmartGyro/Encoder distance Forward
		//Off course Check
		//Distance Check
		// Slow Speed-up/Slow-Down
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

	void basicTurn(int targetAngle)
	{
		targetHeading = targetAngle;
		currentHeading = driveGyro->GetAngle();
		if (targetHeading > currentHeading)
		{
			while(targetHeading > currentHeading)
			{
				leftWheel.Set(-leftSpeed);
				rightWheel.Set(-rightSpeed);
			}
		}
		else if (targetHeading < currentHeading)
					{
						while(targetHeading < currentHeading)
						{
							leftWheel.Set(leftSpeed);
							rightWheel.Set(rightSpeed);
						}
		}

	}

	void basicEncoder(int distance)
	{
		startDistance = leftEncoder.GetDistance();
		targetDistance = startDistance + distance;

		for (currentDistance = startDistance; currentDistance >= targetDistance; currentDistance = leftEncoder.GetDistance())
		{
			rightWheel.Set(rightSpeed);
			leftWheel.Set(-leftSpeed);
		}
		rightWheel.Set(0);
		leftWheel.Set(0);
	}
	bool offCourseCheck()
	{
		currentHeading = driveGyro->GetAngle();

		return !(targetHeading - 1 < currentHeading && currentHeading > targetHeading + 1);
	}
	void trackOffCourse()
	{
		if(offCourseCheck())
		{
			time++;
			howFarOffCourse = howFarOffCourse + distancePerSecond;
		}
		else
		{
			areOffCourse = false;
			targetDistance = targetDistance + howFarOffCourse;

		}

	}

	void correctOffCourse()
	{
		if (currentHeading > targetHeading + 10 || currentHeading < targetHeading - 10)
		{
			if (currentHeading > targetHeading)
			{
				leftSpeed = leftSpeed + .3;
				rightSpeed = rightSpeed - .1;
			}
			else
			{
				leftSpeed = leftSpeed - .1;
				rightSpeed = rightSpeed + .3;
			}

		}
		else
		{
			if (currentHeading > targetHeading)
			{
				leftSpeed = leftSpeed + .1;

			}
			else
			{

				rightSpeed = rightSpeed + .1;
			}
		}
	}

	Robot() :


		 mainJoystick(MainJoystickPort),


		 leftWheel(LeftDriveMotorPort),
		 rightWheel(RightDriveMotorPort),
		 leftShoot(LeftShootMotorPort),
		 rightShoot(RightShootMotorPort),
		 armLift(ArmLiftMotorPort),
		 kicker(KickerMotorPort),


		 rightEncoder(RightEncoderPort1, RightEncoderPort2, false, Encoder::k4X),
		 leftEncoder(LeftEncoderPort1, LeftEncoderPort2, false, Encoder::k4X),
		 UpSwitch(UpArmLimitSwitchPort),
		 DownSwitch(DownArmLimitSwitchPort),
		 UltraSonicFrontRight(UltraSonicFrontRightPort)




{


		driveGyro = new AHRS(SPI::Port::kMXP);

		frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
		camera.reset(new AxisCamera("axis-camera.local"));

		rightEncoder.SetSamplesToAverage(5);
		rightEncoder.SetDistancePerPulse(1.0 / 360.0 * 2.0 * 3.1415 * 1.5);
		rightEncoder.SetMinRate(1.0);



		leftEncoder.SetSamplesToAverage(5);
		leftEncoder.SetDistancePerPulse(1.0 / 360.0 * 2.0 * 3.1415 * 1.5);
		leftEncoder.SetMinRate(1.0);


		for(int r = 0; r < 321; r++)
			{
				for(int c = 0; c< 650; c++)
				{
				field[r][c] = FPS(r,c);
				}
			}


		 time = 0;
		 rightSpeed = 0.0;
		 leftSpeed = 0.0;
		 targetDistance = 0.0;
		 currentDistance = 0.0;
		 startDistance = 0.0;
		 currentHeading = 0;
		 targetHeading = 0;
		 distancePerSecond = 0.0;
		 howFarOffCourse = 0.0;
		 timeOffCourse = 0.0;
		 slowStop = false;
		 slowStart = false;
		 reachedTarget = false;
		 areOffCourse = false;
		 areObjects = false;


				// Defines the number of samples to average when determining the rate.
				// On a quadrature encoder, values range from 1-255; larger values
				//   result in smoother but potentially less accurate rates than lower values.


				// Defines how far the mechanism attached to the encoder moves per pulse.
				// In this case, we assume that a 360 count encoder is directly attached
				//   to a 3 inch diameter (1.5inch radius) wheel, and that we want to
				//   measure distance in inches.


				// Defines the lowest rate at which the encoder will not be considered
				//   stopped, for the purposes of the GetStopped() method.
				// Units are in distance / second, where distance refers to the units
				//   of distance that you are using, in this case inches.



}
	void RobotInit()
	{





		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);

	}



	void AutonomousInit()
	{
		autoSelected = *((std::string*)chooser->GetSelected());
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void AutonomousPeriodic()
	{
		if(autoSelected == autoNameCustom){
			camera->GetImage(frame);
			imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			CameraServer::GetInstance()->SetImage(frame);
			Wait(0.05);
		} else {
			camera->GetImage(frame);
						//imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			CameraServer::GetInstance()->SetImage(frame);
			Wait(0.05);
		}
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		// acquire images
				IMAQdxStartAcquisition(session);

				// grab an image, draw the circle, and provide it for the camera server which will
				// in turn send it to the dashboard.
				while(IsOperatorControl() && IsEnabled()) {
					IMAQdxGrab(session, frame, true, NULL);
					if(imaqError != IMAQdxErrorSuccess) {
							DriverStation::ReportError("IMAQdxGrab error: " + std::to_string((long)imaqError) + "\n");
					} else {
							imaqDrawShapeOnImage(frame, frame, { 190, 250, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_RECT, 0.0f);
							//				 Shape parameters: {x-axis, y axis, length, width}
							CameraServer::GetInstance()->SetImage(frame);
					}

				//Scheduler::GetInstance().Run();
				if(mainJoystick.GetRawAxis(3) > 0 )
							{
								leftShoot.Set(1);
								rightShoot.Set(-1);
							}
							else if(mainJoystick.GetRawAxis(2) > 0 )
							{
								leftShoot.Set(-0.4);
								rightShoot.Set(0.4);

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
							}
							else if (mainJoystick.GetRawAxis(1) > 0.2)
							{
								float speed = mainJoystick.GetRawAxis(1) * mainJoystick.GetRawAxis(1);
								//Lower(speed);
							}
							else
							{
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

					rightWheel.Set((squareDrive(yIn) + squareDrive(xIn)) / 2);
					leftWheel.Set((squareDrive(xIn) - squareDrive(yIn)) / 2);

				}
				// wait for a motor update time
				Wait(0.005);
				}

				// stop image acquisition
				IMAQdxStopAcquisition(session);




	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)


