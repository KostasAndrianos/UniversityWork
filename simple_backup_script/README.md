Simple Backup Shell Script

This script takes a path and creates a backup of it in a tar file in another path.
If you give an existing tar as the second path, it will append the files of the first path to the existing tar file.
It is really simple but doesn't have many features. 
If you like, you can put the prgbckp file in the same path with the script and automate this task.

USAGE:
```
./bckp1 username path_to_backup path_to_put_the_backup
```
AUTOMATED:
```
./bckp1 username path_to_backup path_to_put_the_backup time
```
Time in
```
"now + 1 min"
```
You can also use absolute time format.
My script is using "at". You need to install "at" first and set it up.

ToDo:
Add tar.gz support
Add multi directory support
