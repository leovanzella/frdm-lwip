#include "enc28j60.h"
#include "spi.h"

static uint8_t Enc28j60Bank;
static int gNextPacketPtr;
uint8_t macaddr[] = {0x00, 0xFF, 0x7A, 0xA5, 0x06, 0xDD}; // fake mac, please change it

void wait(char mseconds)
{
	int i;
		for(i = 1000; i >=0; i--)
		{
			
		}
}

static void enableChip(void) 
{
    //cs = 0;
		FPTB->PCOR   = 0;
}

static void disableChip(void) 
{
    //cs = 1;
		FPTB->PSOR   = 0;
}

static uint8_t readOp(uint8_t op, uint8_t address) 
{
    uint8_t result;
    
    enableChip();
    
//    spi.write(op | (address & ADDR_MASK));
		spi_send(op | (address & ADDR_MASK));
//    result = spi.write(0x00);
		spi_send(0x00);
    if (address & 0x80)
//        result = spi.write(0x00);
					spi_send(0x00);
    
    disableChip();
    
    //pc.printf("readOp result %02X\r\n",result);
    
    return result;
}
 
static void writeOp(uint8_t op, uint8_t address, uint8_t data) 
{
 
    //pc.printf("writeOp %02X %02X %02X\r\n",op,address,data);
    
    enableChip();
    
//    spi.write(op | (address & ADDR_MASK));
		spi_send(op | (address & ADDR_MASK));
//    spi.write(data);
		spi_send(data);
    
    disableChip();
}

static void SetBank (uint8_t address) 
{
    if ((address & BANK_MASK) != Enc28j60Bank) 
    {
        writeOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_BSEL1|ECON1_BSEL0);
        Enc28j60Bank = address & BANK_MASK;
        writeOp(ENC28J60_BIT_FIELD_SET, ECON1, Enc28j60Bank>>5);
    }
}

static uint8_t readRegByte (uint8_t address) 
{
    SetBank(address);
    return readOp(ENC28J60_READ_CTRL_REG, address);
}

static uint16_t readReg(uint8_t address) 
{
    return readRegByte(address) + (readRegByte(address+1) << 8);
}
 
static void writeRegByte (uint8_t address, uint8_t data) 
{
    SetBank(address);
    writeOp(ENC28J60_WRITE_CTRL_REG, address, data);
}
 
static void writeReg(uint8_t address, uint16_t data) 
{
    writeRegByte(address, data);
    writeRegByte(address + 1, data >> 8);
}

void enableBroadcast () 
{
    writeRegByte(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN|ERXFCON_BCEN);
}
 
void disableBroadcast () 
{
    writeRegByte(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN);
}
 
void disableMulticast () 
{ // disable multicast filter , enable multicast reception
    writeRegByte(ERXFCON, ERXFCON_CRCEN);
}

static uint16_t readPhyByte(uint8_t address) 
{
    writeRegByte(MIREGADR, address);
    writeRegByte(MICMD, MICMD_MIIRD);
    while (readRegByte(MISTAT) & MISTAT_BUSY);
    writeRegByte(MICMD, 0x00);
    return readRegByte(MIRD+1);
}

static void writePhy(uint8_t address, uint16_t data) 
{
    writeRegByte(MIREGADR, address);
    writeReg(MIWR, data);
    while (readRegByte(MISTAT) & MISTAT_BUSY)
        ;
}
 
static void writeBuf(const uint8_t* data, uint16_t len) 
{
    enableChip();
    
//    spi.write(ENC28J60_WRITE_BUF_MEM);
		spi_send(ENC28J60_WRITE_BUF_MEM);
    while (len--)
//        spi.write(*data++);
					spi_send(*data++);
        
    disableChip();
}
 
static void readBuf(uint8_t* data, uint16_t len) 
{
    enableChip();
    
//    spi.write(ENC28J60_READ_BUF_MEM);
		spi_send(ENC28J60_READ_BUF_MEM);
    while (len--) {
//        *data++ = spi.write(0x00);
					*data++ = spi_send(0x00);
    }
    
    disableChip();
}
 
uint8_t isLinkUp(void) 
{
    return (readPhyByte(PHSTAT2) >> 2) & 1;
}

void packetSend(uint8_t *buffer, uint16_t len) 
{
//    pc.printf("packetSend %d bytes ...\r\n",len);
    while (readOp(ENC28J60_READ_CTRL_REG, ECON1) & ECON1_TXRTS)
    {
        if (readRegByte(EIR) & EIR_TXERIF) 
        {
            writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRST);
            writeOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRST);
        }
    }
    writeReg(EWRPT, TXSTART_INIT);
    writeReg(ETXND, TXSTART_INIT+len);
    writeOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);
    writeBuf(buffer,len);
    writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
}

uint16_t packetReceive(uint8_t *buffer, uint16_t bufferSize) 
{
			struct {
				uint16_t nextPacket;
				uint16_t byteCount;
				uint16_t status;
		} header;
    uint16_t len = 0;
    if (readRegByte(EPKTCNT) > 0) 
    {
        writeReg(ERDPT, gNextPacketPtr);
        
        readBuf((uint8_t*) &header, sizeof header);
 
        gNextPacketPtr  = header.nextPacket;
        len = header.byteCount - 4; //remove the CRC count
        if (len>bufferSize-1)
            len=bufferSize-1;
        if ((header.status & 0x80)==0)
            len = 0;
        else
            readBuf(buffer,len);
        buffer[len] = 0;
        if (gNextPacketPtr - 1 > RXSTOP_INIT)
            writeReg(ERXRDPT, RXSTOP_INIT);
        else
            writeReg(ERXRDPT, gNextPacketPtr - 1);
        writeOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
    }
    return len;
}

// Contributed by Alex M. Based on code from: http://blog.derouineau.fr
//                  /2011/07/putting-enc28j60-ethernet-controler-in-sleep-mode/
void powerDown() 
{
    writeOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_RXEN);
    while(readRegByte(ESTAT) & ESTAT_RXBUSY);
    while(readRegByte(ECON1) & ECON1_TXRTS);
    writeOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_VRPS);
    writeOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PWRSV);
}
 
void powerUp() 
{
    writeOp(ENC28J60_BIT_FIELD_CLR, ECON2, ECON2_PWRSV);
    while(!readRegByte(ESTAT) & ESTAT_CLKRDY);
    writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
}

static int initialize(void) 
{    
    uint8_t rev;
    writeOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
    wait(2);
    
    while(!readOp(ENC28J60_READ_CTRL_REG, ESTAT) & ESTAT_CLKRDY);
 
    gNextPacketPtr = RXSTART_INIT;
    writeReg(ERXST, RXSTART_INIT);
    writeReg(ERXRDPT, RXSTART_INIT);
    writeReg(ERXND, RXSTOP_INIT);
    writeReg(ETXST, TXSTART_INIT);
    writeReg(ETXND, TXSTOP_INIT);
    enableBroadcast(); // change to add ERXFCON_BCEN recommended by epam
    writeReg(EPMM0, 0x303f);
    writeReg(EPMCS, 0xf7f9);
    writeRegByte(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
    writeRegByte(MACON2, 0x00);
    writeOp(ENC28J60_BIT_FIELD_SET, MACON3,
                        MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);
    writeReg(MAIPG, 0x0C12);
    writeRegByte(MABBIPG, 0x12);
    writeReg(MAMXFL, MAX_FRAMELEN);  
    writeRegByte(MAADR5, macaddr[0]);
    writeRegByte(MAADR4, macaddr[1]);
    writeRegByte(MAADR3, macaddr[2]);
    writeRegByte(MAADR2, macaddr[3]);
    writeRegByte(MAADR1, macaddr[4]);
    writeRegByte(MAADR0, macaddr[5]);
    writePhy(PHCON2, PHCON2_HDLDIS);
    SetBank(ECON1);
    writeOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);
    writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
 
    rev = readRegByte(EREVID);
    // microchip forgot to step the number on the silcon when they
    // released the revision B7. 6 is now rev B7. We still have
    // to see what they do when they release B8. At the moment
    // there is no B8 out yet
    if (rev > 5) ++rev;
    return rev;
}

