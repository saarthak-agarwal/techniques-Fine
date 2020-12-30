import re, sys


def numberOfOccurences(file):
    tag_pattern = re.compile(r"<(?P<tag>\w+)(?P<attr>[\s\S]*?)>")
    d = dict()
    for line in file:
        ans = tag_pattern.findall(line)
        if ans:
            for i in ans:
                t = i[0]
                if t not in d.keys():
                    d[t] = 0
                d[t] += 1
    return d


def numberOfUnclosed(file):
    open_tag_pattern = re.compile(r"<(?P<tag>\w+)(?P<attr>[\s\S]*?)>")
    close_tag_pattern = re.compile(r"</(?P<tag>\w+)(?P<attr>[\s\S]*?)>")
    text = ""
    d = dict()
    for i in file:
        text = text + i
        ans = open_tag_pattern.search(text)
        if ans:
            tag = ans.group("tag")
            if tag in d.keys():
                d[tag] += 1
            else:
                d[tag] = 1
            text = ""
            continue

        ans = close_tag_pattern.search(text)
        if ans:
            tag = ans.group("tag")
            if tag in d.keys() and d[tag] > 0:
                d[tag] -= 1
            text = ""

    ans = 0
    for i in d:
        ans += d[i]
    return ans


def numberOfMaxDepth(text, tag):
    t = ""
    level = 0
    last_tag = ""
    tag_level = 0
    d = dict()
    tag_found = 0
    tag_omatch = re.compile(r"<\s*" + tag + r"\s*>")
    tag_ematch = re.compile(r"<\s*/\s*" + tag + r"\s*>")
    any_open = re.compile(r"<(?P<tag>\w+)(?P<attr>[\s\S]*?)>")
    any_close = re.compile(r"</(?P<tag>\w+)(?P<attr>[\s\S]*?)>")
    count = 0
    ans = 0
    for i in text:
        t = t + i
        match_any = any_open.search(t)
        if match_any:

            if level < 3:
                d[match_any.group("tag")] = 1
            if level <= 3:
                level += 1
                if level == 3:
                    last_tag = match_any.group("tag")

            matched = tag_omatch.search(t)
            if matched:
                tag_found = 1
                tag_level = level
                count = 0

            if tag_found and level == tag_level + 1:
                count += 1
            if count >= 3:
                ans += 1
                count = 0
                tag_found = 0

            t = ""

        match_any = any_close.search(t)
        if match_any:
            z = match_any.group("tag")
            if z in d.keys() and d[z] > 0 and level <= 3:
                d[z] = d[z] - 1
                level -= 1
            elif level == 4 and last_tag == z:
                level = 2

            matched = tag_ematch.search(t)
            if matched:
                tag_found = 0
                tag_level = 0
                count = 0
            t = ""

    return ans


arg_file = sys.argv[1]
file = open(arg_file, "r")
fc = open(arg_file, "r")

text = ""
for line in file:
    text = text + line


t = 4
while t:
    user_input = input()
    op = user_input[0]

    if op == "a":
        op, tag = user_input.split()
        opened_tags_count = numberOfOccurences(fc)
        print(opened_tags_count[tag])

    elif op == "b":
        print(numberOfUnclosed(text))

    elif op == "c":
        pattern = r"<*/?(?P<tag>\w+)>\s*(?:(?:(?:\+\+\+\+\+)?(?:\?\?\?)+(?:\*\*)+)|(?:(?:\+\+)(?:\?\?)+(?:\*\*\*)+))[^\+\?\*]+?"
        ans = re.findall(pattern, text)
        print(len(ans))

    elif op == "d":
        op, tag = user_input.split()
        print(numberOfMaxDepth(text, tag))

    t -= 1
