# Research Report Merkle Patricia Trie

通过此次实验了解了MPT所涉及的数据结构，通过阅读网上资料和相关的论文综述进行总结，下面是实验报告的主要内容

## 1. Definition

> MPT的全称为Merkle Patricia Trie，是一种融合了 Merkle Tree 和 Prefix Tree 优点的数据结构。它在区块链技术和密码学领域得到广泛应用，尤其是以太坊等智能合约平台中。Merkle Patricia Trie是对传统前缀树的改进，引入了Merkle树的概念，使其具备高效验证和防篡改的特性。

## 2. Conception

### 2.1 Trie

前缀树是一种树状数据结构，用于存储字符串键和关联的值。每个节点表示一个字符，从根节点开始，根据字符依次向下遍历。在Merkle Patricia Trie中，每个节点都存储着它的子节点的哈希值，以实现数据的完整性验证。

### 2.2 Merkle Tree

Merkle Tree是一种哈希树结构，用于对大量数据进行快速验证。Merkle树通过将数据分割成小块，并为每个块计算哈希值，然后逐层聚合这些哈希值，最终得到根哈希，称为Merkle根。当数据发生改变时，根哈希也会随之改变，从而实现数据的完整性验证。

## 3. MPT's Principle

### 3.1 Struct

Merkle Patricia Trie使用Trie的结构，将键值对存储在树中的叶子节点。与传统Trie不同的是，Merkle Patricia Trie使用哈希值而不是直接存储值，这样可以确保数据的不可变性和高效验证。

### 3.2 Build

在Merkle Patricia Trie中，每个节点都有一个唯一的哈希标识，其由节点的类型、键、值以及子节点的哈希值共同组成。节点的类型包括：分支节点、叶子节点和扩展节点。

### 3.3 Compression

Merkle Patricia Trie使用路径压缩技术，通过合并路径上相邻的单一分支节点和扩展节点，减少树的高度，节省存储空间，并提高访问效率。

## 4. Learning

下面我们一起探讨一下MPT的有关性质

![](https://oyrd-1313391192.cos.ap-nanjing.myqcloud.com/images/image-20230720145332691.png)

### 4.1 Q1

> Question: existence and non-existence proof with MPT?

这个点没啥好说的，融合了Merkel Tree的性质，存在性或不存在性证明上都是类似的

### 4.2 Q2

>Question: this is just mental model, how to impl?

按照上面图片进行实现即可

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 64

struct Node {
    char key;
    char* value;
    char hash[HASH_SIZE];
    struct Node* children[16];
};

struct Node* create_node(char key, char* value) {
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->key = key;
    new_node->value = value;
    new_node->hash[0] = '\0';
    memset(new_node->children, 0, sizeof(new_node->children));
    return new_node;
}

char* hash_node(struct Node* node) {
    unsigned int hash = 0;
    for (int i = 0; i < strlen(node->value); ++i) {
        hash = (hash * 31) + node->value[i];
    }
    snprintf(node->hash, HASH_SIZE, "%08x", hash);
    return node->hash;
}

struct Node* insert(struct Node* node, char* key, char* value) {
    if (*key == '\0') {
        if (node == NULL) {
            return create_node('\0', value);
        }
        node->value = value;
        return node;
    }

    int index = (*key >= 'a') ? (*key - 'a' + 10) : (*key - '0');
    if (node == NULL) {
        node = create_node(*key, NULL);
    }
    node->children[index] = insert(node->children[index], key + 1, value);
    hash_node(node);
    return node;
}

struct Node* search(struct Node* node, char* key) {
    if (node == NULL) {
        return NULL;
    }
    if (*key == '\0') {
        return node;
    }

    int index = (*key >= 'a') ? (*key - 'a' + 10) : (*key - '0');
    return search(node->children[index], key + 1);
}
```

### 4.3 Q3

> Question: can you propose new type Merkle tree ?

这里可以搜索相关的Merkle Tree变种，下面做简要介绍

**Shrubs Merkle Tree**

Shrubs默克尔树的核心思想是将传统的Merkle树转换为一种基于森林结构的数据组织方式。每个树都包含一部分数据。通过组合多个小型Merkle树，Shrubs默克尔树避免了单个大型Merkle树的性能瓶颈。为一些分布式系统和区块链平台提供了一种高效的数据结构，使得数据更新和验证操作更加轻松，同时保持较低的存储和计算开销。然而，具体的实现和优化可能因应用场景而异，需要根据实际需求来进行调整和优化。

**Sparse Merkle Tree**

Sparse Merkle Tree是一种优化的默克尔树变体，旨在减少存储空间和计算成本，特别适用于大规模数据集的情况。相比传统的默克尔树，稀疏默克尔树只存储实际存在的数据项，而不需要为全部叶节点分配空间。其核心思想是使用哈希链表来表示数据集。哈希链表是一个由哈希值组成的链表，其中每个哈希值对应一个数据项。当数据集较为稀疏时，很多叶节点将是空的，而稀疏默克尔树通过哈希链表仅存储实际存在的数据项，从而节省了存储空间。

## 5. Reference

https://zhuanlan.zhihu.com/p/96659723

https://www.simplilearn.com/tutorials/blockchain-tutorial/merkle-tree-in-blockchain