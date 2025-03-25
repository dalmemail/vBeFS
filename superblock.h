struct vbefs_superblock
{
	char name[BFS_DISK_NAME_LENGTH];
	int32 magic1;
	int32 fs_byte_order;
	uint32 block_size;
	uint32 block_shift;
	off_t num_blocks;
	off_t used_blocks;
	int32 inode_size;
	int32 magic2;
	int32 blocks_per_ag;
	int32 ag_shift;
	int32 num_ags;
	int32 flags;
	block_run log_blocks;
	off_t log_start;
	off_t log_end;
	int32 magic3;
	inode_addr root_dir;
	inode_addr indices;
	int32 pad[8];
};

/*@
	// is n == 2^exp ?
	// TODO: This can probably be done in a more "idiomatic" way somehow
	predicate power_of_two(uint32 n, uint32 exp) = (exp == 0) ? n == 1 : (n % 2 == 0 &*& power_of_two(n / 2, exp - 1));
	predicate superblock(struct vbefs_superblock *superblock) =
		malloc_block_vbefs_superblock(superblock) &*&
		superblock->name |-> ?name &*& string(name, ?list_name) &*& length(list_name) < BFS_DISK_NAME_LENGTH &*&
		superblock->magic1 |-> ?magic1 &*& magic1 == SUPER_BLOCK_MAGIC1 &*&
		superblock->fs_byte_order |-> ?fs_byte_order &*& fs_byte_order == SUPER_BLOCK_FS_LITTLE_ENDIAN &*&
		superblock->block_size |-> ?block_size &*&
		superblock->block_shift |-> ?block_shift &*& block_size == power_of_two(block_shift) &*&
		superblock->num_blocks |-> ?num_blocks &*&
		superblock->used_blocks |-> ?used_blocks &*& used_blocks <= num_blocks &*&
		superblock->inode_size |-> ?inode_size &*&
		superblock->magic2 |-> ?magic2 &*& magic2 == SUPER_BLOCK_MAGIC2 &*&
		superblock->blocks_per_ag |-> ?bitmap_blocks_per_ag &*&
		superblock->ag_shift |-> ?ag_shift &*& power_of_two(bitmap_blocks_per_ag * block_size * 8, ag_shift) &*&
		superblock->num_ags |-> ?num_ags &*&
		superblock->flags |-> ?flags &*& flags == SUPER_BLOCK_DISK_CLEAN &*&
		superblock->log_blocks |-> ?log_blocks &*&
		superblock->log_start |-> ?log_start &*&
		superblock->log_end |-> ?log_end &*& block_run(log_blocks, log_start, log_end) &*&
		superblock->magic3 |-> ?magic3 &*& magic3 == SUPER_BLOCK_MAGIC3 &*&
		superblock->root_dir |-> ?root_dir &*& block_run(root_dir) &*&
		superblock->indices |-> ?indices &*& block_run(indices) &*&
		superblock->pad |-> _;
@*/

#define SUPER_BLOCK_MAGIC1 0x42465331 /* BFS1 */
#define SUPER_BLOCK_MAGIC2 0xdd121031
#define SUPER_BLOCK_MAGIC3 0x15b6830e

#define BFS_DISK_NAME_LENGTH 32

#define SUPER_BLOCK_FS_LITTLE_ENDIAN 'BIGE' /* BIGE */
#define SUPER_BLOCK_DISK_CLEAN 'CLEN' /* CLEN */
