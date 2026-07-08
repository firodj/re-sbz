# Digging STD C++


```cpp
class CCloth {     // size: 0x24
public:
    int id;
    char name[32];
};
```

# std::vector

In relse will be always 0x10 struct size.

```cpp
typedef std::vector<CCloth> CClothes;
```

```c
struct CClothes {       // size: 0x10
    int _Alval;         // 0x00 | unused
    Clothes *_Myfirst;  // 0x04 | first element (malloc, realloc)
    Clothes *_Mylast;   // 0x08 | current allocated size
    Clothes *_Myend;    // 0x0C | total capacity
}
```

But, the element size can be deducted from `sizeof(CCloth)`.

Initialize:

```c
void main() {
    CClothes clothes;

    clothes._Myfirst = 0;
    clothes._Mylast = 0;
    clothes._Myend = 0;
}
```

# std::map

```cpp
typedef std::map<int, CCloth> MClothes;
```

```c
struct MClothes {            // size: 0xC
    int _Alval;              // 0x00 | Allocator
    MClothes_Node *_Myhead;  // 0x04 | Root tree
    unsigned int _Msize      // 0x08 |
}

struct MClothes_Node {       // size: 0x38
    MClothes_Node *_Left;    // 0x00 |
    MClothes_Node *_Parent;  // 0x04 |
    MClothes_Node *_Right;   // 0x08 |
    MClothes_Pair _Myval;    // 0x0C |
    char _Color;             // 0x34 |
    char _Isnil;             // 0x35 |
}

struct MClothes_Pair { // size: 0x28
    const int first; // 0x00 | _Key 
    CCloth second;   // 0x04 | _Val
}
```

Initialize:

```c
MClothes_Buynode() {
    MClothes_Node *Node = new(sizeof(MClothes_Node));
    if (Node)
        Node->_Left = 0;
    if (Node != -4)
        Node->_Parent = 0;
    if (Node != -8)
        Node->_Right = 0;
    Node->_Color = 1;
    Node->_Isnil = 0;
}

void main() {
    MClothes mclothes;

    MClothes_Node * pHead = MClothes_Buynode();

    mclothes._Myhead = pHead;
    pHead->_Isnil = 1;
    mclothes._Myhead->_Parent = mclothes._Myhead;
    mclothes._Myhead->_Left = mclothes._Myhead;
    mclothes._Myhead->_Right = mclothes._Myhead;
    mclothes._Mysize = 0;
}
```