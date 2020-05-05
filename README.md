# InfiniteCalculator

USE:

calculator executable requires an input and output text file to be run
ex run of program:
./calculator in.txt out.txt

in.txt should contain one arithmetic phrase per line
out.txt will contain the answer to the arithmetic phrase on the corresponding line
for an example of correct input and output, see in.txt and out.txt


FEATURES:

-is infinite, meaning the numbers entered and output are not constrained by max interger size

-supports addition and subtraction (using + and * respectively)

-follows conventional order of operations, and allows for use of parantheses () to alter calculation order

-ignores entries like space and tab


LIMITATIONS:

-only allows for addition and multiplication, there is no subtraction, division, exponentiation, or other operations

-all numbers must be positive, negative numbers cannot be entered

-numbers must be decimal (base 10), no other bases are supported

-complex numbers are not supported


TESTING:
intest.txt contains the example input file given on the course website
outtest.txt contains the results of running calculator on intest.txt
outcorr.txt contains the expected output file given on the course website
the command "diff outtest.txt outcorr.txt" verifies both files are the same

in.txt contains the sample input from the course website and some additional input
out.txt contains the results of running calculator on in.txt
