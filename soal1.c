 
    #define FUSE_USE_VERSION 28
    #include <fuse.h>
    #include <stdio.h>
    #include <string.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <dirent.h>
    #include <errno.h>
    #include <sys/time.h>
     
    static const char *pathdoc = "/home/jwilyandi19/Documents";

	const char *get_filename_ext(const char *filename) {
		const char *dot = strrchr(filename,'.');
		if(!dot || dot==filename) return "";
		return dot+1;
	}

	int checker(const char *filename) {
		printf("%s\n",get_filename_ext(filename));
		if(strcmp(get_filename_ext(filename),"pdf")==0 || strcmp(get_filename_ext(filename),"doc")==0 || strcmp(get_filename_ext(filename),"txt")==0) 
		return 1;

		return 0;
	}
     
    static int jsn_getattr(const char *path, struct stat *stbuf)
    {
      int res;
      char fpath[1000];
      sprintf(fpath,"%s%s",pathdoc, path);
	//printf("%s\n",fpath);
      res = lstat(fpath, stbuf);
     
      if (res == -1)
        return -errno;
     
      return 0;
    }
     
    static int jsn_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
     	      off_t offset, struct fuse_file_info *fi)
    {
     char fpath[1000];
      if(strcmp(path,"/") == 0)
      {
        path=pathdoc;
        sprintf(fpath,"%s",path);
      }
      else sprintf(fpath, "%s%s",pathdoc,path);
      int res = 0;
     
      DIR *dp;
      struct dirent *de;
     
      (void) offset;
      (void) fi;
     
      dp = opendir(fpath);
      if (dp == NULL)
        return -errno;
     
      while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        res = (filler(buf, de->d_name, &st, 0));
          if(res!=0) break;
      }
     
      closedir(dp);
      return 0;
    }
     
    static int jsn_read(const char *path, char *buf, size_t size, off_t offset,
     	   struct fuse_file_info *fi)
    {
      char fpath[1000];
      if(strcmp(path,"/") == 0)
      {
       path=pathdoc;
        sprintf(fpath,"%s",path);
      }
      else {
     
        sprintf(fpath, "%s%s",pathdoc,path);
      }
      int res = 0;
      int fd = 0 ;
	char source[1000],target[1000],com[1000],coma[1000],comz[1000];
	DIR *tardir = opendir("/home/jwilyandi19/rahasia");
	if(checker(fpath)==1) {
		if(ENOENT==errno) system("mkdir /home/jwilyandi19/rahasia"); //jwilyandi19 bisa diganti user
		sprintf(source,"%s",fpath);
		sprintf(target,"%s.ditandai",fpath);
		int ret = rename(source,target);
		sprintf(comz,"chmod 000 %s",fpath);
		sprintf(com,"chmod 000 %s.ditandai",fpath);
		system(comz);
		sprintf(coma,"mv %s.ditandai /home/jwilyandi19/rahasia",fpath);
		//system(comz);

		system(com);
		system("zenity --error --text=\"Terjadi Kesalahan! File berisi konten berbahaya.\n\" --title=\"Error!\"");
		system(coma);
		return 1;
		
	}
else {
		
     
      (void) fi;
      fd = open(fpath, O_RDONLY);
      if (fd == -1)
        return -errno;
     
      res = pread(fd, buf, size, offset);
      if (res == -1)
        res = -errno;
     
      close(fd);
      return res;
	}
    }
     
    static struct fuse_operations xmp_oper = {
      .getattr  = jsn_getattr,
      .readdir  = jsn_readdir,
      .read   = jsn_read,
    };
     
    int main(int argc, char *argv[])
    {
	//printf("%s\n",get_filename_ext("haha/haha/gfs.pdf"));
      umask(0);
      return fuse_main(argc, argv, &xmp_oper, NULL);
}
