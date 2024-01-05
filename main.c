//FUNCTION PROTOTYPES
void configureAllSensors();
int candyColour ();
void pushCandy (int candy, float ENC_LIM);
int stock (int candy);
int moneyPayment ();
int activity ();
int restock ();
const float distance_kitkat = 1.5; //actual is 3
const float distance_smarties = 1.2; //actual measurement 2.5
const float distance_coffeecrisp = 1.5; //3
const float CM_TO_ENC = 360/(2*PI*1);

const float ENC_LIM_k = distance_kitkat* CM_TO_ENC;
const float ENC_LIM_s = distance_smarties* CM_TO_ENC;
const float ENC_LIM_c = distance_coffeecrisp* CM_TO_ENC;

task main()
{
	// pass by ref--changes based on the stock
	float a = ENC_LIM_k; // encoder limit for kitcat
	float b = ENC_LIM_s; // encoder limit for smarties
	float c = ENC_LIM_c; // encoder limit for coffee_crisp

	// configures sensors
	configureAllSensors();

	int active = activity();

	// machine starts while it detects activity
  while (active==1)
  {
  	//start up
  	displayBigTextLine (5, "Welcome!");
		wait1Msec (2000);
		displayBigTextLine (5, "Select Candy:");
		wait1Msec (4000);
		int candy = 0;

		//detect candy colour for 2s
		time1[T3] = 0;
		int clock = time1[T3];
		while (clock < 2000)
		{
			candy = candyColour();
			clock = time1[T3];
		}

    // checks for money payment
    if (candy == 0)
    {
    	active = 0;
    }

    int paid = moneyPayment();
    if (paid == 0)
    {
    	displayBigTextLine (5, "No Payment!");
      displayBigTextLine (7, "Terminated."); // should go back to welcome screen
      wait1Msec (3000);
      eraseDisplay ();
    }
    // if money is paid, machine pushes out candy
    else
    {
    	if (candy == 1)
      {
      	pushCandy (candy, a);
      }
     	else if (candy == 2)
      {
      	pushCandy (candy, b);
      }
      else
    	{
      	pushCandy (candy, c);
      }
      wait1Msec(2000);
      active = activity();
 		}
 		active = activity ();
 		active = restock();
	}//while loop closing brace
}//task main closing brace

void configureAllSensors()
{
	SensorType[S1] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[S2] = sensorEV3_Ultrasonic;
	wait1Msec(50);
	SensorType[S3] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S3] = modeEV3Color_Color;
	wait1Msec(50);
	nMotorEncoder[motorA]=nMotorEncoder[motorB]=nMotorEncoder[motorC]= 0;
}

int candyColour () // works
{
	// 5 is red, 2 is blue, 4 is yellow
	while (!getButtonPress (buttonAny)&& !SensorValue[S3] > 1) // test it
	{}
	eraseDisplay ();
	int candy = 0;
	if (SensorValue[S3] == 5 || getButtonPress(buttonRight))
	{
		candy = 1;
		displayBigTextLine(5, "KitKat");
    wait1Msec (3000);
    eraseDisplay ();
	}
	else if (SensorValue[S3] == 2 || getButtonPress(buttonUp))
	{
		candy = 2;
		displayBigTextLine(5, "Smarties");
    wait1Msec (3000);
    eraseDisplay ();
	}
	else if (SensorValue[S3] == 4 || getButtonPress(buttonLeft))
	{
		candy = 3;
		displayBigTextLine(5, "Coffee Crisp");
    wait1Msec (3000);
    eraseDisplay ();
	}
	else
	{
		candy = 0;
		displayBigTextLine(5, "None");
	}
	return candy;
}

int stock (int candy)
{
    int stock_of_candy = 0;
    if (candy == 1)
    {
        stock_of_candy = 4 - round((nMotorEncoder[motorA])/(ENC_LIM_k/2));
    }
    if (candy == 2)
    {
        stock_of_candy = 5 - round((nMotorEncoder[motorB])/(ENC_LIM_s/2));
    }
    if (candy == 3)
    {
        stock_of_candy = 5 - round((nMotorEncoder[motorC])/(ENC_LIM_c/2));
  	}
		wait1Msec(2000);
		eraseDisplay();
    return stock_of_candy;
}

int restock ()
{
  if (stock(1)==0 || stock(2)==0 || stock(3)==0)
	{
        displayBigTextLine (5, "Initiating restock!");
        displayBigTextLine (7, "Enter password:");
    		while (!getButtonPress(buttonUp))
        {}
        while (!getButtonPress(buttonRight))
        {}
        while (!getButtonPress(buttonDown))
        {}
        while (!getButtonPress(buttonLeft))
        {}
      	configureAllSensors();
        nMotorEncoder[motorA] = nMotorEncoder[motorB] = nMotorEncoder[motorC] = 0;
        eraseDisplay ();
        displayBigTextLine (5, "Restock complete");
        wait1Msec (3000);
        eraseDisplay ();
        return 0;
    }
    return 1; // if there is stock
}

int moneyPayment ()
{
	displayBigTextLine(5, "Enter payment");
	time1 [T1] = 0;
	int money = 0;
	while( time1 [T1] < 7000 )
	{
		if (SensorValue[S1]== 1)
		{
			money = 1;
			displayBigTextLine (5, "Payment received");
		}
	}
	eraseDisplay ();
 	return money;
}

void pushCandy (int candy, float ENC_LIM)
{
	if (candy == 1)
	{
		int initialEncoder = nMotorEncoder[motorA];
		motor[motorA] = 3;
		if (initialEncoder == 0)
		{
			while(nMotorEncoder[motorA] < ENC_LIM)
				{}
			motor [motorA] = 0;
		}
		else
		{
			while(nMotorEncoder[motorA] < initialEncoder + ENC_LIM/2)
				{}
			motor [motorA] = 0;
		}
	}

  if (candy == 2)
  {
   	int initialEncoder = nMotorEncoder[motorB];
		motor[motorB] = 3;
		if (initialEncoder == 0)
		{
			while(nMotorEncoder[motorB] < ENC_LIM)
				{}
			motor [motorB] = 0;
		}
		else
		{
			while(nMotorEncoder[motorB] < initialEncoder + ENC_LIM/2)
				{}
			motor [motorB] = 0;
		}
	}

	if (candy == 3)
	{
		int initialEncoder = nMotorEncoder[motorC];
		motor[motorC] = 3;
		if (initialEncoder == 0)
		{
			while(nMotorEncoder[motorC] < ENC_LIM)
			{}
			motor [motorC] = 0;
		}
		else
		{
			while(nMotorEncoder[motorC] < initialEncoder + ENC_LIM/2)
			{}
			motor [motorC] = 0;
		}
	}
    displayBigTextLine (5, "Dispensed!");
    displayBigTextLine (7, "Thank you!");
    wait1Msec (2000);
    eraseDisplay ();
	return;
}

int activity ()
{
	int active = 0;
	const int PERSON_DIST = 50;
	while(SensorValue[S2] > PERSON_DIST)
	{
		time1 [T2] = 0; //resets timer2
		while (time1[T2] < 60000)
		{
			if(SensorValue[S2]<=PERSON_DIST)
			{
				return 1;
			}
			else
			{}
		}
		displayBigTextLine (5, "Inactive");
		displayBigTextLine (7, "Shutting Down");
		active = 0;
	}
	while (SensorValue[S2] <= PERSON_DIST)
	{
		active = 1;
	}
	return active;
}
