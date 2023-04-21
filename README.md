# Bitvectors
The bitvector is a datastructure used to store bits. This is used to store information 
in a more compact manner so that space is not wasted. Bit manipulation techniques can be used
to handle data in a more accurate method.

OPERATIONS THAT CAN BE PERFORMED ARE:
    OR to set a bit to one: 11101010 OR 00000100 = 11101110
    AND to set a bit to zero: 11101010 AND 11111101 = 11101000
    AND to determine if a bit is set, by zero-testing:
        11101010 AND 00000001 = 00000000 = 0
        11101010 AND 00000010 = 00000010 ≠ 0
    XOR to invert or toggle a bit:
        11101010 XOR 00000100 = 11101110
        11101110 XOR 00000100 = 11101010
    NOT to invert all bits.
        NOT 10110010 = 01001101


  APPLICATIONS:
  -QUICKSAVE FEATURE IN GAMES
