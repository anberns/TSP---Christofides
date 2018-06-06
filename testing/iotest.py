fileName = raw_input("Enter the name of the file to open: ");
with open(fileName, "r") as fo:
    contents = fo.read()
    print(contents.rstrip())
    lines = fo.readlines()
    for line in lines:
        print(line.rstrip())
