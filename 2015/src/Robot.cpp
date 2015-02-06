#include "WPILib.h"
#include "math.h"

class Robot: public IterativeRobot {
	//Joysticks
	Joystick *rstick 	= new Joystick(1);
	Joystick *lstick 	= new Joystick(0);

	//Motor controllers
	RobotDrive *robot 	= new RobotDrive(0, 1, 2, 3);
	Talon *spindleA 	= new Talon(4);
	Talon *spindleB		= new Talon(5);
	Talon *grip 		= new Talon(6);
	Servo *cam_motor 	= new Servo(7);

	//Camera output & variables
	CameraServer *server = CameraServer::GetInstance();
	float camrot = 0.1f;
	//enviroment/autonomous variables.
	int position;

private:
	LiveWindow *lw;

	void RobotInit() {
		lw = LiveWindow::GetInstance();

		//Set camera position
		cam_motor->Set(camrot);
		//Begin camera stream
		server->SetQuality(50);
		server->StartAutomaticCapture("cam1");
	}

	void AutonomousInit() {
		//Clamp tote
		grip->Set(1.0);
		Wait(.25);
		grip->Set(0);
		spindleA->Set(1);
		spindleB->Set(1);
		robot->MecanumDrive_Cartesian(1,0,0);
		Wait(1.50);
		spindleA->Set(0);
		spindleB->Set(0);
		Wait(.50);
		robot->MecanumDrive_Cartesian(0,0,0);
		grip->Set(-1.0);
		Wait(.25);
		grip->Set(0);


	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		//Control camera servo with 2 and 3 buttons on RSTICK
		camrot = camrot + ((float) rstick->GetRawButton(3)) * 0.01
				- ((float) rstick->GetRawButton(2)) * 0.01;
		cam_motor->Set(camrot);

		//Drive the robot with mecanum drive
		//RSTICK X is strafe, RSTICK buttons 4 & 5 are rotation, RSTICK Y is f/b
		robot->MecanumDrive_Cartesian(pow(rstick->GetX(), 3), //curved for precision control
				-pow(lstick->GetX(),3), -pow(rstick->GetY(),3));

		//Control the forklift's grip actuator TEMPORARY
		grip->Set(
				((float) lstick->GetRawButton(3))
						- ((float) lstick->GetRawButton(2)));

		//Control the forklift's vertical motion (LTRIGGER + LSTICK Y)
		//todo: electronic braking
		if (lstick->GetTrigger()) {
			spindleA->Set(pow(-lstick->GetY(),3));
			spindleB->Set(pow(-lstick->GetY(),3));
		}
	}

	void TestPeriodic() {
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
