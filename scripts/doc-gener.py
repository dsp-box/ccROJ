#! /usr/bin/python3 -B

# *************************************************** *
# This file is a part of ccROJ project                *
# distributed under GNU General Public License v3.0.  *
# Please visit the webpage: github.com/dsp-box/ccROJ  *
# for more information.                               *
#                       contact: Krzysztof Czarnecki  *
#                email: czarnecki.krzysiek@gmail.com  *
# *************************************************** */

# *************************************************** */
# import

import sys, re

# *************************************************** */
# parser definition

class line_generator:

    def __init__(self, file_list):
        self.flist = file_list
        self.flags = {}

        self.flags["open"] = False
        self.flags["close"] = False
        self.flags["close2"] = False
        self.flags["block"] = False
        self.flags["sign"] = False
        self.flags["struct"] = False

    def get_internal_generator(self):
        for fname in self.flist:
            with open(fname, "r") as fd:
                for line in fd: yield line

    def __iter__(self):
        g = self.get_internal_generator()
        for line in g:
            if self.check_empty_line(line):
                continue
                    
            self.check_type_line(line)
            self.check_block_line(line)
            self.check_struct_line(line)

            # sign line modification
            if self.flags["sign"]:
                line = re.sub("\s*[{;]\s*$", "", line)
                line = line.replace("struct", " <span class='range'>struct</span>")
                line = line.replace("#define", "<span class='func'>define</span>")

                line = re.sub("([a-zA-Z0-9_]*)(?=\s*[(])", "<span class='func'>\\1</span>", line)
                line = re.sub("([a-zA-Z0-9_]*)(?=\s*[:]{2})", "<span class='range'>\\1</span>", line)
                line = re.sub("(?<=[:]{2})([~\sa-zA-Z0-9_]*)(?=[(])", "<span class='func'>\\1</span>", line)

                line = "<span class='bold'>%s</span>" % line
                line = "<span class='key'>@sign</span>: %s" % line
                    
            # remove initial *
            if self.flags["block"]:
                line = re.sub("^\s*[*]\s*", "", line)
                line = re.sub("([@][a-z]*)", "<span class='key'>\\1</span>", line)

            # struct line modification
            if self.flags["struct"]:
                line = re.sub("\s*[;]\s*$", "", line)

                line = re.sub("\s*([a-zA-Z0-9_]*)\s*$", " <span class='arg'>\\1</span>", line)
                line = "<span class='bold'>%s</span>" % line


            line = line.replace("\n", " ")
            if self.flags["sign"] or self.flags["block"] or self.flags["struct"]:
                line = re.sub("(\sa_[a-zA-Z0-9_]*)", "<span class='arg'>\\1</span>", line)
                line = re.sub("(\sm_[a-zA-Z0-9_]*)", "<span class='field'>\\1</span>", line)

                yield line
                self.flags["close2"] = self.flags["close"]


    def check_struct_line(self, line):
        ret = re.match("^\s*struct", line)
        if ret and self.flags["sign"]:
            self.flags["struct"] = True
        ret = re.match("^\s*};", line)
        if ret:
            self.flags["struct"] = False
            self.flags["close2"] = True
        
    def check_empty_line(self, line):
        ret = re.match("^\s*$", line)
        if ret:
            self.flags["sign"] = False
            self.flags["close"] = False
            self.flags["block"] = False
            self.flags["open"] = False
        if  re.match("^\s*[*]\s*$", line):
            ret = True
        return ret

    def check_type_line(self, line):
        ret = re.match("^\s*[*]\s*@type: private.*", line)
        if ret: return
        ret = re.match("^\s*[*]\s*@type.*", line)
        if ret:
            self.flags["open"] = True
            self.flags["block"] = True
            self.block_type = re.split('^\s*[*]\s*@type:\s*', line[0:-1])[1]
        else: self.flags["open"] = False
        
    def check_block_line(self, line):
        self.flags["sign"] = False
        ret = re.match("^\s*[*].*", line)
        if not ret: 
            if self.flags["block"]:
                self.flags["sign"] = True
            self.flags["block"] = False

        if re.match("\s*[*][/]\s*", line):
            self.flags["close"] = True
        else: self.flags["close"] = False

    
# *************************************************** */
# some additional functions

def print_line(line):
    print(line + "<br>")
    
def print_head():
    print("<html>")
    print("<head>")
    print("   <title>ccROJ</title>")
    print("   <link rel='stylesheet' href='style.css'>")
    print("   <script src='head.js'></script>")
    print("</head>")
    print("<body>")
    print("<br><br>")

def print_tail():
    print("<br>")
    print("</body>")
    print("</html>")


def print_files(file_list):
    print("<div class=\"file\">")
    print("<span class=\"key\">@files</span>:<br>")
    for i in file_list: 
        print(i, "<br>")    
    print("</div>")

# *************************************************** */
# generate

print_head()
flist = sys.argv[1:]
print_files(flist)

g = line_generator(flist)
for line in g:

    if not g.flags["sign"] and g.flags["close2"]:
        print("</div>")
        
    if g.flags["open"]:
        print("<br><div class='%s'>" % g.block_type)

    if  not g.flags["open"] and not g.flags["close"]: 
        print_line(line)

    if g.flags["sign"] and not g.flags["struct"]:
        print("</div>")

print_tail()
