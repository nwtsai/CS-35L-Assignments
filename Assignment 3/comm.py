#!/usr/bin/python

import string, sys, locale, random
from optparse import OptionParser

class comm:

    def __init__(self, args, file1, file2, nonSortedFiles, allFiles):

        # This if-else structure reads lines depending on the file options inputted by the user
        if args[0] == "-":
            self.input1 = sys.stdin.readlines()
        else:
            f1le = open(args[0], 'r')
            self.input1 = f1le.readlines()
            f1le.close()

        if args[1] == "-":
            self.input2 = sys.stdin.readlines()
        else:
            f2le = open(args[1], 'r')
            self.input2 = f2le.readlines()
            f2le.close()

        # Assign file1 variables
        self.file1 = file1
        self.name1 = args[0]
        self.input1 = self.appendNewLine(self.input1)
        
        # Assign file2 variables
        self.file2 = file2
        self.name2 = args[1]
        self.input2 = self.appendNewLine(self.input2)
        
        # Assign variables for all files and unsorted files
        self.unsorted = nonSortedFiles
        self.allFiles = allFiles

    # Return true if the file is sorted; if file is not sorted, output error message and return false
    def isFileSorted(self):
        
        isSorted = True
        if sorted(self.input1) != self.input1:
            isSorted = False
            sys.stderr.write("comm.py: " + self.name1 + " is not in sorted order\n")

        if sorted(self.input2) != self.input2:
            isSorted = False
            sys.stderr.write("comm.py: " + self.name2 + " is not in sorted order\n")

        return isSorted

    # Append a new line after a file if a new line does not exist there already 
    def appendNewLine(self, file):

        # This if not statement works because the file is simply a list of lines
        if not len(file):
            file.append("\n")
        else:
            finalLine = file[-1]
            # If a new line is not found, add it to the last line of the document
            if "\n" not in finalLine:
                finalLine += "\n"
            file[-1] = finalLine
        return file

    # Compare both files and then print the correct output 
    def compareAndPrint(self):
        if self.unsorted:
            file2Only = self.input2
            for text1 in self.input1:
                if text1 not in self.input2:
                    self.outputRows(text1, 1)
                else:
                    self.outputRows(text1, 3)
                    file2Only.remove(text1)

            for text2 in file2Only:
                self.outputRows(text2, 2)
            return

        if not self.isFileSorted():
            return

        text1Length = len(self.input1)
        text2Length = len(self.input2)
        a = 0 
        b = 0

        while a in range(text1Length) or b in range(text2Length):
            text1, text2 = "", ""

            if a < text1Length:
                text1 = self.input1[a]
            if b < text2Length:
                text2 = self.input2[b]

            if text1 == "" or (text2 != "" and text1 > text2):
                self.outputRows(text2, 2)
                b += 1
            elif text2 == "" or text2 > text1:
                self.outputRows(text1, 1)
                a += 1
            else:
                self.outputRows(text2, 3)
                a += 1
                b += 1
    
        return

    # Output the correct rows, including a tab character to create the illusion of columns
    def outputRows(self, currLine, colNum):

        # Depending on the inputted options, print tabs in between columns if needed
        output = ""
        if colNum == 1:
            if not self.file1:
                return
        if colNum == 2:
            if not self.file2:
                return
            if self.file1:
                output += "\t"
        if colNum == 3:
            if not self.allFiles:
                return
            if self.file1:
                output += "\t"
            if self.file2:
                output += "\t"

        # Output the tabs in between columns and then the given current line of the document
        sys.stdout.write(output + currLine)

# The main function
def main():
    versionText = "%prog 2.0"
    usageText = """%prog [OPTION]... FILE1 FILE2
    Compare files FILE1 and FILE2 line by line.                                                                                                                                                                    
    When FILE1 or FILE2 (not both files) is -, read standard input.                                                                                                                                                
    -1 suppress column 1 (lines unique to FILE1)                                                                                                                                                                   
    -2 suppress column 2 (lines unique to FILE2)                                                                                                                                                                   
    -3 suppress column 3 (lines that appear in both files)                                                                                                                                                         
    -u Allows sorting of unsorted files                                                                                                                                                                            
    """
    
    # These are the options for comm                                                                                                                                                   
    parser = OptionParser(version=versionText, usage=usageText)
    parser.add_option("-1", action = "store_false", dest = "file1", default = True, help = "Suppress the output column of lines unique to file1.")
    parser.add_option("-2", action = "store_false", dest = "file2", default = True, help = "Suppress the output column of lines unique to file2.")
    parser.add_option("-3", action = "store_false", dest= "allFiles", default = True, help = "Suppress the output column of lines duplicated in file1 and file2.")
    parser.add_option("-u", action = "store_true", dest= "nonSortedFiles", default = False, help = "Works with unsorted files.")
    options, args = parser.parse_args(sys.argv[1:])

    # File input error messages
    if len(args) != 2:
        parser.error("Wrong number of input operands")

    if args[0] == "-" and args[1] == "-":
        parser.error("Cannot read both files")
    
    # Argument input error messages
    try:
        file1 = bool(options.file1)
    except:
        parser.error("Undefined argument for -1".format(options.file1))
    try:
        file2 = bool(options.file2)
    except:
        parser.error("Undefined argument for -2".format(options.file2))
    try:
        allFiles = bool(options.allFiles)
    except:
        parser.error("Undefined argument for -3".format(options.allFiles))
    try:
        nonSortedFiles = bool(options.nonSortedFiles)
    except:
        parser.error("Undefined argument for -u".format(options.nonSortedFiles))

    # System error messages
    try:
        comp = comm(args, file1, file2, nonSortedFiles, allFiles)
    except OSError as err:
        parser.error("OS Error: {0}".format(err))
    try:
        comp.compareAndPrint()
    except:
        parser.error("error")

if __name__ == "__main__":
    main()
