typedef opaque buffer<>;
typedef string filename<50>;

program FS_PROG {
	version FS_VERSION {
		buffer read_file(filename, int, int) = 1;
		int write_file(filename, buffer, int) = 2;
	} = 1;
} = 555555555;
