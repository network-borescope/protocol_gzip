import sys
import gzip

fin = sys.stdin
lines = fin.readlines()

content = ""
for line in lines:
	content += line

content = bytes(content, 'utf-8')

with gzip.open('python.gz', 'wb') as f:
    f.write(content)
