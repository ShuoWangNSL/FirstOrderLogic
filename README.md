# FirstOrderLogic

## Takes some first order logic statements as input

Examples:

Mother(Liz,Charley)

Father(Charley,Billy)

~Mother(x,y)|Parent(x,y)

~Father(x,y)|Parent(x,y)

~Parent(x,y)|Ancestor(x,y)

~Parent(x,y)|~Ancestor(y,z)|Ancestor(x,z)

## Judge the correctness of other statements

Examples:

Ancestor(Liz,Billy)

Ancestor(Liz,Joe)
