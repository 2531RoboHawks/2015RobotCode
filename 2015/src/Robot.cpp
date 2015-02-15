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
	int step;
	float t = 0;

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
		t = 0.0f; //don't smash Peter's legs
		step = 0; //Always start with step zero
	}

	void AutonomousPeriodic() {
		switch (step) {
		case 0:					//First grip the tote
			grip->Set(-1.0f);
			if (t>1.25f) {
				t = 0;
				step = step+1;
			}
			break;
		case 1:					//Then lift it
			grip->Set(0);
			spindleA->Set(-0.3f);
			spindleB->Set(-0.3f);
			if (t>0.75f) {
				t = 0;
				step = step+1;
			}
			break;
		case 2:					//Turn to the center
			spindleA->Set(-0.05f);
			spindleB->Set(-0.05f);
			robot->MecanumDrive_Cartesian(0.0f,-0.4f,0);
			if (t>1.1f) {
				t = 0;
				step = step+1;			}
			break;
		case 3:					//Drive forward
			robot->MecanumDrive_Cartesian(0,0.0,0.5);
			if (t>1.8) {
				t = 0;
				step = step+1;
			}
			break;
		case 4:					//Release the tote
			grip->Set(1.0f);
			if (t>1) {
				t = 0;
				step = step+1;
			}
			break;
		case 5:					//Reverse, to prove it's dropped
			grip->Set(0);
			robot->MecanumDrive_Cartesian(0,0,-.25);
			if (t>0.5f) {
				t = 0;
				step = step+1;
			}
			break;
		}
		SmartDashboard::PutNumber("Time:",t);
		SmartDashboard::PutNumber("Step:",step);
		t = t+0.05;
		Wait(0.05);
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
