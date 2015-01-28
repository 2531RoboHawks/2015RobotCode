#include "WPILib.h"
#include "math.h"

class Robot: public IterativeRobot {
	//Joysticks
	Joystick *rstick 	= new Joystick(1);
	Joystick *lstick 	= new Joystick(0);

	//Motor controllers
	RobotDrive *robot 	= new RobotDrive(0, 1, 2, 3);
	Talon *spindle 		= new Talon(4);
	Talon *grip 		= new Talon(5);
	Servo *cam_motor 	= new Servo(6);
	Servo *ultra_motor  = new Servo(7);

	//Ultrasonic object & variables
	AnalogInput *ultra  = new AnalogInput(3);
	float vpi = 5.0/512;
	float distance = 0.0f;

	//Camera output & variables
	CameraServer *serv  = CameraServer::GetInstance();
	float camrot = 0.5f;
private:
	LiveWindow *lw;

	void RobotInit() {
		lw = LiveWindow::GetInstance();

		//Begin camera stream
		serv->SetQuality(50);
		serv->StartAutomaticCapture("cam1");
	}

	void AutonomousInit() {

	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		//Control camera servo with 2 and 3 buttons on RSTICK
		camrot = camrot + ((float) rstick->GetRawButton(3))*0.01 - ((float) rstick->GetRawButton(2))*0.01;
		cam_motor->Set(camrot);

		//Drive the robot with mecanum drive
		//RSTICK X is strafe, RSTICK Y is f/b, RSTICK buttons 4 & 5 are rotation
		robot->MecanumDrive_Cartesian(pow(rstick->GetX(),3),  	//curved for precision control
				(float) rstick->GetRawButton(4)-(float) rstick->GetRawButton(5),
				-rstick->GetY());

		//Control the forklift's grip actuator
		grip->Set(((float) lstick->GetRawButton(3))-((float) lstick->GetRawButton(2)));

		//Control the forklift's vertical motion (LTRIGGER + LSTICK Y)
		//todo: electronic braking
		if(lstick->GetTrigger()){

			spindle->Set(-lstick->GetY());
		}
	}
	void TestPeriodic() {
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
