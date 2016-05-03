#include "WPILib.h"
#include "AHRS.h"

class Robot: public IterativeRobot
{



private:
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

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
	Encoder rightEncoder;
	Encoder leftEncoder;
	AHRS *DriveGyro;
	// End Define Sensor Types

	// Start Define Sensor Ports
	const int RightEncoderPort1 = 0;
	const int RightEncoderPort2 = 1;
	const int LeftEncoderPort1 = 2;
	const int LeftEncoderPort2 = 3;
	const std::string GyroPort = "SPI::Port::kMXP" ;
	const int DownArmLimitSwitchPort = 5;
	const int UpArmLimitSwitchPort = 6;
	// End Define Sensor Ports
public:
	Robot() :

		 mainJoystick(MainJoystickPort),


		 leftWheel(LeftDriveMotorPort),
		 rightWheel(RightDriveMotorPort),
		 leftShoot(LeftShootMotorPort),
		 rightShoot(RightShootMotorPort),
		 armLift(ArmLiftMotorPort),
		 kicker(KickerMotorPort),


		 rightEncoder(RightEncoderPort1, RightEncoderPort2, false, Encoder::k4X),
		 leftEncoder(LeftEncoderPort1, LeftEncoderPort2, false, Encoder::k4X)




{
		DriveGyro = new AHRS(SPI::Port::kMXP);

		rightEncoder.SetSamplesToAverage(5);
		rightEncoder.SetDistancePerPulse(1.0 / 360.0 * 2.0 * 3.1415 * 1.5);
		rightEncoder.SetMinRate(1.0);



		leftEncoder.SetSamplesToAverage(5);
		leftEncoder.SetDistancePerPulse(1.0 / 360.0 * 2.0 * 3.1415 * 1.5);
		leftEncoder.SetMinRate(1.0);

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





		/*chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);*/

	}



	void AutonomousInit()
	{
		/*autoSelected = *((std::string*)chooser->GetSelected());
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}*/
	}

	void AutonomousPeriodic()
	{
		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{

	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
