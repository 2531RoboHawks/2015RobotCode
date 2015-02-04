#include "WPILib.h"
#include "math.h"
#include "time.h"
#include <ctime>

class Robot: public IterativeRobot {
	//Joysticks
	Joystick *rstick 	= new Joystick(1);
	Joystick *lstick 	= new Joystick(0);
	int up = 1;
	int down = 0;

	//Motor controllers
	RobotDrive *robot 	= new RobotDrive(0, 1, 2, 3);
	Talon *spindleA 	= new Talon(4);
	Talon *spindleB		= new Talon(5);
	Talon *grip 		= new Talon(6);
	Servo *cam_motor 	= new Servo(7);
	Servo *ultra_motor 	= new Servo(8);

	//Ultrasonic object & variables
	AnalogInput *ultra= new AnalogInput(3);
	float vpi = 5.0 / 512;
	float distance[10];
	float reverseVoltage = 0.0;

	//Camera output & variables
	CameraServer *server = CameraServer::GetInstance();
	float camrot = 0.25f;
	//enviroment/autonomous variables.
	int position;

private:
	LiveWindow *lw;

	void RobotInit() {
		lw = LiveWindow::GetInstance();

		//Begin camera stream
		server->SetQuality(50);
		server->StartAutomaticCapture("cam1");
	}

	void AutonomousInit() {
		//outside is the side with the tote facing wall
		// 0 is outside, 1 is middle...
		 position = 0;


	}

	void AutonomousPeriodic() {
		if(position == 0) {
			//Clamp tote
			grip->Set(1.0);
			Wait(.25);
			grip->Set(0);
			spindleA->Set(up);
			spindleB->Set(up);
			robot->MecanumDrive_Cartesian(1,0,0);
			Wait(1.50);
			spindleA->Set(0);
			spindleB->Set(0);
			Wait(.50);
			robot->MecanumDrive_Cartesian(0,0,0);
			grip->Set(-1.0);
			Wait(.25);
			grip->Set(0);


		}else if(position == 1){

		}else if(position == 2){

		}

	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		//Control camera servo with 2 and 3 buttons on RSTICK
		camrot = camrot + ((float) rstick->GetRawButton(3)) * 0.01
				- ((float) rstick->GetRawButton(2)) * 0.01;
		cam_motor->Set(camrot); //*/
		//Read and transmit ultrasonic data
		/*distance[1] = distance[0];
		distance[0] = ultra->GetAverageVoltage() / vpi;
		SmartDashboard::PutNumber("Distance:", distance[0]);
		//if the lift is getting closer....
		if ((distance[0] - distance[1]) < 0 && !lstick->GetTrigger()) {

			reverseVoltage >= 0.5 ? reverseVoltage = 0.5 : reverseVoltage += .01;
		} else //if lift is getting further away...
		if ((distance[0] - distance[1] > 0 && !lstick->GetTrigger())) {

			reverseVoltage <= 0 ? reverseVoltage = 0 : reverseVoltage -= .01;
		}//*/

		//Drive the robot with mecanum drive
		//RSTICK X is strafe, RSTICK Y is f/b, RSTICK buttons 4 & 5 are rotation
		robot->MecanumDrive_Cartesian(pow(rstick->GetX(), 3), //curved for precision control
				-pow(lstick->GetX(),3), -rstick->GetY());

		//Control the forklift's grip actuator TEMPORARY
		grip->Set(
				((float) lstick->GetRawButton(3))
						- ((float) lstick->GetRawButton(2)));

		//Control the forklift's vertical motion (LTRIGGER + LSTICK Y)
		//todo: electronic braking
		if (lstick->GetTrigger()) {

			spindleA->Set(-lstick->GetY());
			spindleB->Set(-lstick->GetY());
		} else {
			spindleA->Set(0);
			spindleB->Set(0);

		}
	}

	void TestPeriodic() {
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
