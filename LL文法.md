### 第三次作业

1，

1)$select(S-> aBc) = first(aBc) = \{a\}$

2)$select(S->bAB) = first(bAB) = \{b\}$

3)$select(A->aAb) = first(aAb) = \{a\}$

4)$select(A->b) = first(b) = \{b\}$

5)$select(B -> b) = first(b) = \{b\}$

6)$select(B->\varepsilon) = fllow(B) = first(c)\cup fllow(S) = \{c,\#\}$



2,是LL(1)文法，具有相同左部的产生式的选择符集合不相交，分析表如下：

|      | a    | b    | c    | #    |
| ---- | ---- | ---- | ---- | ---- |
| S    | 1    | 2    |      |      |
| A    | 3    | 4    |      |      |
| B    |      | 5    | 6    | 6    |

 

 