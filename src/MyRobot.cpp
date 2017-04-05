#include <cstdio>
#include <memory>

#include "WPILib.h"
/*
To get analog values other than X and Y, use Joystick.getRawAxis(), numbers as per driver station
0=L X Axis
1=L Y Axis
2=L Trigger
3=R Trigger
4=R X Axis
5=R Y Axis
Buttons use .getRawButton(), start from 1 instead of 0, and run 1 to 10
1=A
2=B
3=X
4=Y
5=LB
6=RB
7=SELECT
8=START
9=Left Stick
10=Right Stick
*/

class MyRobot : public SampleRobot
{
    private:
        int joystickChannel = 0;
        std::unique_ptr<Talon> MotorLeft1;
        std::unique_ptr<Talon> MotorLeft2;
        std::unique_ptr<Talon> MotorRight1;
        std::unique_ptr<Talon> MotorRight2;
        std::unique_ptr<Talon> MotorClimber;
        std::unique_ptr<frc::XboxController> Drive;

    public:
        void RobotInit()
        {
            /*Robot initialization function*/
        	MotorRight1.reset(new Talon(0));
        	MotorRight2.reset(new Talon(1));
        	MotorLeft1.reset(new Talon(2));
        	MotorLeft2.reset(new Talon(3));
	        MotorClimber.reset(new Talon(4));
            //stick = new Joystick(driveChannel);
            Drive.reset(new frc::XboxController(0));//Driving controller

            CameraServer::GetInstance()->StartAutomaticCapture(0);
            CameraServer::GetInstance()->StartAutomaticCapture(1);
        }

        void arcadeDrive()
        {
            float straight=Drive->GetRawAxis(1);
            float turn=Drive->GetRawAxis(4);
            bool turbo=Drive->GetRawButton(5); //Left shoulder button acts as turbo
            double leftTrigger = Drive->GetTriggerAxis(frc::GenericHID::JoystickHand::kLeftHand);

            //Deadband logic

            if (std::abs(straight)<=0.2)
                straight=0;
            if (std::abs(turn)<=0.2)
                turn=0;
            if (!turbo)
            {
                straight=std::pow(straight,3)*0.5;
                turn=std::pow(turn,3)*0.5;
            }
            else
                std::printf("Turbo!"); //Screams "Turbo!" so we know it's going fast :P

            float right=turn+straight;
            float left=turn-straight;

            if (left>1)
                left=1;
            else if (left<-1)
                left=-1;
            if (right>1)
                right=1;
            else if (right<-1)
                right=-1;
            MotorLeft1->Set(left);
            MotorLeft2->Set(left);
            MotorRight1->Set(right);
            MotorRight2->Set(right);
            MotorClimber->Set(leftTrigger*0.5);
        }

        void Autonomous()
        {
            /*Runs the motors autonomously with tank steering*/
            std::printf("autonomous control mode entered");

            if (IsAutonomous() && IsEnabled())
            {
#define LEFT

#ifdef MIDDLE //goes straight to peg (gear)
            	float left = 0.5;
            	float right = -0.5;
            	double driveTime = 5;

            	MotorLeft1->Set(left);
            	MotorLeft2 ->Set(left);
            	MotorRight1->Set(right);
            	MotorRight2->Set(right);

            	Wait(driveTime); // wait for a motor update time


#endif
            	
#ifdef SIDE  //goes straight to pass line
            	float left = -0.5;
            	float right = 0.5;
            	double driveTimeSide = 5;

            	MotorLeft1->Set(left);
            	MotorLeft2->Set(left);
            	MotorRight1->Set(right);
            	MotorRight2->Set(right);

            	Wait(driveTimeSide); // wait for a motor update time
#endif

            	/*
            	float x = -0.5;
                float y = 0.5;

                std::printf("autonomous motor rates MotorLeft1=%f, MotorRight1=%f", x, y);
                MotorLeft1->Set(x);
                MotorLeft2->Set(x);
                MotorRight1->Set(y);
                MotorRight2->Set(y);

                Wait(0.02); // wait for a motor update time
                */
            }
        }

        void OperatorControl()
        {
            while (IsOperatorControl() && IsEnabled())
            {
                arcadeDrive();
            	Wait(0.02); // wait for a motor update time
            }
        }

        void Test()
        {
            std::printf("entering test mode");
        }
};

START_ROBOT_CLASS(MyRobot)
