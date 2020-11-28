from csv import reader, DictReader
from sys import argv, exit
from cs50 import SQL

# assign db to premade database
db = SQL("sqlite:///students.db")

# checks number of command-line arguments
if len(argv) != 2:

    print("Usage: python import.py data.csv")
    exit()

# read the students information from the file
with open(argv[1]) as file:

    reader = DictReader(file)

    # gets values from every row in the file for the table
    for row in reader:

        name = row["name"].split()
        house = row["house"]
        birth = int(row["birth"])

        if len(name) == 3:

            first_name = name[0]

            middle_name = name[1]

            last_name = name[2]

        elif len(name) == 2:

            first_name = name[0]

            middle_name = None

            last_name = name[1]

        # inserts values into students table of students db
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                   first_name, middle_name, last_name, house, birth)
