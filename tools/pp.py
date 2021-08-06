import sys

def add():
    for i in range(64):
        for j in range(64): 
            print('#define NX_PP_ADD_' + str(i) + '_' + str(j) + ' ' + str(i+j))

def ge():
    for i in range(64):
        for j in range(64): 
            print('#define NX_PP_GREATER_EQUAL_' + str(i) + '_' + str(j) + ' ' + str(1 if i >= j else 0))


if __name__ == '__main__':
    if sys.argv[1] == 'add':
        add()
    elif sys.argv[1] == 'ge':
        ge()