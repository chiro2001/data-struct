import sys

if len(sys.argv) != 2:
    print("python utf8.py [file]")
    sys.exit(0)

try:
    data = None
    with open(sys.argv[1], 'r', encoding='gbk') as f:
        data = f.read()
    with open(sys.argv[1], 'w', encoding='utf8') as f:
        f.write(data)
except Exception as e:
    print(f'Can not convert: {e}')
    sys.exit(1)

