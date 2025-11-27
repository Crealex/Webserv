#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
     struct stat buf;
     stat("./Post.cpp",&buf);

	std::cout << "struct stat:" << std::endl;
     std::cout << "	st_dev: " << buf.st_dev << std::endl;
	 std::cout << "	st_ino: "<< buf.st_ino << std::endl;
     std::cout << "	st_mode: " << mode_t(buf.st_mode) << std::endl;
     std::cout << "	st_nlink: " << buf.st_nlink << std::endl;
     std::cout << "	st_uid: " << buf.st_uid << std::endl;
     std::cout << "	st_gid: " << buf.st_gid << std::endl;
     std::cout << "	st_rdev: " << buf.st_rdev << std::endl;
     std::cout << "	st_size: " << buf.st_size << std::endl;
     std::cout << "	st_blksize: " << buf.st_blksize << std::endl;
     std::cout << "	st_blocks: " << buf.st_blocks << std::endl;
     std::cout << "	st_atime: " << buf.st_atime << std::endl;
     std::cout << "	st_mtime: " << buf.st_mtime << std::endl;
     std::cout << "	st_ctime: " << buf.st_ctime << std::endl;
 }
