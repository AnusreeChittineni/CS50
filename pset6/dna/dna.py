from csv import reader, DictReader
from sys import argv


# checks number of command-line arguments
if len(argv) < 3:
    print("Usage: python dna.py data.csv sequence.txt")
    
    exit()

# read the dna sequence from the file
with open(argv[2]) as dnafile:
    
    dnareader = reader(dnafile)
    
    for row in dnareader:
        
        dnalist = row

# store it in a string
dna = dnalist[0]

# create a dictionary where we will store the sequences we intend to count
sequences = {}

# extract the sequences from the database into a list
with open(argv[1]) as peoplefile:
    
    people = reader(peoplefile)
    
    for row in people:
        
        dnaSequences = row
        
        dnaSequences.pop(0)
        
        break

# copy the list into a dictionary where the genes are the keys
for item in dnaSequences:
    
    sequences[item] = 1

# iterate through the dna sequence, when it finds repetitions of the values from sequence dictionary it counts them
for key in sequences:
    
    l = len(key)
    
    Max = 0
    
    temp = 0
    
    for i in range(len(dna)):
        
        # resets the value of temp as to avoid double counting
        temp = 0

        # if the segment of dna corresponds to the key and there is a repetition of it we start counting
        if dna[i: i + l] == key:
            
            while dna[i - l: i] == dna[i: i + l]:
                
                temp += 1
                
                i += l

            # it compares the value to the previous longest sequence and if it is longer it overrides it
            if temp > Max:
                
                Max = temp

    # store the longest sequences in the dictionary using the corresponding key
    sequences[key] += Max

# open and iterate through the database of people and compare each one to the sequences
with open(argv[1], newline='') as peoplefile:
    
    people = DictReader(peoplefile)
    
    for person in people:
        
        match = 0
        
        # compares the sequences to every person and prints name before leaving the program if there is a match
        for dna in sequences:
            
            if sequences[dna] == int(person[dna]):
                
                match += 1
                
        if match == len(sequences):
            
            print(person['name'])
            exit()

    print("No match")
    
