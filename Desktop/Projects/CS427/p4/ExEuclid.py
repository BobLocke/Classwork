import math
import random

def gcd(a, b):
	r1 = a % b
	if (r1 != 0):
		return gcd(b, r1)
	else:
		return b


def ExEuclid(m, b):
	A = [1, 0, m]
	B = [0, 1, b]
	while 1:
		if B[2] == 0:
			return 0
		elif B[2] == 1:
			return B[0]
		else:
			Q = A[2] / B[2]
			T = [A[0] - Q * B[0], A[1] - Q * B[1], A[2] - Q * B[2]]
			A = B
			B = T

if __name__ == "__main__":
	print ExEuclid(19, 26)
	print ExEuclid(17, 101)
	print ExEuclid(1234, 4321)
	print ExEuclid(550, 1769)
	print ExEuclid(5, 24)
	print ExEuclid(31, 3480)