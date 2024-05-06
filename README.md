# (non-deterministic) Finite State Machine Execution (with empty state transitions)

This is a simple C program that allows you to test the validity of a NDFSM, see in.txt for an example of input. This program was made as an assignment for my Automata class, in which we had to model an arbitrary game's functionality as a state machine. I chose monkey island's map seller. apresentação.pdf and final_relatório.pdf are the files presented to the teacher (in portuguese).

## Usage

```bash
make
./main < in.txt
```

## Program logic

The program tries makes all posible transitions and saves all the current states that the program is in, removing duplicate current states.

More explanation will be provided if asked for.
