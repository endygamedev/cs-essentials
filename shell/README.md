# `Shell`

## What is this?
###### Short description
Unix **shell** is a command-line interpreter or shell that provides a command line user interface for Unix-like operating systems. The shell is both an interactive command language and a scripting language, and is used by the operating system to control the execution of the system using shell scripts.
###### View full in [Wiki](https://en.wikipedia.org/wiki/Unix_shell)

## How it works?
A shell does three main things in its lifetime:
- **Initialize**: In this step, a typical shell would read and execute its configuration files. These change aspects of the shell's behavior.
- **Interpreter**: The shell reads commands from stdin (which could be interactive, or a file) and executes them.
- **Terminate:** After its commands are executed, the shell executes any shutdown commands, frees up any memory, and terminates.

## Command interpetation
Command processing is the same as in programming languages. The input is a string, which is split into tokens. The first token is the command itself, and the rest of the sequence is its arguments. For example:
```bash
mkdir test
```
In this example, the first token is the command name `mkdir`, followed by its argument `test`. Then we look for the command we need, for example in this case `mkdir`, which is located in the directory: `/usr/bin`, and then we give it our arguments.

Thus, the shell acts as an intermediary between the process call and the user input.

## Built-in commands
Of course, there are not all implemented processes and some things you have to do yourself. For example, commands such as: `cd`, `help`, `exit` etc.

For this purpose a header [`commands.h`](/src/commands.h) was created in which the built-in commands are specified.

## Article
[Stephen Brennan — «Write a Shell in C»](https://brennan.io/2015/01/16/write-a-shell-in-c/)
