#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <fcntl.h>
#include <csignal>

using namespace std;

#define MAX_COMMAND_LENGTH 1024
#define HISTORY_SIZE 20
#define HISTORY_DISPLAY 10

vector<pid_t> backgroundProcesses;
string currentDirectory;
string previousDirectory;

vector<string> commandHistory(HISTORY_SIZE);
int historyIndex = 0;
int historyCount = 0;

// ------------------------------display--------------------------//
void displayPrompt() {
    char cwd[MAX_COMMAND_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        currentDirectory = cwd;
        cout << cwd << "~";
    } else {
        perror("getcwd");
    }
}

// Function to handle the 'cd' command
// ------------------------------cd--------------------------//
void changeDirectory(const string &path) {

    char cwd[MAX_COMMAND_LENGTH];

    if (path.empty() || path == "~") {
        const char *homePath = getenv("HOME");
        if (homePath != NULL) {
            int res = chdir(homePath);
            if (res == 0) {
                previousDirectory = currentDirectory;
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    currentDirectory = cwd;
                } else {
                    perror("getcwd");
                }
            } else {
                perror("chdir");
            }
        } else {
            cerr << "Home directory path not found" << endl;
        }
    } else if (path == "-") {
        int res = chdir(previousDirectory.c_str());

        if (res == 0) {
            swap(currentDirectory, previousDirectory);
        } else {
            perror("chdir");
        }
    } else if (path == "..") {
        int res = chdir("..");
        if (res == 0) {
            previousDirectory = currentDirectory;
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                currentDirectory = cwd; 
            } else {
                perror("getcwd");
            }
        } else {
            perror("chdir");
        }
    } else {
        int res = chdir(path.c_str());
        if (res == 0) {
            previousDirectory = currentDirectory;
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                currentDirectory = cwd;
            } else {
                perror("getcwd");
            }
        } else {
            perror("chdir");
        }
    }
}


// ------------------------------pwd--------------------------//
void pwdprint() {
    
    char cwd[MAX_COMMAND_LENGTH];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        cout << cwd << endl;
    } else {
        perror("getcwd");
    }
}


// ------------------------------echo--------------------------//
void echoCommand(const vector<string> &tokens) {
    // Your 'echo' function code
    for (int i = 1; i < tokens.size(); i++) {
        cout << tokens[i];

        if (i < tokens.size() - 1) {
            cout << " ";
        }
    }
    cout << endl;
}


// ------------------------------ls--------------------------//
void listFiles(const vector<string> &flags, const string &path = currentDirectory) {
   
    string directoryPath = path; // path hai jo bheja 
    bool parentDir = false;      

   //special character check krne ke liye
    if (path == "~") {
        const char *homePath = getenv("HOME");
        if (homePath != NULL) {
            directoryPath = homePath; 
        } else {
            cerr << "Home directory path not found" << endl;
            return;
        }
    } else if (path == "..") {
        parentDir = true;
        directoryPath = previousDirectory; 
    } else if (path == ".") {
        directoryPath = currentDirectory;
    }

    DIR *dir;
    struct dirent *entry;

    dir = opendir(directoryPath.c_str());

    if (dir != NULL) {
        vector<string> entries;
        entry = readdir(dir);

        while (entry != NULL) {
            entries.push_back(entry->d_name);
            entry = readdir(dir); 
        }

        sort(entries.begin(), entries.end());

        bool showHidden = false;
        bool longFormat = false;

        for (const string &flag : flags) {
            if (flag == "-a") {
                showHidden = true;
            } else if (flag == "-l") {
                longFormat = true;
            }
        }

        for (const string &entryName : entries) {
            if (!showHidden && entryName[0] == '.') {
                continue;
            }

            string fullPath = directoryPath + '/' + entryName;

            if (longFormat) {
                struct stat fileStat;
                if (stat(fullPath.c_str(), &fileStat) == 0) {
                    cout << setw(10) << left << entryName;
                    cout << " " << setw(5) << left << fileStat.st_size;
                    cout << " " << setw(15) << left << getpwuid(fileStat.st_uid)->pw_name;
                    cout << " " << setw(15) << left << getgrgid(fileStat.st_gid)->gr_name;

                    char dateBuffer[20];
                    strftime(dateBuffer, sizeof(dateBuffer), "%b %d %H:%M", localtime(&fileStat.st_mtime));
                    cout << " " << setw(12) << left << dateBuffer;

                    cout << endl;
                } else {
                    perror("stat");
                }
            } else {
                cout << entryName << " " << endl;
            }
        }
        cout << endl;
        closedir(dir); //close 
    } else {
        perror("opendir");
    }

    if (parentDir) {
        currentDirectory = directoryPath;
    }
}


// ------------------------------search--------------------------//
bool search(const string &currentDirectory, const string &target) {
    
    DIR *dir;
    struct dirent *entry;

    dir = opendir(currentDirectory.c_str());
    if (dir != NULL) {
        entry = readdir(dir);

        while (entry != NULL) {
            string entryName = entry->d_name;
            if (entryName == "." || entryName == "..") {
                entry = readdir(dir);
                continue;
            }

            string fullPath = currentDirectory + "/" + entryName;

    
            if (entryName == target) {
                closedir(dir);
                return true;
            }

            //recusive use
            if (entry->d_type == DT_DIR) {
                if (search(fullPath, target)) {
                    closedir(dir);
                    return true;
                }
            }

            entry = readdir(dir);
        }

        closedir(dir);
    }

    return false;
}


// ------------------------------background--------------------------//
void InBackground(const vector<string> &tokens) {

    pid_t child_pid = fork();

    if (child_pid < 0) {
        cerr << "Fork failed." << endl;
        exit(1);
    } else if (child_pid == 0) {
        // Child process
        // Tokenize the command
        char *arguments[tokens.size() + 1]; // One extra for NULL
        for (size_t i = 0; i < tokens.size(); i++) {
            arguments[i] = const_cast<char *>(tokens[i].c_str());
        }
        arguments[tokens.size()] = NULL;

        // Execute the command in the background
        execvp(arguments[0], arguments);

        // If execvp fails, report an error
        cerr << "Command execution failed." << endl;
        exit(1);
    } else {
        // Parent process
        // Print the PID of the background child process
        cout << "Background process PID: " << child_pid << endl;
        backgroundProcesses.push_back(child_pid);
    }
}

// Function for redirection
// ------------------------------redirection--------------------------//
void redirection(vector<string> &tokens, bool runInBackground) {
    int inputRedirect = -1; // File descriptor for input redirection
    int outputRedirect = -1; // File descriptor for output redirection

    // Look for input and output redirection symbols
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "<" && i < tokens.size() - 1) {
            // Input redirection
            inputRedirect = open(tokens[i + 1].c_str(), O_RDONLY);
            if (inputRedirect == -1) {
                cerr << "Error: Unable to open input file " << tokens[i + 1] << endl;
                return;
            }
            dup2(inputRedirect, STDIN_FILENO);
            close(inputRedirect);
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            i--;
        } else if (tokens[i] == ">" && i < tokens.size() - 1) {
            // Output redirection (overwrite)
            outputRedirect = open(tokens[i + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (outputRedirect == -1) {
                cerr << "Error: Unable to open or create output file " << tokens[i + 1] << endl;
                return;
            }
            dup2(outputRedirect, STDOUT_FILENO);
            close(outputRedirect);
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            i--;
        } else if (tokens[i] == ">>" && i < tokens.size() - 1) {
            // Output redirection (append)
            outputRedirect = open(tokens[i + 1].c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (outputRedirect == -1) {
                cerr << "Error: Unable to open or create output file " << tokens[i + 1] << endl;
                return;
            }
            dup2(outputRedirect, STDOUT_FILENO);
            close(outputRedirect);
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            i--;
        }
    }

    if (!tokens.empty() && tokens[0] == "echo") {
        // Handle echo separately
        for (size_t i = 1; i < tokens.size(); i++) {
            cout << tokens[i];

            if (i < tokens.size() - 1) {
                cout << " ";
            }
        }
        cout << endl;
    } else {
        // Tokenize the command
        char *arguments[tokens.size() + 1]; // One extra for NULL
        for (size_t i = 0; i < tokens.size(); i++) {
            arguments[i] = const_cast<char *>(tokens[i].c_str());
        }
        arguments[tokens.size()] = NULL;

        // Execute the command
        pid_t child_pid = fork();

        if (child_pid < 0) {
            cerr << "Fork failed." << endl;
            exit(1);
        } else if (child_pid == 0) {
            // Child process
            execvp(arguments[0], arguments);
            cerr << "Error: Command execution failed." << endl;
            exit(1);
        } else {
            // Parent process
            if (!runInBackground) {
                // If not running in the background, wait for the child to complete
                int status;
                waitpid(child_pid, &status, 0);
            } else {
                cout << "Background process PID: " << child_pid << endl;
            }
        }
    }


}

// -----------------------------history--------------------------//
void addToHistory(const string &command) {
    commandHistory[historyIndex] = command;
    historyIndex = (historyIndex + 1) % HISTORY_SIZE;
    if (historyCount < HISTORY_SIZE) {
        historyCount++;
    }
}
//--------------pinfo------------------------//
void pinfo(pid_t pid){

}


//-----------------------------------------------------------//
int main() {
    while (true) {
        displayPrompt();

        string input;
        getline(cin, input);

        addToHistory(input);

        vector<string> tokens;
        char *token = strtok(const_cast<char *>(input.c_str()), " ");
        while (token != NULL) {
            tokens.push_back(token);
            token = strtok(NULL, " ");
        }

        if (tokens.empty()) {
            continue;
        }

        bool runInBackground = false;
        if (!tokens.empty() && tokens.back() == "&") {
            runInBackground = true;
            tokens.pop_back();
        }

        vector<string> commandTokens;
        bool redirectionDetected = false;

        for (const string &token : tokens) {
            if (token == "<" || token == ">" || token == ">>") {
                redirectionDetected = true;
                break;
            } else {
                commandTokens.push_back(token);
            }
        }

        if (redirectionDetected) {
            redirection(commandTokens, runInBackground);
        } else if (commandTokens[0] == "cd") {
            if (commandTokens.size() > 1) {
                changeDirectory(commandTokens[1]);
            } else {
                changeDirectory("");
            }
        } else if (commandTokens[0] == "pwd") {
            pwdprint();
        } else if (commandTokens[0] == "echo") {
            echoCommand(commandTokens);
        } else if (commandTokens[0] == "ls") {
            vector<string> flags;
            string directoryName = currentDirectory;

            for (size_t i = 1; i < commandTokens.size(); i++) {
                if (commandTokens[i][0] == '-') {
                    flags.push_back(commandTokens[i]);
                } else {
                    directoryName = commandTokens[i];
                }
            }

            listFiles(flags, directoryName);
        } else if (commandTokens[0] == "search") {
            if (commandTokens.size() > 1) {
                string target = commandTokens[1];
                bool found = search(currentDirectory, target);
                cout << (found ? "true" : "false") << endl;
            } else {
                cout << "Usage: search <target>" << endl;
            }
        } else if (commandTokens[0] == "history") {
            int startIdx = (historyIndex - HISTORY_DISPLAY + HISTORY_SIZE) % HISTORY_SIZE;
            int endIdx = (historyIndex - 1 + HISTORY_SIZE) % HISTORY_SIZE;

            for (int i = startIdx; i <= endIdx; i++) {
                cout << i + 1 << ": " << commandHistory[i] << endl;
            }
        } else {
            // Create a child process for non-built-in commands
            pid_t child_pid = fork();

            if (child_pid < 0) {
                cerr << "Fork failed." << endl;
                exit(1);
            } else if (child_pid == 0) {
                // Child process
                // Tokenize the command
                char *arguments[commandTokens.size() + 1]; // null daalo end mein
                for (size_t i = 0; i < commandTokens.size(); i++) {
                    arguments[i] = const_cast<char *>(commandTokens[i].c_str());
                }
                arguments[commandTokens.size()] = NULL;

                // foreground comm
                execvp(arguments[0], arguments);

                // agar execvp chala toh ye nhi chalega 
                cerr << "Command execution failed." << endl;
                exit(1);
            } else {
                // Parent process
                if (!runInBackground) {

                    int status;
                    waitpid(child_pid, &status, 0);
                } else {
                    cout << "Background process PID: " << child_pid << endl;
                    backgroundProcesses.push_back(child_pid);
                }
            }
        }
    }

    return 0;
}
