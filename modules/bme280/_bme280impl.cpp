#include "_bme280impl.h"
#include "../iper.h"

bool read_values(IoOps* pi2c, int& t, int& h, int& pr)
{

    if(pi2c->iopen(0x2) == false){
	std::cout << "cannot open device \n";
	return false;
    }

    // Read 24 bytes of data from register(0x88)
    uint8_t b1[24] = {0};

    if(0==pi2c->bread(b1, 24, 0x88))
    {
	std::cout << "read failes \n";
        pi2c->iclose();
        return false;
    }

    // Convert the data
    // temp coefficents
    int dig_T1 = (b1[0] + b1[1] * 256);
    int dig_T2 = (b1[2] + b1[3] * 256);
    if(dig_T2 > 32767)
    {
        dig_T2 -= 65536;
    }
    int dig_T3 = (b1[4] + b1[5] * 256);
    if(dig_T3 > 32767)
    {
        dig_T3 -= 65536;
    }

    // pressure coefficents
    int dig_P1 = (b1[6] + b1[7] * 256);
    int dig_P2 = (b1[8] + b1[9] * 256);
    if(dig_P2 > 32767)
    {
        dig_P2 -= 65536;
    }
    int dig_P3 = (b1[10] + b1[11] * 256);
    if(dig_P3 > 32767)
    {
        dig_P3 -= 65536;
    }
    int dig_P4 = (b1[12] + b1[13] * 256);
    if(dig_P4 > 32767)
    {
        dig_P4 -= 65536;
    }
    int dig_P5 = (b1[14] + b1[15] * 256);
    if(dig_P5 > 32767)
    {
        dig_P5 -= 65536;
    }
    int dig_P6 = (b1[16] + b1[17] * 256);
    if(dig_P6 > 32767)
    {
        dig_P6 -= 65536;
    }
    int dig_P7 = (b1[18] + b1[19] * 256);
    if(dig_P7 > 32767)
    {
        dig_P7 -= 65536;
    }
    int dig_P8 = (b1[20] + b1[21] * 256);
    if(dig_P8 > 32767)
    {
        dig_P8 -= 65536;
    }
    int dig_P9 = (b1[22] + b1[23] * 256);
    if(dig_P9 > 32767)
    {
        dig_P9 -= 65536;
    }

    // Read 1 byte of data from register(0xA1)
    //    reg[0] = 0xA1;
    //    write(file, reg, 1);
    uint8_t data[8] = {0};
    //    read(file, data, 1);

    pi2c->bread(data, 1, 0xA1);

    int dig_H1 = data[0];



    // Read 7 bytes of data from register(0xE1)
    //    reg[0] = 0xE1;
    //    write(file, reg, 1);
    //    read(file, b1, 7);

    pi2c->bread(b1, 7, 0xE1);

    // Convert the data
    // humidity coefficents
    int dig_H2 = (b1[0] + b1[1] * 256);
    if(dig_H2 > 32767)
    {
        dig_H2 -= 65536;
    }
    int dig_H3 = b1[2] & 0xFF ;
    int dig_H4 = (b1[3] * 16 + (b1[4] & 0xF));
    if(dig_H4 > 32767)
    {
        dig_H4 -= 65536;
    }
    int dig_H5 = (b1[4] / 16) + (b1[5] * 16);
    if(dig_H5 > 32767)
    {
        dig_H5 -= 65536;
    }
    int dig_H6 = b1[6];
    if(dig_H6 > 127)
    {
        dig_H6 -= 256;
    }

    // Select control humidity register(0xF2)
    // Humidity over sampling rate = 1(0x01)
    uint8_t config[2] = {0};
    //    config[0] = 0xF2;
    config[1] = 0x01;
    //    write(file, config, 2);

    pi2c->bwrite(&config[1],1,0xf2);


    // Select control measurement register(0xF4)
    // Normal mode, temp and pressure over sampling rate = 1(0x27)
    config[0] = 0xF4;
    config[1] = 0x27;
    // write(file, config, 2);

    pi2c->bwrite(&config[1],1,0xf4);

    // Select config register(0xF5)
    // Stand_by time = 1000 ms(0xA0)
    config[0] = 0xF5;
    config[1] = 0xA0;
    //    write(file, config, 2);

    pi2c->bwrite(&config[1],1,0xf5);

    //uint8_t reg[4];
    // Read 8 bytes of data from register(0xF7)
    // pressure msb1, pressure msb, pressure lsb, temp msb1, temp msb, temp lsb, humidity lsb, humidity msb
    //reg[0] = 0xF7;
    //    write(file, reg, 1);
    //    read(file, data, 8);

    pi2c->bread(data,8,0xF7);


    // Convert pressure and temperature data to 19-bits
    long adc_p = ((long)(data[0] * 65536 + ((long)(data[1] * 256) + (long)(data[2] & 0xF0)))) / 16;
    long adc_t = ((long)(data[3] * 65536 + ((long)(data[4] * 256) + (long)(data[5] & 0xF0)))) / 16;
    // Convert the humidity data
    long adc_h = (data[6] * 256 + data[7]);

    // Temperature offset calculations
    float var1 = (((float)adc_t) / 16384.0 - ((float)dig_T1) / 1024.0) * ((float)dig_T2);
    float var2 = ((((float)adc_t) / 131072.0 - ((float)dig_T1) / 8192.0) *
                  (((float)adc_t)/131072.0 - ((float)dig_T1)/8192.0)) * ((float)dig_T3);
    float t_fine = (long)(var1 + var2);
    ///float cTemp = (var1 + var2) / 5120.0;
    float cTemp = (var1 + var2) / 512.00;
    float fTemp = cTemp;// * 1.8 + 32;
    t = fTemp / 10.0; //C

    // Pressure offset calculations
    var1 = ((float)t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((float)dig_P6) / 32768.0;
    var2 = var2 + var1 * ((float)dig_P5) * 2.0;
    var2 = (var2 / 4.0) + (((float)dig_P4) * 65536.0);
    var1 = (((float) dig_P3) * var1 * var1 / 524288.0 + ((float) dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((float)dig_P1);
    float p = 1048576.0 - (float)adc_p;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((float) dig_P9) * p * p / 2147483648.0;
    var2 = p * ((float) dig_P8) / 32768.0;
    //float pressure = (p + (var1 + var2 + ((float)dig_P7)) / 16.0) / 100;
    float pressure = (p + (var1 + var2 + ((float)dig_P7)) / 16.0); // mm mhg
    pr = pressure;

    // Humidity offset calculations
    float var_H = (((float)t_fine) - 76800.0);
    var_H = (adc_h - (dig_H4 * 64.0 + dig_H5 / 16384.0 * var_H)) * (dig_H2 / 65536.0 * (1.0 + dig_H6 / 67108864.0 * var_H * (1.0 + dig_H3 / 67108864.0 * var_H)));
    float humidity = 1.0* (var_H * (1.0 -  dig_H1 * var_H / 524288.0));
    if(humidity > 100.0)
    {
        humidity = 100.0;
    }
    else if(humidity < 0.0)
    {
        humidity = 0.0;
    }
    h = humidity;
    pi2c->iclose();

    return true;
}
