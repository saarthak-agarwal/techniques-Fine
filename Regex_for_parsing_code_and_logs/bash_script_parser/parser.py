import re, sys


def removeAliases(text):
    alias = re.compile(r"alias\s*(?P<variable>\w+)='(?P<value>.*)'")
    l = text.split("\n")
    d = dict()
    for i in range(len(l)):
        l[i] = l[i] + "\n"

    for i in range(len(l)):
        line = l[i]
        is_alias = alias.match(line)
        if is_alias:
            var = is_alias.group("variable")
            val = is_alias.group("value")
            d[var] = val

    for i in range(len(l)):
        line = l[i]
        ans = alias.search(l[i])
        if ans:
            var = ans.group("variable")
            tvar = var
            val = d[var]

            if val != "TO BE REMOVED":
                while val in d.keys() and d[val] != "TO BE REMOVED":
                    var = val
                    val = d[val]
                    d[var] = "TO BE REMOVED"

                if val != d[tvar] and d[tvar] != "TO BE REMOVED":
                    l[i] = re.sub("'" + d[tvar] + "'", "'" + val + "'", l[i])
                elif val == d[tvar]:
                    continue
                else:
                    l[i] = "\n"
            else:
                l[i] = "\n"

    return "".join(l)


def removeComments(text):
    comments = re.compile(r"#[ \t]*.*")
    l = text.split("\n")
    for i in range(len(l)):
        l[i] = l[i] + "\n"

    for i in range(1, len(l)):
        line = l[i]
        is_comment = comments.search(line)
        if is_comment:
            l[i] = comments.sub("", l[i])
    return "".join(l)


def removeEmptyLines(text):
    l = text.split("\n")
    for i in range(len(l)):
        l[i] = l[i] + "\n"

    empty_lines = re.compile(r"\s*")

    for i in range(len(l)):
        is_empty = empty_lines.fullmatch(l[i])
        if is_empty:
            l[i] = ""

    return "".join(l)


def removeIF(text):
    if_pattern = re.compile(r".*\sif[\S\s]+?[\s;]fi[\s;]")
    l = text.split("\n")
    d = dict()
    for i in range(len(l)):
        l[i] = l[i] + "\n"
    nt = ""
    for i in range(len(text) - 1, -1, -1):
        nt = text[i] + nt
        ans = if_pattern.match(nt)
        if ans:
            nt = if_pattern.sub(" \n", nt)

    return nt


def removeIndentation(text):
    l = text.split("\n")
    for i in range(len(l)):
        l[i] = l[i] + "\n"
    for i in range(1, len(l)):
        line = l[i]
        l[i] = re.sub(r"^[ \t]+", "", line)
    return "".join(l)


# Read the file
file = open(sys.argv[1], "r", encoding="utf8")
text = ""

# Convert it to a string
for l in file:
    text = text + l

# Remove Indentation
text = removeIndentation(text)

# Remove Comments
text = removeComments(text)

# Removing if-fi
t = removeIF(text)

# Removing empty functions
func_pattern = re.compile(
    r"(?:(?:\w+\s*\(\s*\))|(?:function\s+\w+\s*))\s*(?:.*?)?\s*\{\s*\}"
)
t = func_pattern.sub("\n", t)

# Removing transient aliases
removed_Aliases = removeAliases(t)

# Removing empty lines
final_ans = removeEmptyLines(removed_Aliases)
print(final_ans)

final_output = open("PES1201801811_A2.sh", "w")
print(final_ans, file=final_output, end="")
