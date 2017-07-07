
import math

def test(a):
  return math.sin(a)   

def test2(a): 
  s = 0 
  for i in range(0,4): s += i +a     
  return math.sin(a) +s  
