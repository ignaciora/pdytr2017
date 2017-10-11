#define VERSION_NUMBER 1

typedef opaque buffer<>;
typedef string filename<255>;

struct read_ret {
	buffer buffer;
	long bytes_read;
	long bytes_requested;
};

struct read_args {
	filename filename;
	long bytes;
	long offset;
};

struct write_args {
	filename filename;
	buffer buffer;
	long bytes;
};

program FS_PROG {
   version FS_VERSION {
     read_ret READ(read_args *args) = 1;
     long WRITE(write_args *args) = 2;
   } = VERSION_NUMBER;
} = 200000001;
