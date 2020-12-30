import datetime
from math import floor
import re
import sys


def date_diff(date1, date2):
    d1 = datetime.datetime.fromisoformat(date1)
    d2 = datetime.datetime.fromisoformat(date2)
    diff = d1 - d2
    ans = int(floor(diff.total_seconds())) * 1000 + (diff.microseconds // (10 ** 3))
    return ans


file = sys.argv[1]
file = open(file, "r")
info = dict()
pattern = re.compile(
    r"(?P<time>\d\d\d\d-\d\d-\d\d \d\d:\d\d:\d\d(?:\.\d\d\d)?) .*\s(?P<sore>Starting|Ending)\sService\s(?P<name>.*)"
)
for line in file:
    g = pattern.match(line)
    if g:
        if g.group("sore") == "Starting":
            info[g.group("name")] = [g.group("time")]
        else:
            info[g.group("name")].append(g.group("time"))
final = []
for i in sorted(info, key=lambda x: info[x][0]):
    final.append([i, info[i][0], info[i][1], date_diff(info[i][1], info[i][0])])
for i in final:
    print(*i, sep=",")
