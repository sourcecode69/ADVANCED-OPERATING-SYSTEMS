Question 1 :
Input -- ./a.out <file path>
Output -- Assignment1_1/1_<filename>.txt
Procedure :
We opened the file using the open function call and then applied access and getcwd to check if the file was already made or not.
It takes the input as a string and make a directory and file using the path.
Then, it uses the a buffer to change reverse the file in chunks.
Seek was used to transverse the file and read and write system calls in the file.
First it take a mod and reverse the mod part and then use rest is done.
It makes a new output file and revise is stored there.
Assumptions:
If the file is same then it would overwrite it.
If the file is not present then it would make a new file in the directory beside the old file.



Question2 :
Input -- ./a.out <file path> 
Input -- start end 
Output -- Assignment1_2/2_<filename>.txt

procedure :
This take the input file path and two inputs like from where to where we don't ha ve to reverse.
This has a function of reversing the file , the function takes file descriptor of input and output and start and end index to reverse and a file size.
We reversed the data from 0 to start and then copied the part as it is from start to end.
Then we revered a file from end to file end.
Lseek,open and wrote are the main system calls that were used prominently.
We are using percentage to show how much it has changed or completed.
Assumptions:
If the file is same then it would overwrite it.
If the file is not present then it would make a new file in the directory beside the old file.


Question3 :


Input type -- ./a.out <newfile path >  <oldfile path > <directory path>

Procedure:
This takes the input as path of new file and path of old file and a directory path.
In this I used 2 buffers that takes the path and returns if the new file is the reversed of the old file or not.
This also shows if the directory has been created or not.
Seek , read and write system calls were used prominently beside stat system call.
It uses stat system call to print all the permissions given to a files and directory.
