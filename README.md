Just a simple tool to save the file btime (birth time) to the xattrs while waiting for a `crtime` like this:

```
$ stat file
...
Access: ....
Modify: ....
Create: .... <-- this
Change: ....
 Birth: ....
```

### Reason
How the hell is the creation timestamp of a file (NOT of the inode) not a thing yet, in the same way `mtime` is a thing and is carried over even through web downloads.

### Building
Run `make` and it will compile the program to `btime-to-xattr`.

### Usage
```
Usage: ./btime-to-xattr <path>
```

### TODO
- util to show the saved btime xattr
- AUR package
