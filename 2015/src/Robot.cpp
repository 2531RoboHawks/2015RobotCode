#include "WPILib.h"

class Robot: public IterativeRobot
{
	Joystick *rstick = new Joystick(1);
	Joystick *lstick = new Joystick (2);
	Talon *talon = new Talon(0);
	RobotDrive *robot = new RobotDrive(1,2);
private:
	LiveWindow *lw;

	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
	}

	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		talon->Set((float) rstick->GetRawButton(2), 0);
		robot->ArcadeDrive(rstick->GetY(),rstick->GetX());
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
