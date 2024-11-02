#include "base.h"
#include "classwiz_bsp.h"

// SPI Mode definitions
inline void spi_transmit_mode()
{
    SIO0MOD0 = (unsigned short)0b0101; // 8bit length, Transmit mode, MSB first
}

inline void spi_receive_mode()
{
    SIO0MOD0 = (unsigned short)0b0011; // 8bit length, Receive mode, MSB first
}

// Basic SPI operations
void spi_send(byte dat)
{
    spi_transmit_mode();
    SIO0CON = 1;
    SIO0BUFL = dat;
    while (SIO0CON == 1)
        __asm("nop");
}

byte spi_recv()
{
    spi_receive_mode();
    SIO0CON = 1;
    while (SIO0CON == 1)
        __asm("nop");
    return SIO0BUFL;
}

// SD card command functions
void sd_send_command(byte command, byte p[4], byte crc)
{
    spi_send(0b01000000 | command);
    spi_send(p[0]);
    spi_send(p[1]);
    spi_send(p[2]);
    spi_send(p[3]);
    spi_send(crc);
}

byte sd_read_response()
{
    byte response;
    byte retry = 0;
    do
    {
        response = spi_recv();
        retry++;
    } while (response == 0xFF && retry < 8);
    return response;
}

// SD card initialization
byte sd_init()
{
    P1D = 0;
    P1DIR = 1;
    P1CON0 = 6;
    P1CON1 = 7;
    P1MOD0 = 7;
    P1MOD1 = 0;

    // Initialize SPI
    SIO0CON = 0;
    SIO0MOD1 = 0;
    spi_transmit_mode();
    SIO0BUFL = 0xFF;

    // CS high
    P1D |= 0b1000;

    // Send at least 74 clock cycles
    for (byte i = 0; i < 16; i++)
    {
        spi_send(0xFF);
    }

    // CS low to start initialization
    P1D &= ~0b1000;

    // Send CMD0
    byte cmd0_arg[4] = {0, 0, 0, 0};
    byte response;
    do
    {
        sd_send_command(0, cmd0_arg, 0x95);
        response = sd_read_response();
    } while (response != 0x01);

    // Send CMD8
    byte cmd8_arg[4] = {0, 0, 0x01, 0xAA};
    sd_send_command(8, cmd8_arg, 0x87);
    response = sd_read_response();

    if (response == 0x01)
    {
        // Read R7 response
        for (byte i = 0; i < 4; i++)
        {
            spi_recv();
        }
    }

    // Initialize card with ACMD41
    byte retry = 0;
    do
    {
        // CMD55 (APP_CMD)
        byte cmd55_arg[4] = {0, 0, 0, 0};
        sd_send_command(55, cmd55_arg, 0xFF);
        sd_read_response();

        // ACMD41
        byte acmd41_arg[4] = {0x40, 0, 0, 0};
        sd_send_command(41, acmd41_arg, 0xFF);
        response = sd_read_response();

        retry++;
        if (retry > 200)
            return 0; // Timeout error
    } while (response != 0);

    return 1; // Success
}

// Read block data (512 bytes)
byte sd_read_block(unsigned long addr, byte *buffer)
{
    byte cmd17_arg[4];

    // Convert block address to byte address
    // addr = addr << 9;
    cmd17_arg[0] = (addr >> 24) & 0xFF;
    cmd17_arg[1] = (addr >> 16) & 0xFF;
    cmd17_arg[2] = (addr >> 8) & 0xFF;
    cmd17_arg[3] = addr & 0xFF;

    // Send CMD17 (READ_SINGLE_BLOCK)
    sd_send_command(17, cmd17_arg, 0xFF);
    if (sd_read_response() != 0x00)
        return 0;

    // Wait for data token
    byte token;
    do
    {
        token = spi_recv();
    } while (token == 0xFF);

    if (token != 0xFE)
        return 0; // Data token error

    // Read 512 bytes
    for (unsigned int i = 0; i < 512; i++)
    {
        buffer[i] = spi_recv();
    }

    // Read CRC (2 bytes)
    spi_recv();
    spi_recv();

    return 1;
}

// Write block data (512 bytes)
byte sd_write_block(unsigned long addr, const byte *buffer)
{
    byte cmd24_arg[4];

    // Convert block address to byte address
    addr = addr << 9;
    cmd24_arg[0] = (addr >> 24) & 0xFF;
    cmd24_arg[1] = (addr >> 16) & 0xFF;
    cmd24_arg[2] = (addr >> 8) & 0xFF;
    cmd24_arg[3] = addr & 0xFF;

    // Send CMD24 (WRITE_BLOCK)
    sd_send_command(24, cmd24_arg, 0xFF);
    if (sd_read_response() != 0x00)
        return 0;

    // Send data token
    spi_send(0xFE);

    // Write 512 bytes
    for (unsigned int i = 0; i < 512; i++)
    {
        spi_send(buffer[i]);
    }

    // Send dummy CRC
    spi_send(0xFF);
    spi_send(0xFF);

    // Check if data was accepted
    byte response = spi_recv();
    if ((response & 0x1F) != 0x05)
        return 0;

    // Wait while card is busy
    while (spi_recv() == 0x00)
        ;

    return 1;
}

// Get SD card information
byte sd_get_info(unsigned long *capacity)
{
    byte csd[16];

    // Send CMD9 (SEND_CSD)
    byte cmd9_arg[4] = {0, 0, 0, 0};
    sd_send_command(9, cmd9_arg, 0xFF);
    if (sd_read_response() != 0x00)
        return 0;

    // Wait for data token
    byte token;
    do
    {
        token = spi_recv();
    } while (token == 0xFF);

    if (token != 0xFE)
        return 0;

    // Read CSD register
    for (byte i = 0; i < 16; i++)
    {
        csd[i] = spi_recv();
    }

    // Calculate card capacity
    if ((csd[0] >> 6) == 0x01)
    { // CSD version 2.0
        unsigned long c_size = ((unsigned long)(csd[7] & 0x3F) << 16) |
                               ((unsigned long)csd[8] << 8) |
                               csd[9];
        *capacity = (c_size + 1) * 512; // Capacity in KBytes
    }
    else
    { // CSD version 1.0
        unsigned long c_size = ((unsigned long)(csd[6] & 0x03) << 10) |
                               ((unsigned long)csd[7] << 2) |
                               ((unsigned long)(csd[8] >> 6));
        byte c_size_mult = ((csd[9] & 0x03) << 1) |
                           ((csd[10] >> 7));
        byte read_bl_len = csd[5] & 0x0F;
        *capacity = (c_size + 1) * (1 << (c_size_mult + 2)) * (1 << read_bl_len) / 1024; // Capacity in KBytes
    }

    return 1;
}