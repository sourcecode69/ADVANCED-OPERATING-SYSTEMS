

1. Display prompt -- this is to make our name of the terminal. I just used get cwd and updated the current directory.
2. Change directory -- in this I mainly used the chdir command to change the directories. and on overstep I am updating the previous and current directory for "-" .I used enviorment variable home for home path .
3. Pwdprint-- This just sprints the name of current directory we are in.
4. echo -- this is just to print the open we have after echo command.
5. ls - I made a function list file, which helped me do ls function. First I handled ~ and used it for home path .This works on the path we have given and the flags used for it . We are using current directory and previous directory global variables . We used library dirent.h which has the usage for dir directory and then open the directory to read its data using opendir call.then we just sort the entries and print the data that is read by the opener call.
We are also keeping bool variable for -an and -l flags so that we can implement its functionality . Then closer is used to free the space and close the file .
6. search -- this is used to search a file in the system and return truer false if the file is present or not.
This is done recursively.we just have to use reader and closed to make this call as it is done recursively.it uses functionality of "." & ".." to iterate through directories.
7. inbackground --  this is function to keep track of the background process in the system and storing its pid for info later if needed. We made a global vector backgroundprocesses and store all the pids there.
We use fork and execvp calls for this, fork for child processes and execvp to load the command in the chid , and for foreground processes we even have the waited for parent process.
But amongst this all we are always changing the current directory and previous directory global variable .
8. history -  this was giving some problem in Mac but till now I have made a history vector nd stored all the input there so when I get the command history it prints the last 10 commands used in the file.
9. i/o redirection - this in not working yet, but till now I have used dup2 and open system calls, later will be doing it via sleek and open and read and write cause its not working in my Mac.


Note --  pinfo is not working good in Mac so that is not in code as it was giving error if I was trying to implement all the functions for it.so till now , I have not updated the pinfo in the file submitted.
