#ifndef FDISK_DOS_LABEL_H
#define FDISK_DOS_LABEL_H

/*
 * per partition table entry data
 *
 * The four primary partitions have the same sectorbuffer (MBRbuffer)
 * and have NULL ext_pointer.
 * Each logical partition table entry has two pointers, one for the
 * partition and one link to the next one.
 */
struct pte {
	struct partition *part_table;	/* points into sectorbuffer */
	struct partition *ext_pointer;	/* points into sectorbuffer */
	char changed;			/* boolean */
	sector_t offset;	        /* disk sector number */
	unsigned char *sectorbuffer;	/* disk sector contents */
};

extern struct pte ptes[MAXIMUM_PARTS];

#define pt_offset(b, n)	((struct partition *)((b) + 0x1be + \
					      (n) * sizeof(struct partition)))

extern int ext_index; /* the prime extended partition */
extern sector_t extended_offset;

/* A valid partition table sector ends in 0x55 0xaa */
static inline unsigned int part_table_flag(unsigned char *b)
{
	return ((unsigned int) b[510]) + (((unsigned int) b[511]) << 8);
}

static inline sector_t get_partition_start(struct pte *pe)
{
	return pe->offset + get_start_sect(pe->part_table);
}

extern void dos_print_mbr_id(struct fdisk_context *cxt);
extern void dos_set_mbr_id(struct fdisk_context *cxt);
extern void dos_init(struct fdisk_context *cxt);

extern int dos_list_table(struct fdisk_context *cxt, int xtra);
extern void dos_list_table_expert(struct fdisk_context *cxt, int extend);

extern void dos_fix_partition_table_order(void);
extern void dos_move_begin(struct fdisk_context *cxt, int i);

extern int mbr_is_valid_magic(unsigned char *b);

extern void change_units(struct fdisk_context *cxt);
extern void update_units(struct fdisk_context *cxt);	/* called from sunlabel too */

#define is_dos_compatible(_x) \
		   (fdisk_is_disklabel(_x, DOS) && \
                    fdisk_dos_is_compatible(fdisk_context_get_label(_x, NULL)))

#endif
