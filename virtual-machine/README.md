# Virtual stack machine


## What is this?
###### Short description
**Stack machine** is computer processor or a virtual machine in which the primary interaction is moving short-lived temporary values to and from a push down stack.
###### View full in [Wiki](https://en.wikipedia.org/wiki/Stack_machine)

## Example
How it's displayed in high-level languages (for example in *Python*)
```python
λ python3
>>> 5 + 6
11
```

What is happening in Virtual Machine
```c
PSH 5   ;   /* Push the value to the top of the stack */
PSH 6   ;   /* Push the value to the top of the stack */
ADD 5 6 ;   /* Pop two last elements from stack, add them and push to the top of the stack */
POP 11  ;   /* Pop the last element from the stack and print it on display */
HLT     ;   /* Exit the program and clear all variables */
```

## Article
[How to implement Virtual Machine in C](https://felix.engineer/blogs/virtual-machine-in-c)
