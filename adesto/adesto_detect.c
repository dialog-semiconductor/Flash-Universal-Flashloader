#include "adesto_defs.h"

#define JEDECID_READ_BUF_SIZE 12
#define JEDEC_ESCAPE_CODE 0x7f

// Manufacturer JEDEC codes and code locations
#define ATMEL_MANUF_ID 0x1f
#define ADESTO_MANUF_ID 0x43
#define MCIX_MANUF_ID 0xc2
#define WINBOND_MANUF_ID 0xef
#define ISSI_MANUF_ID 0x9d
#define MICRON_MANUF_ID 0x2c


static const flash_part_info_t adesto_part_tab[] =
{
	{/* AT25SF128A */ 0x1000000 /*=128 Mbit*/, {0x89, 0x1}, 3, false},
	{/* AT25SL128A */ 0x1000000 /*=128 Mbit*/, {0x42, 0x18}, 3, false},
	{/* AT25QL128A */ 0x1000000 /*=128 Mbit*/, {0x42, 0x18}, 3, false},
	{/* AT25SF641 */ 0x800000 /*=64 Mbit*/, {0x32, 0x17}, 3, false},
	{/* AT25QF641 */ 0x800000 /*=64 Mbit*/, {0x32, 0x17}, 3, false},
	{/* AT25DF641A */ 0x800000 /*=64 Mbit*/, {0x48, 0x0}, 3, false},
	{/* AT25SL641 */ 0x800000 /*=64 Mbit*/, {0x43, 0x17}, 3, false},
	{/* AT25QL641 */ 0x800000 /*=64 Mbit*/, {0x43, 0x17}, 3, false},
	{/* AT25SL321 */ 0x400000 /*=32 Mbit*/, {0x42, 0x16}, 3, false},
	{/* AT25DF321A */ 0x400000 /*=32 Mbit*/, {0x47, 0x1}, 3, false},
	{/* AT25SF321 */ 0x400000 /*=32 Mbit*/, {0x87, 0x1}, 3, false},
	{/* AT25QL321 */ 0x400000 /*=32 Mbit*/, {0x42, 0x16}, 3, false},
	{/* AT25SF161 */ 0x200000 /*=16 Mbit*/, {0x86, 0x1}, 3, false},
	{/* AT25DL161 */ 0x200000 /*=16 Mbit*/, {0x46, 0x3}, 3, false},
	{/* AT25SF081 */ 0x100000 /*=8 Mbit*/, {0x85, 0x1}, 3, false},
	{/* AT25DF081A */ 0x100000 /*=8 Mbit*/, {0x45, 0x1}, 3, false},
	{/* AT25DL081 */ 0x100000 /*=8 Mbit*/, {0x45, 0x2}, 3, false},
	{/* AT25SF041 */ 0x80000 /*=4 Mbit*/, {0x84, 0x1}, 3, false},
	{/* AT25DN256 */ 0x8000 /*=256 Kbit*/, {0x40, 0x0}, 3, false},
	{/* AT25DF256 */ 0x8000 /*=256 Kbit*/, {0x40, 0x0}, 3, false},
	{/* AT25DN512C */ 0x10000 /*=512 Kbit*/, {0x65, 0x1}, 3, false},
	{/* AT25DF512C */ 0x10000 /*=512 Kbit*/, {0x65, 0x1}, 3, false},
	{/* AT25XE512C */ 0x10000 /*=512 Kbit*/, {0x65, 0x1}, 3, false},
	{/* AT25DN011 */ 0x20000 /*=1 Mbit*/, {0x42, 0x0}, 3, false},
	{/* AT25DF011 */ 0x20000 /*=1 Mbit*/, {0x42, 0x0}, 3, false},
	{/* AT25XE011 */ 0x20000 /*=1 Mbit*/, {0x42, 0x0}, 3, false},
	{/* AT25DF021A */ 0x40000 /*=2 Mbit*/, {0x43, 0x1}, 3, false},
	{/* AT25XE021A */ 0x40000 /*=2 Mbit*/, {0x43, 0x1}, 3, false},
	{/* AT25XV021A */ 0x40000 /*=2 Mbit*/, {0x43, 0x1}, 3, false},
	{/* AT25DF041B */ 0x80000 /*=4 Mbit*/, {0x44, 0x2}, 3, false},
	{/* AT25XE041B */ 0x80000 /*=4 Mbit*/, {0x44, 0x2}, 3, false},
	{/* AT25XV041B */ 0x80000 /*=4 Mbit*/, {0x44, 0x2}, 3, false},
	{/* AT25XE041D */ 0x80000 /*=4 Mbit*/, {0x44, 0xc}, 3, false},
	{/* AT25XE081D */ 0x100000 /*=8 Mbit*/, {0x45, 0xc}, 3, false},
	{/* AT25XE161D */ 0x200000 /*=16 Mbit*/, {0x46, 0xc}, 3, false},
	{/* AT25XE321D */ 0x400000 /*=32 Mbit*/, {0x47, 0xc}, 3, false},
	{/* AT25FF041A */ 0x80000 /*=4 Mbit*/, {0x44, 0x8}, 3, false},
	{/* AT25FF081A */ 0x100000 /*=4 Mbit*/, {0x45, 0x8}, 3, false},
	{/* AT25FF161A */ 0x200000 /*=4 Mbit*/, {0x46, 0x8}, 3, false},
	{/* AT25FF321A */ 0x400000 /*=4 Mbit*/, {0x47, 0x8}, 3, false},
	{/* AT25FF0641A */ 0x800000 /*=4 Mbit*/, {0x48, 0x8}, 3, false},
	{/* AT25FF1281A */ 0x1000000 /*=4 Mbit*/, {0x49, 0x8}, 3, false},
	{/* AT45DB021E */ 0x40000 /*=2 Mbit*/, {0x23, 0x0}, 3, false},
	{/* AT45DB041E */ 0x80000 /*=4 Mbit*/, {0x24, 0x0}, 3, false},
	{/* AT45DB081E */ 0x100000 /*=8 Mbit*/, {0x25, 0x0}, 3, false},
	{/* AT45DB161E */ 0x200000 /*=16 Mbit*/, {0x26, 0x0}, 3, false},
	{/* AT45DQ161 */ 0x200000 /*=16 Mbit*/, {0x26, 0x0}, 3, false},
	{/* AT45DB321E */ 0x400000 /*=32 Mbit*/, {0x27, 0x1}, 3, false},
	{/* AT45DB321F */ 0x400000 /*=32 Mbit*/, {0x27, 0x1}, 3, false},
	{/* AT45DB322F */ 0x400000 /*=32 Mbit*/, {0x27, 0x2}, 3, false},
	{/* AT45DQ321 */ 0x400000 /*=32 Mbit*/, {0x27, 0x1}, 3, false},
	{/* AT45DB641E */ 0x800000 /*=64 Mbit*/, {0x28, 0x0}, 3, false},
	{/* AT25PE20 */ 0x40000 /*=2 Mbit*/, {0x23, 0x0}, 3, false},
	{/* AT25PE40 */ 0x80000 /*=4 Mbit*/, {0x24, 0x0}, 3, false},
	{/* AT25PE80 */ 0x100000 /*=8 Mbit*/, {0x25, 0x0}, 3, false},
	{/* AT25PE16 */ 0x200000 /*=16 Mbit*/, {0x26, 0x0}, 3, false},
	{/* ATXP032 */ 0x400000 /*=32 Mbit*/, {0xa7, 0x0}, 4, true},
	// Note: we temporarily keep support for device ID {0xa8, 0x0} as some
	// old ATXP064 parts are still around.
	{/* ATXP064 */ 0x800000 /*=64 Mbit*/, {0xa8, 0x0}, 4, true},
	{/* ATXP064B */ 0x800000 /*=64 Mbit*/, {0xa8, 0x1}, 4, true},
	{/* ATXP128 */ 0x1000000 /*=128 Mbit*/, {0xa9, 0x0}, 4, true},
};


#define ADESTO_PART_TAB_SIZE (sizeof(adesto_part_tab)/sizeof(flash_part_info_t))

static uint8_t jedecid_read_buf[JEDECID_READ_BUF_SIZE];

///////////////////////////////////////////////////////////////////////////////
// Function: adesto_part_detect
// This function attemptsto detect the part's JEDEC ID which includes
// Manufacturer info and device info. 
// Arguments:
// none
// Returns:
// A pointer to the part information structure
///////////////////////////////////////////////////////////////////////////////
const flash_part_info_t * adesto_part_detect(void)
{
	uint32_t i, j, status;

	for(i = 0; i < JEDECID_READ_BUF_SIZE-2; i++)
	{
		status = adesto_read_jedec_id(i+1, jedecid_read_buf);
		if(status != ADESTO_STATUS_OK)
			return NULL;
		if(jedecid_read_buf[i] != JEDEC_ESCAPE_CODE)
			break;
	}

	if(i == JEDECID_READ_BUF_SIZE-2)
		// searched too far down without finding manuf id
		return NULL;

	if((jedecid_read_buf[i] == ATMEL_MANUF_ID && i == 0) ||
		(jedecid_read_buf[i] == ADESTO_MANUF_ID && i == 7))
	{
		// detected a valid Atmel/Adesto manuf ID; now read device ID
		status = adesto_read_jedec_id(i+3, jedecid_read_buf);
		if(status != ADESTO_STATUS_OK)
			return NULL;
	}
	else
		return NULL;

	// Search currently goes all the way down because the key (device ID 1/2) is
	// not unique. But for now we're reporting only the first entry we find.
	for(j = 0; j < ADESTO_PART_TAB_SIZE; j++)
		if(adesto_part_tab[j].device_id[0] == jedecid_read_buf[i+1] && adesto_part_tab[j].device_id[1] == jedecid_read_buf[i+2])
			return &adesto_part_tab[j];

	return NULL;
}
