#!/usr/bin/python3

# Brandon truong

import random, sys, argparse, string

class Shuffle:
    #Stores lines to shuffle in lines, options in repeat and head_count
    def __init__(self, lines, repeat, head_count):
        #Shuffles with random lib
        random.shuffle(lines)
        self.lines = lines
        self.repeat = repeat
        self.head_count = head_count
        #iterator init
        self.i = 0

    #Create iterator and next for looping
    def __iter__(self):
        return self
        
    def __next__(self):
        #Case for repeating, repeats new permutations
        if (self.repeat==True and self.i == len(self.lines)):
            #Reset i and shuffle
            self.i=0
            lines=self.lines
            random.shuffle(lines)
            self.lines=lines
        #Reached end of head_count or end of lines
        if self.head_count == 0 or self.i == len(self.lines):
            raise StopIteration
        #Random sample a line if repeated and with head_count, else random permutation
        if self.repeat==True and self.head_count != -1:
            result = random.choice(self.lines) #Random sampling
        else: 
            result = self.lines[self.i] #Random permutation
            self.i += 1
        #If specified a head_count, then loops only that many times
        if self.head_count > 0:
            self.head_count -= 1
        return result

#Checks if options are valid and constructs Shuffle if so
def process_arguments(options, args):
    #Default -1 mean will not decrement to 0, loops until reaches end of lines or repeats
    if options.head_count is None:
        head_count = -1
    else:
        head_count = int(options.head_count) #convert to int if in integer format, else traceback error from casting
        if head_count < 0:
            parser.error("Invalid head count: {}".format(head_count))
        #Exits without taking in stdin or anything, but doesn't fail
        if head_count == 0:
            exit(0)
    #Checks if argument specifies a range of integers
    if options.input_range is not None:
        #Check for mutually exclusive
        if len(args) != 0:
            parser.error("Cannot use input file with input range")
        #Check if valid input range, then converts str to int if so
        try:
            str_min_value, str_max_value = (options.input_range).split("-", maxsplit=1)
        #Error in inputted range, didn't split properly
        except ValueError:
            parser.error("Invalid range: {}" .format(options.input_range))
        #Checks if valid integer, otherwise error
        if str_min_value.isdigit() and str_max_value.isdigit():
            min_value=int(str_min_value)
            max_value=int(str_max_value)
        else:
            parser.error("Invalid format: {}" .format(options.input_range))
        #Checks if 0<=LO<=HI
        if max_value < min_value or min_value < 0:
            parser.error("Invalid range: {}, not within 0<=LO<=HI" .format(options.input_range))
        #Assigns to list string version of input range
        lines = [str(x) for x in range(min_value, max_value + 1)]
    #Checks if no arguments, read from stdin
    elif len(args) == 0 or args[0] == '-':
        lines = sys.stdin.readlines()
    #Specified input file
    else:
        #Check if there is only one input file
        if len(args) != 1:
            parser.error("Invalid number of operands")
        #Reads lines from file into lines
        filename = args[0]
        input_file = open(filename, 'r')
        lines = input_file.read().splitlines()
        input_file.close()
    
    shuf = Shuffle(lines, options.repeat, head_count)
    for val in shuf:
        sys.stdout.write(val +"\n")


if __name__ == "__main__":
    # this main function should be as minimal as possible
    description_msg="Shuffles its input into random permutations"
    #Parse all the arguments into options, leftover in args
    parser = argparse.ArgumentParser(description=description_msg)
    # parser.add_argument(...)
    parser.add_argument("-n", "--head-count", help="Prints out at most n lines. If used in conjunction with --repeat, then randomly samples input",
                        dest="head_count", default=None, action="store")
    parser.add_argument("-r", "--repeat", help="Repeats output values indefinitely or up to --head-count times",
                        dest="repeat", default=False, action="store_true")
    parser.add_argument("-i", "--input-range", help="Prints out as if file contains integers min-max",
                        dest="input_range", default=None, action="store")
    options, args = parser.parse_known_args()
    process_arguments(options, args)


