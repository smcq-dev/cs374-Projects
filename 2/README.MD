This project recursively searches all subdirectories of a directory specified at the command line. It counts the directory size of each directory in post-order, so all subdirectory sizes are printed before there parent directory. Eventually the size of the specified directory will be displayed at the end of the function. This program ignores everything but directories and regular files, meaning things like symbolic links aren't taken into account. 

main()
  |
  ├── fprintf() 
  ├── exit() 
  └── scan_directories()
        |
        ├── opendir()
        ├── perror() 
        ├── exit() 
        ├── readdir()
        ├── strcmp()
        ├── snprintf()
        ├── lstat()
        ├── closedir()
        ├── printf()
        └── scan_directories()