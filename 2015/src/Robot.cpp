#include "WPILib.h"

class Robot: public IterativeRobot {
	Joystick *rstick 	= new Joystick(1);
	Joystick *lstick 	= new Joystick(0);

	RobotDrive *robot 	= new RobotDrive(0, 1, 2, 3);
	Servo *cam_motor 	= new Servo(6);
	Talon *spindle 		= new Talon(4);
	Talon *grip 		= new Talon(5);

	CameraServer *server = CameraServer::GetInstance();
	float camrot = 0.75f;
private:
	LiveWindow *lw;

	void RobotInit() {
		lw = LiveWindow::GetInstance();
		server->SetQuality(50);
		server->StartAutomaticCapture("cam1");
	}

	void AutonomousInit() {

	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		camrot = camrot + ((float) rstick->GetRawButton(3))*0.01 - ((float) rstick->GetRawButton(2))*0.01;
		cam_motor->Set(camrot);
		robot->MecanumDrive_Cartesian(rstick->GetX(), rstick->GetY(), (float) rstick->GetRawButton(4)-(float) rstick->GetRawButton(5));
		grip->Set(((float) lstick->GetRawButton(3))-((float) lstick->GetRawButton(2)));
		if(rstick->GetTrigger()){
			spindle->Set(-lstick->GetY());
		}
	}
	void TestPeriodic() {
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
