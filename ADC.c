#include <REGX51.H>
#include <INTRINS.H>

sbit scl = P2^0;
sbit sda = P2^1;
sbit green = P2^2;
sbit red = P2^3;

void start();
void delay();
void nack();
void stop();
void write(unsigned char);
void conversion(unsigned char);
void read();

bit c = 0;
unsigned char res, v11, v15;
bit d;
unsigned char v1;

void main()
{
    green = red = 0;
    SCON = 0x50;
    TMOD = 0x20;
    TH1 = -3;
    TR1 = 1;
    
    start();
    write(0x90);
    write(0x01);

    while (1)
    {
        green = 1;
        red = 0;
        start();
        write(0x91);
        read();
        nack();
    }
    stop();
}

void delay()
{
    _nop_();
    _nop_();
}

void nack()
{
    sda = 1;
    scl = 1;
    delay();
    scl = 0;
}

void start()
{
    scl = 1;
    sda = 1;
    delay();
    sda = 0;
    delay();
    scl = 0;
}

void stop()
{
    scl = 1;
    sda = 0;
    delay();
    sda = 1;
    scl = 0;
    delay();
}

void write(unsigned char val)
{
    unsigned char v2 = 0x80, v3, v5;
    v5 = val;
    for (v3 = 0; v3 < 8; v3++)
    {
        sda = v5 & v2;
        scl = 1;
        delay();
        scl = 0;
        v5 = v5 << 1;
    }
    c = sda;
    scl = 1;
    delay();
    scl = 0;
    if (c == 1)
    {
        red = 1;
        green = 0;
        stop();
    }
    sda = 1;
}

void conversion(unsigned char val)
{
    unsigned char v12, v13, v14;
    v12 = val;
    v13 = v12 / 10;
    v14 = v12 % 10;
    v13 = v13 | 0x30;
    v14 = v14 | 0x30;
    SBUF = v13;
    while (TI == 0);
    TI = 0;
    SBUF = v14;
    while (TI == 0);
    TI = 0;
    SBUF = 0x0D;
    while (TI == 0);
    TI = 0;
}

void read()
{
    unsigned char ch = 0x00, v16;
    bit m = 0;
    for (v16 = 0; v16 <= 7; v16++)
    {
        scl = 1;
        delay();
        m = sda;
        scl = 0;
        if (m == 1)
        {
            ch |= 0x01;
        }
        if (v16 <= 6)
            ch <<= 1;
        delay();
    }
    conversion(ch);
}
