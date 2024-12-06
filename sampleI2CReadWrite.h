#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string>
class i2cReadWrite {
        public:
        int file_i2c;
        unsigned char buffer[60];
        int length=2;

        void setup(){
            file_i2c = open("/dev/i2c-1",O_RDWR);
            if(ioctl(file_i2c,I2C_SLAVE,0x20) < 0)
            {
                printf("i2c not open");
            }
            printf("file_i2c. %d\n",file_i2c);
        }

        int readi2cData(){
                int res=0;
                if (read(file_i2c, buffer, length) != length) {
                        printf("Failed to read from the i2c bus.\n");
                }
                else {
                    for (int i=0;i<20;i++) {
                        printf("Data read: %d %d\n",i, buffer[i]);
                    }

                }
                return res;
        }

        void writeBytes(){
          if (write(file_i2c, buffer, length) != length)
          {
                  printf("Failed to write to the i2c bus. %d  %d \n",buffer[0],buffer[1]);
          }
        }
};
