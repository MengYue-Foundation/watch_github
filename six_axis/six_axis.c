#include <six_axis.h>


static int fd = 0;

void checkRC(int rc, char *text) {
  if (rc < 0) {
    printf("Error: %s - %d\n");
    exit(-1);
  }
}

static inline int i2c_smbus_access (int fd, char rw, uint8_t command, int size, union i2c_smbus_data *data)
{
  struct i2c_smbus_ioctl_data args ;

  args.read_write = rw ;
  args.command    = command ;
  args.size       = size ;
  args.data       = data ;
  return ioctl (fd, I2C_SMBUS, &args) ;
}

int I2CReadReg8 (int fd, int reg)
{
  union i2c_smbus_data data;

  if (i2c_smbus_access (fd, I2C_SMBUS_READ, reg, I2C_SMBUS_BYTE_DATA, &data))
    return -1 ;
  else
    return data.byte & 0xFF ;
}


int I2CWriteReg8 (int fd, int reg, int value)
{
  union i2c_smbus_data data ;

  data.byte = value ;
  return i2c_smbus_access (fd, I2C_SMBUS_WRITE, reg, I2C_SMBUS_BYTE_DATA, &data) ;
}

int I2CSetupInterface (const char *device, int devId)
{
  int fd ;

  if ((fd = open (device, O_RDWR)) < 0)
    return -1;
  if (ioctl (fd, I2C_SLAVE, devId) < 0)
    return -1;
  return fd ;
}


/*
 * wiringPiI2CSetup:
 *	Open the I2C device, and regsiter the target device
 *********************************************************************************
 */

int I2CSetup (const int devId)
{
  int rev ;

  int fd ;

  if ((fd = open (SIX_AXIS_I2C_BUS_NAME, O_RDWR)) < 0)
    return -1;
  if (ioctl (fd, I2C_SLAVE, devId) < 0)
    return -1;
  return fd ;

}

int six_axis_module_init()
{
	
	printf("MPU6050 starting\n");
	// Open an I2C connection
	fd = I2CSetup(MPU6050_ADDRESS);
	checkRC(fd, "wiringPiI2CSetup");
	
	// Perform I2C work
	I2CWriteReg8(fd, MPU6050_REG_PWR_MGMT_1, 0);
	usleep(100);
	return 0;
}
int get_six_axis_status() {

#if 1
  // Setup Wiring Pi
  float accelX_f = 0;
  float accelY_f = 0;
  float accelZ_f = 0;



    int msb = I2CReadReg8(fd, MPU6050_REG_DATA_START);
    int lsb = I2CReadReg8(fd, MPU6050_REG_DATA_START+1);
    short accelX = msb << 8 | lsb;

    msb = I2CReadReg8(fd, MPU6050_REG_DATA_START+2);
    lsb = I2CReadReg8(fd, MPU6050_REG_DATA_START+3);
    short accelY = msb << 8 | lsb;

    msb = I2CReadReg8(fd, MPU6050_REG_DATA_START+4);
    lsb = I2CReadReg8(fd, MPU6050_REG_DATA_START+5);
    short accelZ = msb << 8 | lsb;

    msb = I2CReadReg8(fd, MPU6050_REG_DATA_START+6);
    lsb = I2CReadReg8(fd, MPU6050_REG_DATA_START+7);
    short temp = msb << 8 | lsb;

    msb = I2CReadReg8(fd, MPU6050_REG_DATA_START+8);
    lsb = I2CReadReg8(fd, MPU6050_REG_DATA_START+9);
    short gyroX = msb << 8 | lsb;

    msb = I2CReadReg8(fd, MPU6050_REG_DATA_START+10);
    lsb = I2CReadReg8(fd, MPU6050_REG_DATA_START+11);
    short gyroY = msb << 8 | lsb;

    msb = I2CReadReg8(fd, MPU6050_REG_DATA_START+12);
    lsb = I2CReadReg8(fd, MPU6050_REG_DATA_START+13);
    short gyroZ = msb << 8 | lsb;

#if 1
    printf("accelX=%f, accelY=%f, accelZ=%f\n", accelX/A_SCALE, accelY/A_SCALE, accelZ/A_SCALE);
#endif

    accelX_f = accelX/A_SCALE;
    accelY_f = accelY/A_SCALE;
    accelZ_f = accelZ/A_SCALE;

#endif


        if (accelY_f > -1.0  && accelY_f < -0.7) {
            if (accelZ_f > -0.5 && accelZ_f < -0.1) {
                printf("hhhhhhhhhhhhh\n");
                return 1;
            }
        }
	return 0;
}	

#if 0
int main() {
	int iStatus = 0;
	six_axis_module_init();
	while (1) {
		iStatus = get_six_axis_status();
		usleep(100*1000);
	}
	return 0;
}
#endif
