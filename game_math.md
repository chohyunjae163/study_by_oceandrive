# QUATERNION
q1 = a + bi + cj + dk  
q2 = e + fi + gj + hk  
q1+q2 = a+e + (b+f)i + (c+g)j + (d+h)k  
q1q2 =  
ae + afi + agj + ahk  
bei + bfi^2 + bgij + bhik  
cej + cfji + cgj^2 + chjk  
dek + dfki + dgkj + dhk^2  

i^2 = -1  
j^2 = -1  
k^2 = -1  
ijk = -1
i^2=j^2=k^2= ijk = -1

iijk = -i
i^2jk = -i
-jk = -i
jk = i

j^2k = ji
-k = ji
-ki = ji^2
-ki = -j
ki = j

ik = -j
ki = j
ij = k
ji = -k

## Quaternion Multiplication
q1q2=
ae + afi + agj + ahk +  
bei - bf + bgk + bhj +  
cej - cfk - cg + chi +  
dek + dfj - dgi - dh

q1q2=  
(ae - bf - cg - dh) +    //scalar  
(af + be + ch - dg)i +   //vector  
(ag - bh + ce + df)j +   //vector  
(ah + bg - cf + de)k     //vector  

q1q2 != q2q1

ae - bf - cg - dh  
be + af - dg + ch  
ce + df + ag - bh  
de - cf + bg + ah  

[a -b -c -d] [e]  
[b  a -d  c] [f]  
[c  d  a -b] [g]  
[d -c  b  a] [h]  

(1,2,3,6)(0,1,0,0)  
[ 1 -2 -3 -6]  [ 0 ]  [ -2 ]  
[ 2  1 -6  3]  [ 1 ]  [  1 ]  
[ 3  6  1 -2]  [ 0 ]  [  6 ]  
[ 6 -3  2  1]  [ 0 ]  [ -3 ]  

1 = (1,0,0,0)  
i = (0,1,0,0)  
j = (0,0,1,0)  
k = (0,0,0,1)  





