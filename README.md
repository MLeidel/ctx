# ctx
### _command-line clipboard text saving / retrieving (exchange)_

Wrote this for fun. Don't know if I'll really use it much.  
Incorporates several new functions I've added to the __myc.h__ project.  
Writing C functions to do higher level stuff is fun &#x1F60A;

ctx lets you save and recall text from/to your system clipboard or command-line,  
in order to recall previous saved text back to your clipboard.

Currently ctx has 5 functions:

- (many args saves as one string to list)
- (s) pastes clipboard contents to list
- (d) deletes a saved item from list
- (l) lists items and prompts to put an item back in the clipboard
- (m) moves a saved item from the list to index 0 of the list



![ctx](images/ctx_sshot.png "ctx run at command-line")


Makes heavy use of the [my_h c library](https://github.com/MLeidel/myc "myc.h"):  
```c
readfile  
open_for_write, open_for_append  
strcon  
clr_fg color codes sturcture  
contains  
cbcopy  
cbpaste  
list_def  
list_split  
list_display  
list_remove
list_inject
list_string
```

see: [my_h c library](https://github.com/MLeidel/myc "myc.h")

---

