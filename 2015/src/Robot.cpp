#include "WPILib.h"
#include "math.h"

class Robot: public IterativeRobot {
	//Joysticks
	Joystick *rstick 	= new Joystick(1);
	Joystick *lstick 	= new Joystick(0);

	//Motor controllers
	RobotDrive *robot 	= new RobotDrive(0, 1, 2, 3);
	CANTalon *spindleA 	= new CANTalon(2);
	CANTalon *spindleB	= new CANTalon(3);
	CANTalon *grip 		= new CANTalon(1);
	Servo *cam_motor 	= new Servo(4);

	//Camera output & variables
	CameraServer *server = CameraServer::GetInstance();
	float camrot 		= 0.1f;
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
		server->StartAutomaticCapture("cam2");
	}

	void AutonomousInit() {
		//Clamp tote and raise it
		grip->Set(1.0);
		Wait(.25);
		grip->Set(0);
		spindleA->Set(0.1f);
		spindleB->Set(0.1f);
		Wait(0.5f);
		spindleA->Set(0.05);
		spindleB->Set(0.05);
		//Turn and drive to the center
		robot->MecanumDrive_Cartesian(0,0.2f,0);
		Wait(1.0f);
		robot->MecanumDrive_Cartesian(0,0,0.5f);
		Wait(3);
		robot->MecanumDrive_Cartesian(0,0,0);
		//Drop tote
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
				-pow(lstick->GetX(),3), -rstick->GetY());

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
