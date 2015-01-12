#include "WPILib.h"
class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick;
	Jaguar pickup;
	public:
	RobotDemo():
	myRobot(1, 2, 5, 6),
	stick(2), pickup(7)
	{
		myRobot.SetExpiration(0.1);
	}
/**
* Drive left & right motors for 2 seconds then stop
*/
void Autonomous() {
}
/**
* Runs the motors with arcade steering. 
*/
void OperatorControl()
{
	myRobot.SetSafetyEnabled(true);
	while (IsOperatorControl())
	{
		myRobot.MecanumDrive_Cartesian(stick.GetRawButton(5)-stick.GetRawButton(4), stick.GetY(), -stick.GetX(), 0.0f); // drive with 
		pickup.Set(stick.GetRawButton(1));
		Wait(0.005);	// wait for a motor update time
	}
}
/**
* Runs during test mode
*/
void Test() {
}
};
START_ROBOT_CLASS(RobotDemo);
