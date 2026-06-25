import sys

with open(sys.argv[1], 'r') as f:
    content = f.read()

name = sys.argv[2]
print('/* auto-generated   DO NOT EDIT */')
print(f'static const char* {name} =')
for line in content.splitlines():
    escaped = line.replace('\\', '\\\\').replace('"', '\\"')
    print(f'    "{escaped}\\n"')
print(';')
