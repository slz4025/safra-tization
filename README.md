Safra algorithm from non-deterministic Buchi automata to deterministic Rabin automata in C++.

This project was a final project for the Computational Discrete Mathematics class at CMU.

## Command
Run `./safra BuechiExamples/[input_file] [out_file] <-t>`

The flag `-t` prints out the Safra trees generated by the Safra program in the order in which they are made.

Example: 
Running
`./safra BuechiExamples/buechi1.aut safra.aut`
should create the "safra.aut" file of the following [form](https://github.com/slz4025/safra-tization/blob/master/README.md#Output)...

## Input
The input file should be a Buchi (non-deterministic) automata with an ".aut" extension.  You can find existing examples in `BuechiExamples/`.  
The general format of the input is:
```
BUECHI
# Rabin size: <n', number of nodes in expected output Rabin automata>
# Rabin transitions: <|delta|, size of expected output Rabin transition function>
# Number of states
<n, number of nodes in input Buchi automata, where nodes are indexed from 1-n>
# Alphabet size
<|Sigma|, the size of the alphabet, where elements are indexed from 1-|Sigma|>
# Number of transitions
<|tau|, size of input Buchi transition relation, equals the number of lines between "# begin transitions" and "# end transitions">
# begin transitions
<three space-separated numbers: input node [1-n], alphabet character [1-|Sigma|], output node [1-n]>
<another three-tuple>
<another three-tuple>
<another three-tuple>
...
# end transitions
# Buechi initial
<I, space-separated list of the Buchi initial nodes>
# Buechi final
<F, space-separated list of the Buchi final nodes>
# Buechi eof
```

## Output
The output file will be the specification of a Rabin automata, made through the Safra algorithm.
It will be of the following format:
```
RABIN
# Buechi File: BuechiExamples/<input file>.aut
# Number of states
<n', number of states in Buchi automata>
# Alphabet size
<|Sigma|, size of the alphabet, where the elements are indexed by 1-|Sigma|>
# Number of transitions
<|delta|, size of Buchi automata transition function, equals the number of lines between "# begin transitions" and "# end transitions">
# begin transitions
<three space-separated numbers: input node [1-n], alphabet character [1-|Sigma|], output node [1-n]>
<another three-tuple>
<another three-tuple>
<another three-tuple>
...
# end transitions
# Number of Rabin pairs
<number of left/right Rabin pairs, equals the number of lines between "#Rabin pairs" and "#Rabin eof">
# Rabin pairs
<a pair of lists indicating which nodes are in the left/rejecting final or right/accepting final states of the pair>
<another pair>
<another pair>
<another pair>
...
# Rabin eof
```


