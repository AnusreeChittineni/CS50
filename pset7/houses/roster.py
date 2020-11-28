from sys import argv, exit
from cs50 import SQL

# assign db to premade database
db = SQL("sqlite:///students.db")

# checks number of command-line arguments
if len(argv) != 2:

    print("Usage: python import.py house")
    exit()

# initializes rows to all the rows in inputted house
rows = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first", argv[1])

# loops through each row of rows and prints the name and birth year
for row in rows:

    if row["middle"] == None:

        print("%s %s, born %d" % (row["first"], row["last"], row["birth"]))

    else:

        print("%s %s %s, born %d" % (row["first"], row["middle"], row["last"], row["birth"]))
