from hashlib import sha256
import time
import copy


def build_tree(leaves):
    tree = []
    chain = [sha256(item).digest() for item in leaves]
    while len(chain)>1:
        if len(chain)%2:
            chain.append(chain[-1])
        chain = [sha256(chain[i]+chain[i+1]).digest() for i in range(0, len(chain), 2)]
        tree.append(chain)
    return tree, chain[0]


db = [str(time.time()).encode() for _ in range(1024)]
tree, root = build_tree(db)

def verify(data):
    if build_tree(data)[1] == root:
        return True
    return False


def search_error(data):
    error_tree = build_tree(data)[0]
    idx = 0
    for _ in range(len(error_tree)-2, -1, -1):
        for choice in range(2):
            if error_tree[_][idx+choice] != tree[_][idx+choice]:
                idx = 2*(idx+choice)
                continue
    if data[idx] != db[idx]:
        return idx
    else:
        return idx+1


def test():
    db2 = copy.deepcopy(db)
    db2[255] = b'hello'
    print(verify(db), verify(db2))
    print("error data idx: ", search_error(db2))


if __name__ == '__main__':
    test()