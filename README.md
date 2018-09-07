# linux_pipes

Inter-process communication(IPC) refers specifically to the mechanisms 
an operating system provides to allow the processes to manage shared data.

One of the IPC tool in linux is pipe.A pipe is a unidirectional stream.
It takes the output of one command as input and directs this input to the
next command.
Ex: command 1 | command 2 | command 3

I have implemented the pipe functionality in cpp with one additional feature.
The new feature works as follow:
consider an example: command1 | (command2) (command3) | command4
here, output of command1 will be provides to command2 and command3 and now the
combined output of command2 and command3 will be provided to command4.
