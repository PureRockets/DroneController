#include <iostream>
#include <unistd.h>
#include "PracticalSocket.h"
#include "SFE_LSM9DS0.h"

using namespace std;

int main(int argc, char *argv[]) {

	UDPSocket sock;
	LSM9DS0 *imu;
	string destAddress = argv[1];
	int nodeVal = atoi(argv[2]);

	unsigned short destPort = 33333;
	char* sendString = new char[100];

	const long SLEEP_TIME = 400; //ms

	imu = new LSM9DS0(0x6B, 0x1D);


	imu->begin();

	  cout<<"Starting capture..."<<endl;

	  bool newAccelData = false;
	  bool newMagData = false;
	  bool newGyroData = false;
	  bool overflow = false;




	  // Loop and report data
	   while (1)
	   {
	     // First, let's make sure we're collecting up-to-date information. The
	     //  sensors are sampling at 100Hz (for the accelerometer, magnetometer, and
	     //  temp) and 95Hz (for the gyro), and we could easily do a bunch of
	     //  crap within that ~10ms sampling period.
	     while ((newGyroData & newAccelData & newMagData) != true)
	     {
	       if (newAccelData != true)
	       {
	         newAccelData = imu->newXData();
	       }
	       if (newGyroData != true)
	       {
	         newGyroData = imu->newGData();
	       }
	       if (newMagData != true)
	       {
	         newMagData = imu->newMData(); // Temp data is collected at the same
	                                       //  rate as magnetometer data.
	       }
	     }

	     newAccelData = false;
	     newMagData = false;
	     newGyroData = false;

	     // Of course, we may care if an overflow occurred; we can check that
	     //  easily enough from an internal register on the part. There are functions
	     //  to check for overflow per device.
	     overflow = imu->xDataOverflow() |
	                imu->gDataOverflow() |
	                imu->mDataOverflow();

	     if (overflow)
	     {
	       //cout<<"WARNING: DATA OVERFLOW!!!"<<endl;
	     }


	     // Calling these functions causes the data to be read from the IMU into
	     //  10 16-bit signed integer public variables, as seen below. There is no
	     //  automated check on whether the data is new; you need to do that
	     //  manually as above. Also, there's no check on overflow, so you may miss
	     //  a sample and not know it.
	     imu->readAccel();
	     imu->readMag();
	     imu->readGyro();
	     imu->readTemp();

	     // Print the unscaled 16-bit signed values.
//	     /*cout<<"-------------------------------------"<<endl;
//	     cout<<"Gyro x: "<<imu->gx<<endl;
//	     cout<<"Gyro y: "<<imu->gy<<endl;
//	     cout<<"Gyro z: "<<imu->gz<<endl;
//	     cout<<"Accel x: "<<imu->ax<<endl;
//	     cout<<"Accel y: "<<imu->ay<<endl;
//	     cout<<"Accel z: "<<imu->az<<endl;
//	     cout<<"Mag x: "<<imu->mx<<endl;
//	     cout<<"Mag y: "<<imu->my<<endl;
//	     cout<<"Mag z: "<<imu->mz<<endl;
//	     cout<<"Temp: "<<imu->temperature<<endl;
//	     cout<<"-------------------------------------"*/<<endl;

/*	     // Print the "real" values in more human comprehensible units.
	     cout<<"-------------------------------------"<<endl;
	     cout<<"Gyro x: "<<imu->calcGyro(imu->gx)<<" deg/s"<<endl;
	     cout<<"Gyro y: "<<imu->calcGyro(imu->gy)<<" deg/s"<<endl;
	     cout<<"Gyro z: "<<imu->calcGyro(imu->gz)<<" deg/s"<<endl;
	     cout<<"Accel x: "<<imu->calcAccel(imu->ax)<<" g"<<endl;
	     cout<<"Accel y: "<<imu->calcAccel(imu->ay)<<" g"<<endl;
	     cout<<"Accel z: "<<imu->calcAccel(imu->az)<<" g"<<endl;
	     cout<<"Mag x: "<<imu->calcMag(imu->mx)<<" Gauss"<<endl;
	     cout<<"Mag y: "<<imu->calcMag(imu->my)<<" Gauss"<<endl;
	     cout<<"Mag z: "<<imu->calcMag(imu->mz)<<" Gauss"<<endl;*/
	     // Temp conversion is left as an example to the reader, as it requires a
	     //  good deal of device- and system-specific calibration. The on-board
	     //  temp sensor is probably best not used if local temp data is required!
	     //cout<<"-------------------------------------"<<endl;



	     sprintf(sendString, "%d,%d,%d,%d", nodeVal, imu->ax/1000, imu->ay/1000, imu->az/1000);

	     //sprintf(sendString, "%d", imu->az/1000);

	     sock.sendTo(sendString, strlen(sendString), destAddress, destPort);
	     usleep(SLEEP_TIME * 1000);
	   }


	 	return MRAA_SUCCESS;
}
